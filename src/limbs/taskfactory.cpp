#include "taskfactory.h"

#include "basictask.h"

TaskFactory::TaskFactory(QObject *parent) :
    QObject(parent)
{
}

BasicTask *TaskFactory::create(TaskModel *model)
{
	return new BasicTask(model);
}
