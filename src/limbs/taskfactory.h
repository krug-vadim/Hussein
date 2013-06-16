#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include <QtCore/QObject>

class Task;

class TaskFactory : public QObject
{
	Q_OBJECT

	public:
		explicit TaskFactory(QObject *parent = 0);

		Task *create();
};

#endif // TASKFACTORY_H
