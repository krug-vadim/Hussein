#ifndef YAMLSERIALIZATION_H
#define YAMLSERIALIZATION_H

#include <QtCore/QByteArray>

class Task;
class QJsonArray;
class QJsonObject;

namespace YAML
{
	class Emitter;
	class Node;
};

class YamlSerialization
{
	public:
		YamlSerialization();

		static QByteArray serialize(Task *root);
		static bool serialize(const QString &fileName, Task *root);

		static void deserialize(const QByteArray &yaml, Task *root);
		static bool deserialize(const QString &fileName, Task *root);

	private:
		static void serializeTask(YAML::Emitter &out, Task *task);
		static void deserializeRoot(const YAML::Node &node, Task *root);
		static Task *deserializeTask(const YAML::Node &node);
};

#endif // YAMLSERIALIZATION_H
