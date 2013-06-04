#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include <QtCore/QByteArray>

class BasicTask;
class QJsonArray;
class QJsonObject;

class JsonSerialization
{
	public:
		explicit JsonSerialization();

		static QByteArray serialize(BasicTask *root);
		static bool serialize(const QString &fileName, BasicTask *root);

		static void deserialize(const QByteArray &json, BasicTask *root);
		static bool deserialize(const QString &fileName, BasicTask *root);

	private:
		static void serializeTask(QJsonArray &list, BasicTask *task);
		static BasicTask *deserializeTask(const QJsonObject &object);
};

#endif // JSONSERIALIZATION_H
