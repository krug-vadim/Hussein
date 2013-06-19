#ifndef TASKTREEWIDGET_H
#define TASKTREEWIDGET_H

#include <QtWidgets/QWidget>

class Task;
class GuiTaskModel;
class TaskSortFilterProxyModel;

class QFileSystemWatcher;

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
		QString title() const;

		bool isModified() const;

	signals:
		void fileNameChanged(const QString &fileName);
		void taskListModified();

		void message(const QString &text);

	public slots:
		bool load(const QString &fileName = QString());
		bool save(const QString &fileName = QString());

		void toggleDone();

		void setTitle(const QString &title);

		void undo();
		void redo();

	protected:
		virtual void showEvent(QShowEvent * event);
		virtual void keyPressEvent(QKeyEvent *event);

	private slots:
		void showDoneChanged(int state);
		void modifyTaskList();
		void taskListFileModified(const QString &fileName);

		void setSearchString(const QString &text);

	private:
		void setFileName(const QString &fileName);
		void setModified(const bool modified);

		/*void createModel();
		void createProxyModel();*/

		bool _modified;

		GuiTaskModel *_taskModel;
		TaskSortFilterProxyModel *_taskProxyModel;

		QString _fileName;
		QString _title;

		QFileSystemWatcher *_fileWatcher;

		Ui::TaskTreeWidget *ui;
};

#endif // TASKTREEWIDGET_H
