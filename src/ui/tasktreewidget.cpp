#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

TaskTreeWidget::TaskTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskTreeWidget)
{
	ui->setupUi(this);
}

TaskTreeWidget::~TaskTreeWidget()
{
	delete ui;
}
