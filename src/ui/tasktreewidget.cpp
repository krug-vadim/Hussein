#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include "../limbs/taskfactory.h"
#include "../models/taskmodel.h"

TaskTreeWidget::TaskTreeWidget(BasicTask *root, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskTreeWidget)
{
	ui->setupUi(this);

	_taskModel = new TaskModel(this);
	_taskModel->setRoot(root);
	_taskModel->setTaskFactory(new TaskFactory(this));

	ui->tasksView->setModel(_taskModel);
}

TaskTreeWidget::~TaskTreeWidget()
{
	delete ui;
}
