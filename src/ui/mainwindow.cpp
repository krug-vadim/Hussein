#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tasktreewidget.h"

#include <QtCore/QSignalMapper>

#include <QtGui/QCloseEvent>
#include <QtGui/QSessionManager>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "../serialization/yamlserialization.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	_doExit = false;

	ui->setupUi(this);

	connect(qApp, SIGNAL(commitDataRequest(QSessionManager&)),
	        this, SLOT(commitAppData(QSessionManager&)));

	createTrayIcon();

	_tabsWidgetModifyMapper = new QSignalMapper(this);
	connect(_tabsWidgetModifyMapper, SIGNAL(mapped(QWidget*)),
	        this, SLOT(taskListModified(QWidget*)));

	_tabsWidgetFileNameChangeMapper = new QSignalMapper(this);
	connect(_tabsWidgetFileNameChangeMapper, SIGNAL(mapped(QWidget*)),
	        this, SLOT(taskListFileNameChanged(QWidget*)));

	connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
	        this, SLOT(closeTab(int)));

	setupActions();

	loadSettings();

	//setCentralWidget( new TaskTreeWidget(_rootTask, this) );
}

MainWindow::~MainWindow()
{
	delete ui;
}

TaskTreeWidget * MainWindow::createNewTaskTreeWidget(const QString &title)
{
	TaskTreeWidget *taskTreeWidget = new TaskTreeWidget(this);

	int index = ui->tabWidget->addTab(taskTreeWidget, title);
	ui->tabWidget->setCurrentIndex(index);

	_tabsWidgetModifyMapper->setMapping(taskTreeWidget, taskTreeWidget);
	_tabsWidgetFileNameChangeMapper->setMapping(taskTreeWidget, taskTreeWidget);

	connect(taskTreeWidget, SIGNAL(taskListModified()),
	        _tabsWidgetModifyMapper, SLOT(map()));
	connect(taskTreeWidget, SIGNAL(fileNameChanged(QString)),
	        _tabsWidgetFileNameChangeMapper, SLOT(map()));

	return taskTreeWidget;
}

void MainWindow::newFile()
{
	QString taskListName = tr("(untitled)");

	createNewTaskTreeWidget(taskListName);

	status(tr("Created %1.").arg(taskListName));
}

void MainWindow::openFile()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this,
	                                                      tr("Open tasklist(s)..."),
	                                                      QString(),
	                                                      tr("Tasklist (*.yml);;Any (*.*)"));

	openFiles(fileNames);
}

void MainWindow::openFiles(const QStringList fileNames)
{
	if ( fileNames.empty() )
		return;

	int index;

	foreach(const QString &fileName, fileNames)
	{
		if ( fileName.isEmpty() )
			continue;

		QFileInfo fileInfo(fileName);

		TaskTreeWidget *taskTreeWidget = createNewTaskTreeWidget(fileInfo.baseName());

		if ( taskTreeWidget->load(fileName) )
			status(tr("Opened %1.").arg(fileName));
		else
			status(tr("Failed to open %1.").arg(fileName));
	}

	//ui->tabWidget->setCurrentIndex();
}

bool MainWindow::saveTaskList(int index, bool askForNewFileName)
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->widget(index));

	if ( !taskTreeWidget )
		return false;

	QString saveFileName;

	if ( taskTreeWidget->fileName().isEmpty() || askForNewFileName )
	{
		saveFileName = QFileDialog::getSaveFileName(this,
		                                            tr("Save tasklist"),
		                                            QString(),
		                                            tr("Tasklist (*.yml);;Any (*.*)"));
	}
	else
		saveFileName = taskTreeWidget->fileName();

	if ( taskTreeWidget->save(saveFileName) )
	{
		status(tr("Saved %1.").arg(saveFileName));
		return true;
	}
	else
	{
		status(tr("Failed to save %1.").arg(saveFileName));
		return false;
	}
}

void MainWindow::saveFile()
{
	saveTaskList(ui->tabWidget->currentIndex());
}

void MainWindow::saveAsFile()
{
	saveTaskList(ui->tabWidget->currentIndex(), true);
}

bool MainWindow::maybeSaveTab(int index)
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->widget(index));

	if ( !taskTreeWidget->isModified() )
		return true;

	QMessageBox::StandardButton ret;

	ret = QMessageBox::warning(this,
	                           tr("Tasklist have been modified"),
	                           tr("Do you want to save changes?"),
	                           QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);

	switch ( ret )
	{
		case QMessageBox::Save:
			return saveTaskList(index);
			break;

		case QMessageBox::Cancel:
			return false;
			break;

		default:
			return true;
			break;
	}
}

void MainWindow::closeTab(int index)
{
	if ( index < 0 || index >= ui->tabWidget->count() )
		return;

	if ( !maybeSaveTab(index) )
		return;

	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->widget(index));

	if ( !taskTreeWidget )
		return;

	if ( !taskTreeWidget->close() )
		return;

	_tabsWidgetModifyMapper->removeMappings(taskTreeWidget);
	_tabsWidgetFileNameChangeMapper->removeMappings(taskTreeWidget);

	disconnect(taskTreeWidget, SIGNAL(taskListModified()),
	           _tabsWidgetModifyMapper, SLOT(map()));
	disconnect(taskTreeWidget, SIGNAL(fileNameChanged(QString)),
	           _tabsWidgetFileNameChangeMapper, SLOT(map()));

	delete taskTreeWidget;
}

