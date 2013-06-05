#ifndef TASKTREEWIDGET_H
#define TASKTREEWIDGET_H

#include <QtWidgets/QWidget>

class BasicTask;
class TaskModel;
class TaskSortFilterProxyModel;

namespace Ui
{
	class TaskTreeWidget;
}

class TaskTreeWidget : public QWidget
{
	Q_OBJECT

	public:
		explicit TaskTreeWidget(QWidget *parent = 0);
		~TaskTreeWidget();

		BasicTask *root() const;

		QString fileName() const;
		void setFileName(const QString &fileName);

	public slots:
		bool open();
		bool save();

		void showDoneChanged(int state);

	private:
		BasicTask *_rootTask;

		TaskModel *_taskModel;
		TaskSortFilterProxyModel *_taskProxyModel;

		QString _fileName;

		Ui::TaskTreeWidget *ui;
};

#endif // TASKTREEWIDGET_H
