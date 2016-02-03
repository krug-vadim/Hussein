#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "../limbs/task.h"

class JsonSerializer
{
	public:
		JsonSerializer();

		static QByteArray serialize(TaskSharedPointer root);
		static bool serialize(const QString &fileName, TaskSharedPointer root);

		static void deserialize(const QByteArray &json, TaskSharedPointer root);
		static bool deserialize(const QString &fileName, TaskSharedPointer root);

		static QByteArray serializeSettings(const QVariantHash &settings);
		static bool serializeSettings(const QString &fileName, const QVariantHash &settings);

		static void deserializeSettings(const QByteArray &json, QVariantHash &settings);
		static bool deserializeSettings(const QString &fileName, QVariantHash &settings);

	private:
		static void serializeTask(QJsonObject &out, TaskSharedPointer task);
		static void deserializeRoot(const QJsonObject &node, TaskSharedPointer root);
		static TaskSharedPointer deserializeTask(const QJsonObject &node);

		static void deserializeSettingsToHash(const QJsonObject &node, QVariantHash &settings);

};

#endif // JSONSERIALIZER_H
