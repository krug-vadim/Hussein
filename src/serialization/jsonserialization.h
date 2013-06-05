#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include <QtCore/QByteArray>

class Task;
class QJsonArray;
class QJsonObject;

class JsonSerialization
{
	public:
		explicit JsonSerialization();

		static QByteArray serialize(Task *root);
		static bool serialize(const QString &fileName, Task *root);

		static void deserialize(const QByteArray &json, Task *root);
		static bool deserialize(const QString &fileName, Task *root);

	private:
		static void serializeTask(QJsonArray &list, Task *task);
		static Task *deserializeTask(const QJsonObject &object);
};

#endif // JSONSERIALIZATION_H
