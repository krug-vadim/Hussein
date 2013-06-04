#ifndef TASKTREEWIDGET_H
#define TASKTREEWIDGET_H

#include <QWidget>

namespace Ui {
	class TaskTreeWidget;
}

class TaskTreeWidget : public QWidget
{
		Q_OBJECT
		
	public:
		explicit TaskTreeWidget(QWidget *parent = 0);
		~TaskTreeWidget();
		
	private:
		Ui::TaskTreeWidget *ui;
};

#endif // TASKTREEWIDGET_H
