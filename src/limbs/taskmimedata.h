#ifndef TASKMIMEDATA_H
#define TASKMIMEDATA_H

#include <QtCore/QMimeData>
#include <QtCore/QStringList>

class TaskMimeData : public QMimeData
{
		Q_OBJECT
	public:
		explicit TaskMimeData();

		virtual bool hasFormat(const QString & mimeType) const;
		virtual QStringList formats() const;

	protected:
		virtual QVariant retrieveData(const QString & mimeType, QVariant::Type type) const;
};

#endif // TASKMIMEDATA_H
