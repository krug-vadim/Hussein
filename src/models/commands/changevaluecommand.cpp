#include "changevaluecommand.h"

#include <QtCore/QAbstractItemModel>

ChangeValueCommand::ChangeValueCommand(const QModelIndex &index, const QVariant &value, const int role, QAbstractItemModel *model)
    : QUndoCommand()
{
	_oldValue = index.data(Qt::DisplayRole);
	_newValue = value;

	_row = index.row();
	_col = index.column();

	_role = role;

	_model = model;

	//setText()

	/*setText

	setText(QApplication::translate("ChangeValueCommand",
	                                "Set (%1,%2) to %3").arg(m_col+1)
			.arg(m_row+1).arg(m_new.toInt()));*/
}

void ChangeValueCommand::redo()
{
	/*QModelIndex index = _model->index(_row, _col);
	m_model->m_data[m_col][m_row] = m_new.toInt();
	m_model->emitDataChanged(index);*/
}

void ChangeValueCommand::undo()
{
}
