#include <QtWidgets/QApplication>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QQmlApplicationEngine engine(QUrl("../src/ui/qml/main.qml"));

	MainWindow w;
	w._engine = &engine;
	w.show();

	return app.exec();
}
