#ifndef YAMLSERIALIZATION_H
#define YAMLSERIALIZATION_H

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QVariant>

class Task;

namespace YAML
{
	class Emitter;
	class Node;
}

class YamlSerialization
{
	public:
		YamlSerialization();

		static QByteArray serialize(Task *root);
		static bool serialize(const QString &fileName, Task *root);

		static void deserialize(const QByteArray &yaml, Task *root);
		static bool deserialize(const QString &fileName, Task *root);

		static QByteArray serializeSettings(const QVariantHash &settings);
		static bool serializeSettings(const QString &fileName, const QVariantHash &settings);

		static void deserializeSettings(const QByteArray &yaml, QVariantHash &settings);
		static bool deserializeSettings(const QString &fileName, QVariantHash &settings);

	private:
		static void serializeTask(YAML::Emitter &out, Task *task);
		static void deserializeRoot(const YAML::Node &node, Task *root);
		static Task *deserializeTask(const YAML::Node &node);

		static void deserializeSettingsToHash(const YAML::Node &node, QVariantHash &settings);
};

#endif // YAMLSERIALIZATION_H