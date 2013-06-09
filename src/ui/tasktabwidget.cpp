#include "tasktabwidget.h"

#include <QtGui/QKeyEvent>

TaskTabWidget::TaskTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}

void TaskTabWidget::keyPressEvent(QKeyEvent *event)
{
	switch ( event->key() )
	{
		case Qt::Key_1:
		case Qt::Key_2:
		case Qt::Key_3:
		case Qt::Key_4:
		case Qt::Key_5:
		case Qt::Key_6:
		case Qt::Key_7:
		case Qt::Key_8:
		case Qt::Key_9:
			if ( event->modifiers() & Qt::ControlModifier )
			{
				int index = event->key() - Qt::Key_1;
				if ( index < count() )
					setCurrentIndex(index);
			}
			else
			{
				QTabWidget::keyPressEvent(event);
				return;
			}

		default:
			QTabWidget::keyPressEvent(event);
			return;
	}

	event->accept();
}
