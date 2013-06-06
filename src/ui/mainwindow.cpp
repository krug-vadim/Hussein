#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tasktreewidget.h"

#include <QtWidgets/QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupActions();

	//setCentralWidget( new TaskTreeWidget(_rootTask, this) );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::newFile()
{
	QString taskList = tr("Tasklist %1").arg(ui->tabWidget->count() + 1);

	ui->tabWidget->addTab(new TaskTreeWidget(this), taskList);

	status(tr("Created %1.").arg(taskList));
}

void MainWindow::openFile()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this,
	                                                      tr("Open tasklist(s)..."),
	                                                      QString(),
	                                                      tr("Tasklist (*.json);;Any (*.*)"));

	if ( fileNames.empty() )
		return;

	foreach(const QString &fileName, fileNames)
	{
		QFileInfo fileInfo(fileName);

		TaskTreeWidget *taskTreeWidget = new TaskTreeWidget(this);
		taskTreeWidget->open(fileName);
		ui->tabWidget->addTab(taskTreeWidget, fileInfo.baseName());
	}
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
