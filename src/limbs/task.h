#ifndef TASK_H
#define TASK_H

#include <QtCore/QObject>
#include <QtCore/QList>

class Task;
typedef QList<Task *> TaskList;

class Task
{
	public:
		Task();
		virtual ~Task();

		Task *parent() const;

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
		void setParent(Task *parent);
		void getPath(QList<int> &path);

		Task *_parent;

		QString    _description;
		bool       _done;
		bool       _expanded;

		TaskList   _subtasks;
};

#endif // TASK_H
