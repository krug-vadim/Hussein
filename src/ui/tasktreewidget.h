#ifndef TASKTREEWIDGET_H
#define TASKTREEWIDGET_H

#include <QtWidgets/QWidget>

class BasicTask;
class TaskModel;

namespace Ui
{
	class TaskTreeWidget;
}

class TaskTreeWidget : public QWidget
{
	Q_OBJECT

	public:
		explicit TaskTreeWidget(BasicTask *root, QWidget *parent = 0);
		~TaskTreeWidget();

	private:
		TaskModel *_taskModel;

		Ui::TaskTreeWidget *ui;
};

#endif // TASKTREEWIDGET_H
