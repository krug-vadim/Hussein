#include "yamlserialization.h"

#include <yaml-cpp/yaml.h>

#include <QtCore/QFile>

#include "../limbs/task.h"


YamlSerialization::YamlSerialization()
{
}

QByteArray YamlSerialization::serialize(Task *root)
{
	YAML::Emitter out;

	out << YAML::BeginMap;

	if ( !root->subtasks().empty() )
	{
		out << YAML::Key << "tasks";
		out << YAML::Value << YAML::BeginSeq;

		foreach(Task *subTask, root->subtasks())
			serializeTask(out, subTask);

		out << YAML::EndSeq;
	}

	out << YAML::EndMap;

	return QByteArray(out.c_str());
}

bool YamlSerialization::serialize(const QString &fileName, Task *root)
{
	QFile file(fileName);

	if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
	{
		file.write(YamlSerialization::serialize(root));
		file.flush();
		file.close();
		return true;
	}
	else
		return false;
}

void YamlSerialization::serializeTask(YAML::Emitter &out, Task *task)
{
//	object.insert(QString("description"), QJsonValue());
//	object.insert(QString("done"), QJsonValue(task->isDone()));
//	object.insert(QString("expanded"), QJsonValue(task->isExpanded()));

	out << YAML::BeginMap;
	out << YAML::Key << "description";
	out << YAML::Value << task->description().toStdString();

	if ( !task->subtasks().empty() )
	{
		out << YAML::Key << "tasks";
		out << YAML::Value << YAML::BeginSeq;

		foreach(Task *subTask, task->subtasks())
			serializeTask(out, subTask);

		out << YAML::EndSeq;
	}

	out << YAML::EndMap;
}