void MainWindow::closeCurrentTab()
{
	closeTab(ui->tabWidget->currentIndex());
}

void MainWindow::closeAllTabs()
{
	while ( ui->tabWidget->count() > 0 )
		closeTab(0);
}

void MainWindow::taskListModified(QWidget *widget)
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(widget);

	if ( !taskTreeWidget )
		return;

	QString newTabTitle = taskTreeWidget->title();

	if ( taskTreeWidget->isModified() )
		newTabTitle += "*";

	ui->tabWidget->setTabText(ui->tabWidget->indexOf(widget), newTabTitle);
}

void MainWindow::taskListFileNameChanged(QWidget *widget)
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(widget);

	if ( !taskTreeWidget )
		return;

	QFileInfo fileInfo;

	fileInfo.setFile(taskTreeWidget->fileName());
	taskTreeWidget->setTitle(fileInfo.baseName());
	taskListModified(widget);
}

bool MainWindow::maybeSave()
{
	bool maybe;

	maybe = true;
	for(int i = 0; i < ui->tabWidget->count(); i++)
		maybe = maybe && maybeSaveTab(i);

	return maybe;
}

void MainWindow::quit()
{
	_doExit = true;
	close();
}

void MainWindow::placeToTray()
{
	_windowGeometry = saveGeometry();
	_windowState = saveState();

	_trayIcon->show();
	hide();
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if ( reason != QSystemTrayIcon::DoubleClick )
		return;

	if ( isHidden() )
	{
		restoreGeometry(_windowGeometry);
		restoreState(_windowState);

		show();
		raise();
		setFocus();
		_trayIcon->hide();
	}
	else
		placeToTray();
}

void MainWindow::loadSettings()
{
	QVariantHash settings;
	QString settingsSavePath;

	settingsSavePath = QString("%1/Hussein.conf").arg(QCoreApplication::applicationDirPath());

	YamlSerialization::deserializeSettings(settingsSavePath, settings);

	if ( settings.contains(tr("geometry")) )
		restoreGeometry(QByteArray::fromHex(settings.value(tr("geometry"), QByteArray()).toByteArray()));

	if ( settings.contains(tr("state")) )
		restoreState(QByteArray::fromHex(settings.value(tr("state"), QByteArray()).toByteArray()));

	if ( settings.contains(tr("files")) )
		openFiles(settings.value(tr("files"), QStringList()).toStringList());

	if ( settings.contains(tr("current")) )
		ui->tabWidget->setCurrentIndex( settings.value(tr("current"), -1).toInt() );
}

void MainWindow::saveSettings()
{
	QVariantHash settings;

	QStringList openedFiles;

	for(int i = 0; i < ui->tabWidget->count(); i++)
	{
		TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->widget(i));

		if ( taskTreeWidget )
			openedFiles << taskTreeWidget->fileName();
	}

	settings["geometry"] = this->saveGeometry();
	settings["state"] = this->saveState();
	settings["files"] = openedFiles;
	settings["current"] = ui->tabWidget->currentIndex();

	QString settingsSavePath;

	settingsSavePath = QString("%1/Hussein.conf").arg(QCoreApplication::applicationDirPath());

	YamlSerialization::serializeSettings(settingsSavePath, settings);
}

void MainWindow::toggleTaskDone()
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->currentWidget());

	if ( !taskTreeWidget )
		return;

	taskTreeWidget->toggleDone();
}

void MainWindow::undoCurrent()
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->currentWidget());

	if ( !taskTreeWidget )
		return;

	taskTreeWidget->undo();
}

void MainWindow::redoCurrent()
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->currentWidget());

	if ( !taskTreeWidget )
		return;

	taskTreeWidget->redo();
}

void MainWindow::commitAppData(QSessionManager &manager)
{
	saveSettings();
}

void MainWindow::status(const QString &message)
{
	ui->statusbar->showMessage(message, DEFAULT_STATUS_TIME);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if ( !_doExit )
	{
		placeToTray();
		event->ignore();
	}
	else if ( maybeSave() )
	{
		saveSettings();
		event->accept();
	}
	else
		event->ignore();
}

void MainWindow::setupActions()
{
	connect(ui->actionNew, &QAction::triggered,
	        this, &MainWindow::newFile);

	connect(ui->actionOpen, &QAction::triggered,
	        this, &MainWindow::openFile);

	connect(ui->actionSave, &QAction::triggered,
	        this, &MainWindow::saveFile);

	connect(ui->actionSaveAs, &QAction::triggered,
	        this, &MainWindow::saveAsFile);

	connect(ui->actionClose, &QAction::triggered,
	        this, &MainWindow::closeCurrentTab);
	connect(ui->actionCloseAll, &QAction::triggered,
	        this, &MainWindow::closeAllTabs);

	connect(ui->actionQuit, &QAction::triggered,
	        this, &MainWindow::quit);

	// menu edit
	connect(ui->actionUndo, &QAction::triggered,
	        this, &MainWindow::undoCurrent);

	connect(ui->actionRedo, &QAction::triggered,
	        this, &MainWindow::redoCurrent);

	connect(ui->actionToggleDone, &QAction::triggered,
	        this, &MainWindow::toggleTaskDone);
}

void MainWindow::createTrayIcon()
{
	_trayIcon = new QSystemTrayIcon(this);
	_trayIcon->setIcon(QIcon(":/Hussein.ico"));

	connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	        this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}
