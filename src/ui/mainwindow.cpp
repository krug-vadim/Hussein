#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../limbs/basictask.h"
#include "../serialization/jsonserialization.h"

#include "tasktreewidget.h"

#include <QtCore/QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	_rootTask = new BasicTask(0);
	_rootTask->setDescription(tr("(root)"));

	QFile file("test.json");

	if ( file.open(QIODevice::ReadOnly|QIODevice::Text) )
	{
		QByteArray data = file.readAll();
		JsonSerialization::deserialize(_rootTask, data);
		file.close();
	}

	setCentralWidget( new TaskTreeWidget(_rootTask, this) );
}

MainWindow::~MainWindow()
{
	QFile file("test.json");

	if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
	{
		file.write(JsonSerialization::serialize(_rootTask));
		file.flush();
		file.close();
	}

	delete ui;
}
