#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include "../limbs/basictask.h"
#include "../limbs/taskfactory.h"
#include "../models/taskmodel.h"
#include "../models/tasksortfilterproxymodel.h"

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

	_taskProxyModel = new TaskSortFilterProxyModel(this);
	_taskProxyModel->setSourceModel(_taskModel);
	_taskProxyModel->setDynamicSortFilter(true);

	ui->showDone->setChecked(_taskProxyModel->showDone());
	connect(ui->showDone, &QCheckBox::stateChanged,
	        this, &TaskTreeWidget::showDoneChanged);

	ui->tasksView->setModel(_taskProxyModel);
	//ui->tasksView->setModel(_taskModel);


	ui->tasksView->setFocus();
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

bool TaskTreeWidget::open()
{
	_rootTask->clear();

	if ( !fileName().isEmpty() )
	{
		bool result = JsonSerialization::deserialize(fileName(), _rootTask);
		ui->tasksView->expandTasks();
		return result;
	}
	else
		return false;
}

bool TaskTreeWidget::save()
{
	if ( !fileName().isEmpty() )
		return JsonSerialization::serialize(fileName(), _rootTask);
	else
		return false;
}

void TaskTreeWidget::showDoneChanged(int state)
{
	if ( state == Qt::Checked )
		_taskProxyModel->setShowDone(true);
	else
		_taskProxyModel->setShowDone(false);

	ui->tasksView->expandTasks();
}
