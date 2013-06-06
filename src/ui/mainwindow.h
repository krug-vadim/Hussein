#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class QSignalMapper;
class TaskTreeWidget;

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:
		void status(const QString &message);

	private slots:
		void newFile();
		void openFile();
		void saveFile();
		void saveAsFile();

		void taskListModified(int index);
		void taskListFileNameChanged(int index);

		//
		//void placeToTray();

	private:
		static const int DEFAULT_STATUS_TIME = 3000;

		void setupActions();

		TaskTreeWidget *createNewTaskTreeWidget(const QString &title);

		QSignalMapper *_tabsWidgetModifyMapper;
		QSignalMapper *_tabsWidgetFileNameChangeMapper;

		QHash<int, QString> _indexToTitle;

		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
