#ifndef BASICTASK_H
#define BASICTASK_H

#include <QtCore/QObject>
#include <QtCore/QList>

class BasicTask;
typedef QList<BasicTask *> TaskList;

class BasicTask : public QObject
{
	Q_OBJECT

	public:
		BasicTask(BasicTask *parent = 0);
		virtual ~BasicTask();

		BasicTask *parent() const;
		void setParent(BasicTask *parent);

		const QString &description() const;
		void setDescription(const QString &description);

		bool isDone() const;
		void setDone(const bool done);

		bool isCollapsed() const;
		void setCollapsed(const bool collapsed);

		void clear();

		const TaskList &subtasks() const;

		bool appendSubtask(BasicTask *task);
		bool insertSubtask(BasicTask *task, int position);
		bool removeSubtask(int position);

		virtual int row() const;

	private:
		BasicTask *_parent;

		QString    _description;
		bool       _done;
		bool       _collapsed;

		TaskList   _subtasks;
};

#endif // BASICTASK_H
