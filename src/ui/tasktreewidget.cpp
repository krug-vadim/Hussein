#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include "../limbs/basictask.h"
#include "../limbs/taskfactory.h"
#include "../models/taskmodel.h"

#include "../serialization/jsonserialization.h"

TaskTreeWidget::TaskTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskTreeWidget)
{
	ui->setupUi(this);

	_rootTask = new BasicTask(0);
	_rootTask->setDescription(tr("(root)"));

	_fileName = QString();

	_taskModel = new TaskModel(this);
	_taskModel->setRoot(_rootTask);
	_taskModel->setTaskFactory(new TaskFactory(this));

	ui->tasksView->setModel(_taskModel);
}

TaskTreeWidget::~TaskTreeWidget()
{
	delete ui;
}

BasicTask *TaskTreeWidget::root() const
{
	return _rootTask;
}

QString TaskTreeWidget::fileName() const
{
	return _fileName;
}

void TaskTreeWidget::setFileName(const QString &fileName)
{
	_fileName = fileName;
}

void TaskTreeWidget::open()
{
	_rootTask->clear();

	if ( !fileName().isEmpty() )
		JsonSerialization::deserialize(fileName(), _rootTask);
}

void TaskTreeWidget::save()
{
	if ( !fileName().isEmpty() )
		JsonSerialization::serialize(fileName(), _rootTask);
}
