#include "taskswidget.h"
#include "ui_taskswidget.h"

TasksWidget::TasksWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TasksWidget)
{
	ui->setupUi(this);
}

TasksWidget::~TasksWidget()
{
	delete ui;
}
