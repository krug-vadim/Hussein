#ifndef TASKTREEVIEW_H
#define TASKTREEVIEW_H

#include <QtWidgets/QTreeView>

class QKeyEvent;

class TaskTreeView : public QTreeView
{
	Q_OBJECT

	public:
		explicit TaskTreeView(QWidget *parent = 0);

	protected:
		virtual void keyPressEvent(QKeyEvent *event);

	private:
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
