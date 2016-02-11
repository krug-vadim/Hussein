import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

TreeView {
    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Return:
            case Qt.Key_Enter:
                console.log("Key ENTER");
                event.accepted = true;
                break;

            case Qt.Key_Up:
                if ( !(event.modifiers & Qt.ControlModifier) )
                    break;
                console.log("Key UP");
                event.accepted = true;
                //taskMoveUp();
                break;

            case Qt.Key_Down:
                if ( !(event.modifiers & Qt.ControlModifier) )
                    break;
                console.log("Key DOWN");
                event.accepted = true;
                //taskMoveUp();
                break;

            case Qt.Key_Tab:
                event.accepted = true;
                break;

            case Qt.Key_Backtab:
                event.accepted = true;
                break;

            case Qt.Key_Delete:
                event.accepted = true;
                break;
        }

        /*case Qt::Key_Return:
		case Qt::Key_Enter:
			if ( state() == QAbstractItemView::EditingState )
				break;
			if ( event->modifiers() & Qt::ShiftModifier )
				addSubtask();
			else if ( event->modifiers() & Qt::ControlModifier )
				toggleTaskDone();
			else
				addTask();
			break;

		case Qt::Key_Up:
			if ( event->modifiers() & Qt::ControlModifier )
				taskMoveUp();
			else
			{
				QTreeView::keyPressEvent(event);
				return;
			}
			break;

		case Qt::Key_Down:
			if ( event->modifiers() & Qt::ControlModifier )
				taskMoveDown();
			else
			{
				QTreeView::keyPressEvent(event);
				return;
			}
			break;

		case Qt::Key_Tab:
			changeCurrentToSubtask();
			break;

		case Qt::Key_Backtab:
			changeCurrentToTask();
			break;

		case Qt::Key_Delete:
			deleteTask();
			break;*/
    }
}
