#include "taskfactory.h"

#include "basictask.h"

TaskFactory::TaskFactory(QObject *parent) :
    QObject(parent)
{
}

BasicTask *TaskFactory::create(BasicTask *parent)
{
	return new BasicTask(parent);
}
