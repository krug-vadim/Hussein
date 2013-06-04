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

	private:
		void setupActions();

		Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
