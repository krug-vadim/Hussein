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
		static void deserialize(BasicTask *root, const QByteArray &json);

	private:
		static void serializeTask(QJsonArray &list, BasicTask *task);
		static BasicTask *deserializeTask(const QJsonObject &object);
};

#endif // JSONSERIALIZATION_H
