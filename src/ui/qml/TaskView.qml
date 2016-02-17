import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2

TreeView {

    selectionMode: SelectionMode.ExtendedSelection;

    function addTask() {

        if ( selection.hasSelection )
        {
            var i;
            var selectedIndexes = selection.selectedIndexes;
            var selectedPersistent = [];
            var newItemSelection = model.createItemSelection();

            for(i = 0; i < selectedIndexes.length; i++)
            {
                selectedPersistent.push(model.createPersistentModelIndex(selectedIndexes[i]));
            }

            for(i = 0; i < selectedPersistent.length; i++)
            {
                model.insertRow(selectedPersistent[i].row + 1, selectedPersistent[i].parent);
            }

            for(i = 0; i < selectedPersistent.length; i++)
            {
                var next = model.index(selectedPersistent[i].row + 1,
                                   selectedPersistent[i].column,
                                   selectedPersistent[i].parent);
                model.itemSelectionSelect(newItemSelection, next, next);
            }
            selection.select(newItemSelection, ItemSelectionModel.ClearAndSelect);
            selection.setCurrentIndex(next, ItemSelectionModel.ClearAndSelect);
        }
        else
        {
            var index = selectionModel.currentIndex;

            if ( !model.insertRow(index.row + 1, index.parent) )
                return;

            index = model.index(index.row + 1, 0, index.parent);

            selectionModel.setCurrentIndex(index, ItemSelectionModel.ClearAndSelect);
        }
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
