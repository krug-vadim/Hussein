#include "taskmimedata.h"

TaskMimeData::TaskMimeData()
{
}

bool TaskMimeData::hasFormat(const QString &mimeType) const
{
	return ( mimeType == tr("Hussein/Task") );
}

QStringList TaskMimeData::formats() const
{
	QStringList formats;

	formats << tr("Hussein/Task");

	return formats;
}

QVariant TaskMimeData::retrieveData(const QString &mimeType, QVariant::Type type) const
{
}
