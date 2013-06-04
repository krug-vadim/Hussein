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
		explicit TaskTreeWidget(QWidget *parent = 0);
		~TaskTreeWidget();

		BasicTask *root() const;

		QString fileName() const;
		void setFileName(const QString &fileName);

	public slots:
		void open();
		void save();

	private:
		TaskModel *_taskModel;
		BasicTask *_rootTask;

		QString _fileName;

		Ui::TaskTreeWidget *ui;
};

#endif // TASKTREEWIDGET_H
