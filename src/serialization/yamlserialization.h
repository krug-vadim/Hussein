#ifndef YAMLSERIALIZATION_H
#define YAMLSERIALIZATION_H

#include <QtCore/QByteArray>

class Task;
class QJsonArray;
class QJsonObject;

namespace YAML
{
	class Emitter;
};

class YamlSerialization
{
	public:
		YamlSerialization();

		static QByteArray serialize(Task *root);
		static bool serialize(const QString &fileName, Task *root);

		/*static void deserialize(const QByteArray &json, Task *root);
		static bool deserialize(const QString &fileName, Task *root);*/

	private:
		static void serializeTask(YAML::Emitter &out, Task *task);
		//static Task *deserializeTask(const QJsonObject &object);
};

#endif // YAMLSERIALIZATION_H
