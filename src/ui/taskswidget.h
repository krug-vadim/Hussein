#ifndef TASKSWIDGET_H
#define TASKSWIDGET_H

#include <QWidget>

namespace Ui {
	class TasksWidget;
}

class TasksWidget : public QWidget
{
		Q_OBJECT
		
	public:
		explicit TasksWidget(QWidget *parent = 0);
		~TasksWidget();
		
	private:
		Ui::TasksWidget *ui;
};

#endif // TASKSWIDGET_H
