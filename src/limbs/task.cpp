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
	_parent = parent;
}

const QString &Task::description() const
{
	return _description;
}

void Task::setDescription(const QString &description)
{
	_description = description;
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
	_done = done;
}

bool Task::isExpanded() const
{
	return _expanded;
}

void Task::setExpanded(const bool expanded)
{
	_expanded = expanded;
}

void Task::clear()
{
	_done = false;
	_expanded = false;

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

	return true;
}

bool Task::removeSubtask(int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	_subtasks.at(position)->setParent(0);
	_subtasks.removeAt(position);

	return true;
}

int Task::row() const
{
	if ( !parent() )
		return -1;

	return parent()->subtasks().indexOf( const_cast<Task *>(this) );
}
