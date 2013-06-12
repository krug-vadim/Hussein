#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tasktreewidget.h"

#include <QtCore/QSignalMapper>

#include <QtGui/QCloseEvent>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	_doExit = false;

	ui->setupUi(this);

	createTrayIcon();

	_tabsWidgetModifyMapper = new QSignalMapper(this);
	connect(_tabsWidgetModifyMapper, SIGNAL(mapped(int)),
	        this, SLOT(taskListModified(int)));

	_tabsWidgetFileNameChangeMapper = new QSignalMapper(this);
	connect(_tabsWidgetFileNameChangeMapper, SIGNAL(mapped(int)),
	        this, SLOT(taskListFileNameChanged(int)));

	connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
	        this, SLOT(closeTab(int)));

	setupActions();

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

	_indexToTitle[index] = title;

	_tabsWidgetModifyMapper->setMapping(taskTreeWidget, index);
	_tabsWidgetFileNameChangeMapper->setMapping(taskTreeWidget, index);

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

	if ( fileNames.empty() )
		return;

	int index;

	foreach(const QString &fileName, fileNames)
	{
		QFileInfo fileInfo(fileName);

		TaskTreeWidget *taskTreeWidget = createNewTaskTreeWidget(fileInfo.baseName());

		if ( taskTreeWidget->open(fileName) )
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

	ui->tabWidget->removeTab(index);
	delete taskTreeWidget;
}

void MainWindow::closeCurrentTab()
{
	closeTab(ui->tabWidget->currentIndex());
}

void MainWindow::closeAllTabs()
{
	for(int i = 0; i < ui->tabWidget->count(); i++)
		closeTab(i);
}

void MainWindow::taskListModified(int index)
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->widget(index));

	if ( !taskTreeWidget )
		return;

	QString newTabTitle = _indexToTitle.value(index);

	if ( taskTreeWidget->isModified() )
		newTabTitle += "*";

	ui->tabWidget->setTabText(index, newTabTitle);
}

void MainWindow::taskListFileNameChanged(int index)
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->widget(index));

	if ( !taskTreeWidget )
		return;

	QFileInfo fileInfo;

	fileInfo.setFile(taskTreeWidget->fileName());

	_indexToTitle[index] = fileInfo.baseName();
	taskListModified(index);
}

bool MainWindow::maybeSave()
{
	return false;
}

void MainWindow::quit()
{
	_doExit = true;
	close();
}

void MainWindow::placeToTray()
{
	_trayIcon->show();
	hide();
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if ( reason != QSystemTrayIcon::DoubleClick )
		return;

	if ( isHidden() )
	{
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
}

void MainWindow::saveSettings()
{
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
	        this, &MainWindow::close);
}

void MainWindow::createTrayIcon()
{
	_trayIcon = new QSystemTrayIcon(this);
	_trayIcon->setIcon(QIcon(":/Hussein.ico"));

	connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	        this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}
