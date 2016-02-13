import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2


TreeView {

    selectionMode: SelectionMode.ExtendedSelection;


    function addTask() {
        var indexes = selection.selectedIndexes;

        console.log(indexes);
        console.log(indexes.length);

        if ( indexes.length == 0 )
        {
            console.log("size 0");
        }
        else
        {
            for(var i = 0; i < indexes.length; i++)
            {
                console.log(model);
                model.insert(indexes[i].row + 1, indexes[i].parent);
                //model.insertRow(indexes[i].row + 1, indexes[i].parent);
            }

            /*QItemSelection selection;

            foreach (const QPersistentModelIndex &i, indexes)
                model()->insertRow(i.row() + 1, i.parent());*/

            /*QModelIndex next;

            foreach (const QPersistentModelIndex &i, indexes)
            {
                next = model()->index(i.row() + 1, i.column(), i.parent());
                selection.select(next, next);
            }

            selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
            selectionModel()->setCurrentIndex(next, QItemSelectionModel::ClearAndSelect);*/
            console.log("size != 0");
        }


            /*if ( indexes.size() == 0 )
            {
                QModelIndex index = selectionModel()->currentIndex();

                if ( !model()->insertRow(index.row() + 1, index.parent()) )
                    return;

                index = model()->index(index.row() + 1, 0, index.parent());

                selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
            }
            else
            {
                QItemSelection selection;

                foreach (const QPersistentModelIndex &i, indexes)
                    model()->insertRow(i.row() + 1, i.parent());

                QModelIndex next;

                foreach (const QPersistentModelIndex &i, indexes)
                {
                    next = model()->index(i.row() + 1, i.column(), i.parent());
                    selection.select(next, next);
                }

                selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
                selectionModel()->setCurrentIndex(next, QItemSelectionModel::ClearAndSelect);
            }*/
    }


    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Return:
            case Qt.Key_Enter:
                addTask();
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
