#ifndef TASK_H
#define TASK_H

#include <QtCore/QObject>
#include <QtCore/QList>

class Task;
typedef QList<Task *> TaskList;

class Task : public QObject
{
	Q_OBJECT

	public:
		Task(Task *parent = 0);
		virtual ~Task();

		Task *parent() const;
		void setParent(Task *parent);

		const QString &description() const;
		void setDescription(const QString &description);

		bool isDone() const;
		bool isAboveDone() const;
		void setDone(const bool done);

		bool isExpanded() const;
		void setExpanded(const bool expanded);

		void clear();

		const TaskList &subtasks() const;

		bool appendSubtask(Task *task);
		bool insertSubtask(Task *task, int position);
		bool removeSubtask(int position);

		int row() const;

	private:
		Task *_parent;

		QString    _description;
		bool       _done;
		bool       _expanded;

		TaskList   _subtasks;
};

#endif // TASK_H
