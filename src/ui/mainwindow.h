#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>

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

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void newFile();
		void openFile();
		void saveFile();
		void saveAsFile();

		bool saveTaskList(int index, bool askForNewFileName = false);

		bool maybeSaveTab(int index);

		void closeTab(int index);
		void closeCurrentTab();
		void closeAllTabs();

		void taskListModified(int index);
		void taskListFileNameChanged(int index);

		bool maybeSave();
		void quit();

		void placeToTray();
		void trayActivated(QSystemTrayIcon::ActivationReason reason);

		void loadSettings();
		void saveSettings();

	private:
		void openFiles(const QStringList fileNames);
		static const int DEFAULT_STATUS_TIME = 3000;

		void setupActions();
		void createTrayIcon();

		TaskTreeWidget *createNewTaskTreeWidget(const QString &title);

		QSignalMapper *_tabsWidgetModifyMapper;
		QSignalMapper *_tabsWidgetFileNameChangeMapper;

		QHash<int, QString> _indexToTitle;

		bool _doExit;
		QSystemTrayIcon *_trayIcon;

		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
