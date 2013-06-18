#ifndef GUITASKMODEL_H
#define GUITASKMODEL_H

#include "taskmodel.h"

class GuiTaskModel : public TaskModel
{
	Q_OBJECT

	public:
		explicit GuiTaskModel(QObject *parent = 0);

		QVariant data(const QModelIndex &index, int role) const;
};

#endif // GUITASKMODEL_H
