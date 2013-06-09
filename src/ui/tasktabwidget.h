#ifndef TASKTABWIDGET_H
#define TASKTABWIDGET_H

#include <QtWidgets/QTabWidget>

class TaskTabWidget : public QTabWidget
{
	Q_OBJECT

	public:
		explicit TaskTabWidget(QWidget *parent = 0);

	protected:
		virtual void keyPressEvent(QKeyEvent *event);
};

#endif // TASKTABWIDGET_H
