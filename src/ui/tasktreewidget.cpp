#include "tasktreewidget.h"
#include "ui_tasktreewidget.h"

#include <QDebug>

#include <QtCore/QFileSystemWatcher>

#include <QtGui/QKeyEvent>

#include "../limbs/task.h"
#include "../models/guitaskmodel.h"
#include "../models/tasksortfilterproxymodel.h"

TaskTreeWidget::TaskTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskTreeWidget)
{
	_modified = false;
	_title = tr("(untitled)");

	ui->setupUi(this);

	_fileWatcher = new QFileSystemWatcher(this);
	connect(_fileWatcher, SIGNAL(fileChanged(QString)),
	        this, SLOT(taskListFileModified(QString)));

	_fileName = QString();

	_taskModel = new GuiTaskModel(this);
	/*connect(_taskModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
	        ui->tasksView, SLOT(expandTasks()));*/

	_taskProxyModel = new TaskSortFilterProxyModel(this);
	_taskProxyModel->setSourceModel(_taskModel);
	_taskProxyModel->setDynamicSortFilter(true);
	connect(ui->search, &QLineEdit::textChanged,
	        this, &TaskTreeWidget::setSearchString);

	ui->showDone->setChecked(_taskProxyModel->showDone());
	connect(ui->showDone, &QCheckBox::stateChanged,
	        this, &TaskTreeWidget::showDoneChanged);

	ui->tasksView->setModel(_taskProxyModel);

	ui->tasksView->setFocus();
}

TaskTreeWidget::~TaskTreeWidget()
{
	delete ui;
}

QString TaskTreeWidget::fileName() const
{
	return _fileName;
}

QString TaskTreeWidget::title() const
{
	return _title;
}

bool TaskTreeWidget::isModified() const
{
	return _modified;
}

void TaskTreeWidget::setFileName(const QString &fileName)
{
	if ( _fileName == fileName )
		return;

	if ( !_fileName.isEmpty() )
		_fileWatcher->removePath(_fileName);

	_fileName = fileName;

	if ( !_fileName.isEmpty() )
		_fileWatcher->addPath(_fileName);

	emit fileNameChanged(fileName);
}

void TaskTreeWidget::setModified(const bool modified)
{
	_modified = modified;

	emit taskListModified();
}

void TaskTreeWidget::showDoneChanged(int state)
{
	if ( state == Qt::Checked )
		_taskProxyModel->setShowDone(true);
	else
		_taskProxyModel->setShowDone(false);

	ui->tasksView->expandTasks();
}

void TaskTreeWidget::showEvent(QShowEvent * event)
{
	ui->tasksView->setFocus();
	QWidget::showEvent(event);
}

void TaskTreeWidget::keyPressEvent(QKeyEvent *event)
{
	switch ( event->key() )
	{
		case Qt::Key_Escape:
			if ( ui->tasksView->hasFocus() )
				ui->search->setFocus();
			else
				ui->tasksView->setFocus();
			break;

		default:
			QWidget::keyPressEvent(event);
			return;
	}

	event->accept();
}

bool TaskTreeWidget::load(const QString &fileName)
{
	if ( _taskModel->loadTasklist(fileName) )
	{
		setFileName(fileName);
		ui->tasksView->expandTasks();
		setModified(false);
		return true;
	}
	else
		return false;
}

bool TaskTreeWidget::save(const QString &fileName)
{
	if ( fileName.isEmpty() )
		return false;

	if ( _fileWatcher->files().contains(fileName) )
		_fileWatcher->removePath(fileName);

	bool res = _taskModel->saveTasklist(fileName);

	_fileWatcher->addPath(fileName);

	if ( !res )
		return false;

	setFileName(fileName);
	setModified(false);

	return true;
}

void TaskTreeWidget::toggleDone()
{
	ui->showDone->toggle();
}

void TaskTreeWidget::setTitle(const QString &title)
{
	_title = title;
}

void TaskTreeWidget::undo()
{
	_taskModel->undo();
}

void TaskTreeWidget::redo()
{
	_taskModel->redo();
}

void TaskTreeWidget::modifyTaskList()
{
	setModified(true);
}

void TaskTreeWidget::taskListFileModified(const QString &fileName)
{
	load(fileName);
}

void TaskTreeWidget::setSearchString(const QString &text)
{
	_taskProxyModel->setSearchString(text);

	ui->tasksView->expandTasks();
}
