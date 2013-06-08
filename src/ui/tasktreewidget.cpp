#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include <QtGui/QCloseEvent>

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

	ui->setupUi(this);

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

	connect(_taskModel, SIGNAL(layoutChanged()),
	        this, SLOT(modifyTaskList()));
	connect(_taskModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
	        this, SLOT(modifyTaskList()));
	connect(_taskModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
	        this, SLOT(modifyTaskList()));
	connect(_taskModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
	        this, SLOT(modifyTaskList()));
	connect(_taskModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
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
	_fileName = fileName;
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

void TaskTreeWidget::closeEvent(QCloseEvent *event)
{
	if ( maybeSave() )
	{
		event->accept();
	}
	else
		event->ignore();
}

bool TaskTreeWidget::maybeSave()
{
	if ( !isModified() )
		return true;

	QMessageBox::StandardButton ret;

	ret = QMessageBox::warning(this,
	                           tr("Tasklist have been modified"),
	                           tr("Do you want to save changes?"),
	                           QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);

	switch ( ret )
	{
		case QMessageBox::Save:
			return save();
			break;

		case QMessageBox::Cancel:
			return false;
			break;

		default:
			return true;
			break;
	}
}

bool TaskTreeWidget::open(const QString &fileName)
{
	QString openFileName;

	openFileName = (fileName.isEmpty()) ? this->fileName() : fileName;

	if ( openFileName.isEmpty() )
	{
		openFileName = QFileDialog::getOpenFileName(this,
		                                            tr("Open tasklist..."),
		                                            QString(),
		                                            tr("Tasklist (*.json);;Any (*.*)"));
	}

	_taskModel->tasksAboutToBeReseted();
	if ( openTaskList(openFileName) )
	{
		setFileName(openFileName);
		_taskModel->tasksReseted();
		ui->tasksView->expandTasks();
		setModified(false);
		emit message(tr("Opened %1.").arg(openFileName));

		return true;
	}
	else
	{
		_taskModel->tasksReseted();
		emit message(tr("Failed to open %1.").arg(openFileName));
		return false;
	}
}

bool TaskTreeWidget::save(const QString &fileName)
{
	QString saveFileName;

	saveFileName = (fileName.isEmpty()) ? this->fileName() : fileName;

	if ( saveFileName.isEmpty() )
	{
		saveFileName = QFileDialog::getSaveFileName(this,
		                                            tr("Save tasklist"),
		                                            QString(),
		                                            tr("Tasklist (*.json)"));
	}

	if ( saveTaskList(saveFileName) )
	{
		setFileName(saveFileName);
		setModified(false);
		emit message(tr("Saved %1.").arg(saveFileName));

		return true;
	}
	else
	{
		emit message(tr("Failed to save %1.").arg(saveFileName));

		return false;
	}
}

bool TaskTreeWidget::openTaskList(const QString &fileName)
{
	_rootTask->clear();

	if ( !fileName.isEmpty() )
		return JsonSerialization::deserialize(fileName, _rootTask);
	else
		return false;
}

bool TaskTreeWidget::saveTaskList(const QString &fileName)
{
	if ( !fileName.isEmpty() )
		//return JsonSerialization::serialize(fileName, _rootTask);
		return YamlSerialization::serialize(fileName, _rootTask);
	else
		return false;
}

void TaskTreeWidget::modifyTaskList()
{
	setModified(true);
}
