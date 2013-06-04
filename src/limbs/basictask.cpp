#include "basictask.h"

BasicTask::BasicTask(BasicTask *parent)
    : _parent(parent)
{
	clear();
}


BasicTask::~BasicTask()
{
	clear();
}


BasicTask *BasicTask::parent() const
{
	return _parent;
}


void BasicTask::setParent(BasicTask *parent)
{
	_parent = parent;
}

const QString &BasicTask::description() const
{
	return _description;
}

void BasicTask::setDescription(const QString &description)
{
	_description = description;
}

bool BasicTask::isDone() const
{
	return _done;
}

void BasicTask::setDone(const bool done)
{
	_done = done;
}

void BasicTask::clear()
{
	qDeleteAll(_subtasks);
	_subtasks.clear();
}

const TaskList &BasicTask::subtasks() const
{
	return _subtasks;
}

bool BasicTask::appendSubtask(BasicTask *task)
{
	if ( !task )
		return false;

	task->setParent(this);

	_subtasks.append(task);
	return true;
}

bool BasicTask::insertSubtask(BasicTask *task, int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	if ( !task )
		return false;

	task->setParent(this);

	_subtasks.insert(position, task);

	return true;
}

bool BasicTask::removeSubtask(int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	_subtasks.at(position)->setParent(0);
	_subtasks.removeAt(position);

	return true;
}

int BasicTask::row() const
{
	if ( !parent() )
		return -1;

	return parent()->subtasks().indexOf( const_cast<BasicTask *>(this) );
}




