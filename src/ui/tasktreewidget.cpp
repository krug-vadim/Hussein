#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include <QDebug>

#include <QtCore/QFileSystemWatcher>

#include "../limbs/task.h"
#include "../limbs/taskfactory.h"
#include "../models/taskmodel.h"
#include "../models/tasksortfilterproxymodel.h"

#include "../serialization/jsonserialization.h"
#include "../serialization/yamlserialization.h"

TaskTreeWidget::TaskTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskTreeWidget)
{
	_modified = false;
	_saved = false;

	ui->setupUi(this);

	_fileWatcher = new QFileSystemWatcher(this);
	connect(_fileWatcher, SIGNAL(fileChanged(QString)),
	        this, SLOT(taskListFileModified(QString)));

	_rootTask = new Task(0);
	_rootTask->setDescription(tr("(root)"));

	/*connect(_rootTask, SIGNAL(changed(QList<int>)),
	        this, SIGNAL(taskListModified()));*/

	_fileName = QString();

	_taskModel = new TaskModel(this);
	_taskModel->setRoot(_rootTask);
	_taskModel->setTaskFactory(new TaskFactory(this));

	connect(_rootTask, SIGNAL(dataChanged(QList<int>)),
	        _taskModel, SLOT(taskDataChanged(QList<int>)));

	connect(_rootTask, SIGNAL(dataChanged(QList<int>)),
	        this, SLOT(modifyTaskList()));
	connect(_rootTask, SIGNAL(subtasksChanged()),
	        this, SLOT(modifyTaskList()));

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

Task *TaskTreeWidget::root() const
{
	return _rootTask;
}

QString TaskTreeWidget::fileName() const
{
	return _fileName;
}

bool TaskTreeWidget::isModified() const
{
	return _modified;
}

void TaskTreeWidget::setFileName(const QString &fileName)
{
	if ( _fileName == fileName )
		return;

	if ( !_fileName.isEmpty() )
		_fileWatcher->removePath(_fileName);

	_fileName = fileName;

	if ( !_fileName.isEmpty() )
		_fileWatcher->addPath(_fileName);

	emit fileNameChanged(fileName);
}

void TaskTreeWidget::setModified(const bool modified)
{
	_modified = modified;

	emit taskListModified();
}

void TaskTreeWidget::showDoneChanged(int state)
{
	if ( state == Qt::Checked )
		_taskProxyModel->setShowDone(true);
	else
		_taskProxyModel->setShowDone(false);

	ui->tasksView->expandTasks();
}

void TaskTreeWidget::showEvent(QShowEvent * event)
{
	ui->tasksView->setFocus();
	QWidget::showEvent(event);
}

bool TaskTreeWidget::open(const QString &fileName)
{
	_rootTask->clear();

	if ( fileName.isEmpty() )
		return false;

	_taskModel->tasksAboutToBeReseted();
	bool res = YamlSerialization::deserialize(fileName, _rootTask);
	_taskModel->tasksReseted();

	if ( !res )
		return false;

	setFileName(fileName);
	ui->tasksView->expandTasks();
	setModified(false);

	return true;
}

bool TaskTreeWidget::save(const QString &fileName)
{
	if ( fileName.isEmpty() )
		return false;

	_saved = true;

	if ( !YamlSerialization::serialize(fileName, _rootTask) )
		return false;

	setFileName(fileName);
	setModified(false);

	return true;
}

void TaskTreeWidget::modifyTaskList()
{
	setModified(true);
}

void TaskTreeWidget::taskListFileModified(const QString &fileName)
{
	if ( _saved )
		_saved = false;
	else
		open(fileName);
}
