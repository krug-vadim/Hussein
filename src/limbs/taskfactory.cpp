#include "taskfactory.h"

#include "task.h"

TaskFactory::TaskFactory(QObject *parent) :
    QObject(parent)
{
}

Task *TaskFactory::create()
{
	return new Task();
}
