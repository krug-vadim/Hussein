#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../limbs/basictask.h"

#include "tasktreewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	_rootTask = new BasicTask(0);
	_rootTask->setDescription(tr("(root)"));

	_rootTask->appendSubtask( new BasicTask() );
	_rootTask->subtasks().last()->setDescription(tr("123"));

	setCentralWidget( new TaskTreeWidget(_rootTask, this) );
}

MainWindow::~MainWindow()
{
	delete ui;
}
