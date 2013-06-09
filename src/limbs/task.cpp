#include "task.h"

Task::Task(Task *parent)
    : _parent(parent)
{
	clear();
}

Task::~Task()
{
	clear();
}

Task *Task::parent() const
{
	return _parent;
}

void Task::setParent(Task *parent)
{
	if ( _parent )
	{
		disconnect(this, SIGNAL(subtasksChanged()),
		           _parent, SIGNAL(subtasksChanged()));
		disconnect(this, SIGNAL(dataChanged(QList<int>)),
		           _parent, SIGNAL(dataChanged(QList<int>)));
	}

	_parent = parent;

	if ( _parent )
	{
		connect(this, SIGNAL(dataChanged(QList<int>)),
		        _parent, SIGNAL(dataChanged(QList<int>)));
		connect(this, SIGNAL(subtasksChanged()),
		        _parent, SIGNAL(subtasksChanged()));

	}
}

const QString &Task::description() const
{
	return _description;
}

void Task::setDescription(const QString &description)
{
	_description = description;
	changeNotify();
}

bool Task::isDone() const
{
	return _done;
}

bool Task::isAboveDone() const
{
	if ( isDone() )
		return true;

	const Task *p = static_cast<const Task *>(this);
	while ( (p = p->parent()) )
	{
		if ( p->isDone() )
			return true;
	}

	return false;
}

void Task::setDone(const bool done)
{
	if ( _done == done )
		return;

	_done = done;
	changeNotifyRecursive();
}

bool Task::isExpanded() const
{
	return _expanded;
}

void Task::setExpanded(const bool expanded)
{
	if ( _expanded == expanded )
		return;

	_expanded = expanded;
	changeNotify();
}

void Task::clear()
{
	_done = false;
	_expanded = true;

	qDeleteAll(_subtasks);
	_subtasks.clear();
}

const TaskList &Task::subtasks() const
{
	return _subtasks;
}

bool Task::appendSubtask(Task *task)
{
	if ( !task )
		return false;

	task->setParent(this);
	_subtasks.append(task);

	emit subtasksChanged();

	return true;
}

bool Task::insertSubtask(Task *task, int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	if ( !task )
		return false;

	task->setParent(this);
	_subtasks.insert(position, task);

	emit subtasksChanged();

	return true;
}

bool Task::removeSubtask(int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	_subtasks.at(position)->setParent(0);
	_subtasks.removeAt(position);

	emit subtasksChanged();

	return true;
}

int Task::row() const
{
	if ( !parent() )
		return -1;

	return parent()->subtasks().indexOf( const_cast<Task *>(this) );
}

void Task::getPath(QList<int> &path)
{
	path.clear();

	for(Task *current = this; current->row() != -1; current = current->parent())
		path.append(current->row());
}

void Task::changeNotify()
{
	QList<int> path;

	getPath(path);

	emit dataChanged(path);
}

void Task::changeNotifyRecursive()
{
	changeNotify();

	foreach(Task *subtask, subtasks())
		subtask->changeNotifyRecursive();
}
