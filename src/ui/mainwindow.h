#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

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

	private slots:
		void newFile();
		void openFile();
		void saveFile();
		void saveAsFile();

		//
		void status(const QString &message);

	private:
		static const int DEFAULT_STATUS_TIME = 3000;

		void setupActions();

		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
