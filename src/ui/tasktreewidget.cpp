#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include <QDebug>

#include <QtCore/QFileSystemWatcher>

#include <QtGui/QKeyEvent>

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
	_title = tr("(untitled)");

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
	connect(ui->search, &QLineEdit::textChanged,
	        this, &TaskTreeWidget::setSearchString);

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

QString TaskTreeWidget::title() const
{
	return _title;
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

void TaskTreeWidget::keyPressEvent(QKeyEvent *event)
{
	switch ( event->key() )
	{
		case Qt::Key_Escape:
			if ( ui->tasksView->hasFocus() )
				ui->search->setFocus();
			else
				ui->tasksView->setFocus();
			break;

		default:
			QWidget::keyPressEvent(event);
			return;
	}

	event->accept();
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

	if ( _fileWatcher->files().contains(fileName) )
		_fileWatcher->removePath(fileName);

	_saved = true;

	bool res = YamlSerialization::serialize(fileName, _rootTask);

	_fileWatcher->addPath(fileName);

	if ( !res )
		return false;

	setFileName(fileName);
	setModified(false);

	return true;
}

void TaskTreeWidget::toggleDone()
{
	ui->showDone->toggle();
}

void TaskTreeWidget::setTitle(const QString &title)
{
	_title = title;
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

void TaskTreeWidget::setSearchString(const QString &text)
{
	_taskProxyModel->setSearchString(text);

	ui->tasksView->expandTasks();
}
