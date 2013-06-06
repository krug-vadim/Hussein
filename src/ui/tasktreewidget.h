#ifndef TASKTREEWIDGET_H
#define TASKTREEWIDGET_H

#include <QtWidgets/QWidget>

class Task;
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

		Task *root() const;

		QString fileName() const;

		bool isModified() const;

	signals:
		void fileNameChanged(const QString &fileName);
		void taskListModified();

		void message(const QString &text);

	public slots:
		bool open(const QString &fileName = QString());
		bool save(const QString &fileName = QString());

		void showDoneChanged(int state);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		bool openTaskList(const QString &fileName = QString());
		bool saveTaskList(const QString &fileName = QString());

		void modifyTaskList();

	private:
		void setFileName(const QString &fileName);
		void setModified(const bool modified);

		bool maybeSave();

		Task *_rootTask;

		bool _modified;

		TaskModel *_taskModel;
		TaskSortFilterProxyModel *_taskProxyModel;

		QString _fileName;

		Ui::TaskTreeWidget *ui;
};

#endif // TASKTREEWIDGET_H
