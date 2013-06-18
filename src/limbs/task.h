#ifndef TASK_H
#define TASK_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <QtCore/QWeakPointer>

class Task;

typedef QSharedPointer<Task> TaskSharedPointer;
typedef QWeakPointer<Task> TaskWeakPointer;
typedef QList<TaskSharedPointer> TaskList;

class Task
{
	public:
		enum
		{
			TaskDoneRole = Qt::UserRole,
			TaskExpandRole,
			TaskDescriptionRole,
		}
		TaskDataRole;

		Task();
		virtual ~Task();

		TaskWeakPointer parent() const;
		void setParent(const TaskWeakPointer &parent);

		const QString &description() const;
		void setDescription(const QString &description);

		bool isDone() const;
		bool isAboveDone() const;
		void setDone(const bool done);

		bool isExpanded() const;
		void setExpanded(const bool expanded);

		void clear();

		const TaskList &subtasks() const;

		bool appendSubtask(const TaskSharedPointer &task);
		bool insertSubtask(const TaskSharedPointer &task, int position);
		bool removeSubtask(int position);

		QVariant data(int role) const;
		bool setData(const QVariant &value, int role);

	private:

		//void getPath(QList<int> &path);

		TaskWeakPointer _parent;

		QString    _description;
		bool       _done;
		bool       _expanded;

		TaskList   _subtasks;
};

#endif // TASK_H
