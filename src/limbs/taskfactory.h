#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QtCore/QObject>

class BasicTask;

class TaskFactory : public QObject
{
	Q_OBJECT

	public:
		explicit TaskFactory(QObject *parent = 0);

		BasicTask *create(TaskModel *model);
};

#endif // TASKFACTORY_H
