#ifndef YAMLSERIALIZATION_H
#define YAMLSERIALIZATION_H

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "../limbs/task.h"

namespace YAML
{
	class Emitter;
	class Node;
}

class YamlSerialization
{
	public:
		YamlSerialization();

		static QByteArray serialize(TaskSharedPointer root);
		static bool serialize(const QString &fileName, TaskSharedPointer root);

		static void deserialize(const QByteArray &yaml, TaskSharedPointer root);
		static bool deserialize(const QString &fileName, TaskSharedPointer root);

		static QByteArray serializeSettings(const QVariantHash &settings);
		static bool serializeSettings(const QString &fileName, const QVariantHash &settings);

		static void deserializeSettings(const QByteArray &yaml, QVariantHash &settings);
		static bool deserializeSettings(const QString &fileName, QVariantHash &settings);

	private:
		static void serializeTask(YAML::Emitter &out, TaskSharedPointer task);
		static void deserializeRoot(const YAML::Node &node, TaskSharedPointer root);
		static TaskSharedPointer deserializeTask(const YAML::Node &node);

		static void deserializeSettingsToHash(const YAML::Node &node, QVariantHash &settings);
};

#endif // YAMLSERIALIZATION_H
