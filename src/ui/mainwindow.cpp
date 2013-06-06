#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tasktreewidget.h"

#include <QtWidgets/QFileDialog>

#include <QtCore/QSignalMapper>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	_tabsWidgetModifyMapper = new QSignalMapper(this);
	connect(_tabsWidgetModifyMapper, SIGNAL(mapped(int)),
	        this, SLOT(taskListModified(int)));

	_tabsWidgetFileNameChangeMapper = new QSignalMapper(this);
	connect(_tabsWidgetFileNameChangeMapper, SIGNAL(mapped(int)),
	        this, SLOT(taskListFileNameChanged(int)));

	setupActions();



	//setCentralWidget( new TaskTreeWidget(_rootTask, this) );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::newFile()
{
	QString taskListName = tr("Tasklist %1").arg(ui->tabWidget->count() + 1);

	createNewTaskTreeWidget(taskListName);

	status(tr("Created %1.").arg(taskListName));
}

void MainWindow::openFile()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this,
	                                                      tr("Open tasklist(s)..."),
	                                                      QString(),
	                                                      tr("Tasklist (*.json);;Any (*.*)"));

	if ( fileNames.empty() )
		return;

	int index;

	foreach(const QString &fileName, fileNames)
	{
		QFileInfo fileInfo(fileName);

		TaskTreeWidget *taskTreeWidget = createNewTaskTreeWidget(fileInfo.baseName());
		taskTreeWidget->open(fileName);
	}

	//ui->tabWidget->setCurrentIndex();
}

void MainWindow::saveFile()
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->currentWidget());

	if ( !taskTreeWidget )
		return;

	taskTreeWidget->save();
}

void MainWindow::saveAsFile()
{
	TaskTreeWidget *taskTreeWidget = qobject_cast<TaskTreeWidget *>(ui->tabWidget->currentWidget());

	if ( !taskTreeWidget )
		return;

	QString fileName = QFileDialog::getSaveFileName(this,
	                                                tr("Save tasklist"),
	                                                QString(),
	                                                tr("Tasklist (*.json)"));

	if ( fileName.isEmpty() )
		return;

	taskTreeWidget->save(fileName);
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
}

void MainWindow::status(const QString &message)
{
	ui->statusbar->showMessage(message, DEFAULT_STATUS_TIME);
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

	connect(ui->actionQuit, &QAction::triggered,
	        this, &MainWindow::close);
}

TaskTreeWidget * MainWindow::createNewTaskTreeWidget(const QString &title)
{
	TaskTreeWidget *taskTreeWidget = new TaskTreeWidget(this);

	int index = ui->tabWidget->addTab(taskTreeWidget, title);
	ui->tabWidget->setCurrentIndex(index);

	_indexToTitle[index] = title;
	_tabsWidgetModifyMapper->setMapping(taskTreeWidget, index);
	connect(taskTreeWidget, SIGNAL(taskListModified()),
	        _tabsWidgetModifyMapper, SLOT(map()));
	connect(taskTreeWidget, SIGNAL(fileNameChanged(QString)),
	        _tabsWidgetFileNameChangeMapper, SLOT(map()));

	return taskTreeWidget;
}
