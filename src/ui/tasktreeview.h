#ifndef TASKTREEVIEW_H
#define TASKTREEVIEW_H

#include <QtWidgets/QTreeView>

class QKeyEvent;

class TaskTreeView : public QTreeView
{
	Q_OBJECT

	public:
		explicit TaskTreeView(QWidget *parent = 0);

		void setModel(QAbstractItemModel *model);

	public slots:
		void expandTasks();

	protected:
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void paintEvent(QPaintEvent *event);

	private slots:
		void taskCollapsed(const QModelIndex &index);
		void taskExpanded(const QModelIndex &index);

		void expandTask(const QModelIndex &index);

		void layoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);

	private:
		int unSelectedRowBefore(const QModelIndex &index);
		int unSelectedRowAfter(const QModelIndex &index);

		void addTask();
		void addSubtask();
		void deleteTask();

		void taskMoveUp();
		void taskMoveDown();

		void changeCurrentToSubtask();
		void changeCurrentToTask();

		void toggleTaskDone();
};

#endif // TASKTREEVIEW_H
