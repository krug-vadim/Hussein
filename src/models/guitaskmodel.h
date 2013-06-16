#ifndef GUITASKMODEL_H
#define GUITASKMODEL_H

#include "taskmodel.h"

class GuiTaskModel : public TaskModel
{
	Q_OBJECT

	public:
		explicit GuiTaskModel(QObject *parent = 0);

	signals:

	public slots:

};

#endif // GUITASKMODEL_H
