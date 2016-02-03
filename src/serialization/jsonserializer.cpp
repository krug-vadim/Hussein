#include "jsonserializer.h"

#include <QtCore/QFile>

#include <QtCore/QHashIterator>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

#include "../limbs/task.h"

JsonSerializer::JsonSerializer()
{
}

QByteArray JsonSerializer::serialize(TaskSharedPointer root)
{
	QJsonObject out;

	if ( !root->subtasks().empty() )
	{
		QJsonArray childs;

		foreach(TaskSharedPointer subTask, root->subtasks())
		{
			QJsonObject child;
			serializeTask(child, subTask);
			childs.append(child);
		}

		out["tasks"] = childs;
	}

	return QJsonDocument(out).toJson();
}

bool JsonSerializer::serialize(const QString &fileName, TaskSharedPointer root)
{
	QFile file(fileName);

	if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
	{
		file.write(JsonSerializer::serialize(root));
		file.flush();
		file.close();
		return true;
	}
	else
		return false;
}

#include <QDebug>

#include <QDateTime>

void JsonSerializer::deserialize(const QByteArray &json, TaskSharedPointer root)
{
	qDebug() << QDateTime::currentDateTime() << "loaded done, parsing";
	QJsonDocument node = QJsonDocument::fromJson(json);
	qDebug() << QDateTime::currentDateTime() << "parsed, creating items";

	deserializeRoot(node.object(), root);

	qDebug() << QDateTime::currentDateTime() <<  "items created" << root->subtasks().size();
}


bool JsonSerializer::deserialize(const QString &fileName, TaskSharedPointer root)
{
	QFile file(fileName);

	if ( file.open(QIODevice::ReadOnly|QIODevice::Text) )
	{
		qDebug() << "deserializing";
		JsonSerializer::deserialize(file.readAll(), root);
		qDebug() << "stop deserializing";
		file.close();
		return true;
	}
	else
		return false;
}

QByteArray JsonSerializer::serializeSettings(const QVariantHash &settings)
{
	QJsonObject out;

	QHashIterator<QString, QVariant> i(settings);
	while ( i.hasNext() )
	{
		i.next();

		QVariant data = i.value();

		switch ( data.type() )
		{
			case QMetaType::QString:
				out[i.key()] = data.toString();
				break;

			case QMetaType::Int:
				out[i.key()] = data.toInt();
				break;

			case QMetaType::Bool:
				out[i.key()] = data.toBool();
				break;

			case QMetaType::QByteArray:
				out[i.key()] = QString(i.value().toByteArray().toHex());
				break;

			/*case QMetaType::QStringList:
				list = data.toStringList();

				foreach(const QString &string, list)
					out << YAML::Value << string.toStdString();

				out << YAML::EndSeq;
				break;*/

			default:
				/*out << YAML::Value << QString("(wrong type %1)").arg(i.value().type()).toStdString();*/
				break;
		}
	}

	return QJsonDocument(out).toJson();
}

bool JsonSerializer::serializeSettings(const QString &fileName, const QVariantHash &settings)
{
	QFile file(fileName);

	if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
	{
		file.write(JsonSerializer::serializeSettings(settings));
		file.flush();
		file.close();
		return true;
	}
	else
		return false;
}

void JsonSerializer::deserializeSettings(const QByteArray &json, QVariantHash &settings)
{
	QJsonObject node = QJsonDocument::fromJson(json).object();

	deserializeSettingsToHash(node, settings);
}

bool JsonSerializer::deserializeSettings(const QString &fileName, QVariantHash &settings)
{
	QFile file(fileName);

	if ( !file.exists() )
		return false;

	if ( file.open(QIODevice::ReadOnly|QIODevice::Text) )
	{
		qDebug() << "deserializing";
		JsonSerializer::deserializeSettingsToHash(QJsonDocument::fromJson(file.readAll()).object(), settings);
		qDebug() << "stop deserializing";
		file.close();
		return true;
	}
	else
		return false;

	return true;
}

void JsonSerializer::serializeTask(QJsonObject &out, TaskSharedPointer task)
{
	out["description"] = task->description();
	out["done"] = task->isDone();
	out["expanded"] = task->isExpanded();

	if ( !task->subtasks().empty() )
	{
		QJsonArray subchilds;

		foreach(TaskSharedPointer subTask, task->subtasks())
		{
			QJsonObject subchild;
			serializeTask(subchild, subTask);
			subchilds.append(subchild);
		}

		out["tasks"] = subchilds;
	}
}

void JsonSerializer::deserializeRoot(const QJsonObject &node, TaskSharedPointer root)
{
	if ( !node.contains("tasks") )
		return;

	QJsonArray tasks = node["tasks"].toArray();

	foreach(const QJsonValue &n, tasks)
	{
		TaskSharedPointer task = deserializeTask(n.toObject());
		task->setParent(root);
		root->appendSubtask(task);
	}
}

TaskSharedPointer JsonSerializer::deserializeTask(const QJsonObject &node)
{
	TaskSharedPointer task(new Task());

	if ( node.contains("description") )
		task->setDescription( node["description"].toString() );

	if ( node.contains("what") )
		task->setDescription( node["what"].toString() );

	if ( node.contains("done") )
		task->setDone( node["done"].toBool() );

	if ( node.contains("expanded") )
		task->setExpanded( node["expanded"].toBool() );

	if ( !node.contains("tasks") )
		return task;

	QJsonArray tasks = node["tasks"].toArray();

	foreach(const QJsonValue &subNode, tasks)
	{
		TaskSharedPointer subTask = deserializeTask(subNode.toObject());
		subTask->setParent(task);
		task->appendSubtask(subTask);
	}

	return task;
}

void JsonSerializer::deserializeSettingsToHash(const QJsonObject &node, QVariantHash &settings)
{
	/*node.
	for(QJsonObject::iterator it = node.begin(); it != node.end(); ++it)
	{
		if ( it->second.IsNull() )
			continue;

		if ( it->second.IsScalar() )
			settings[QString::fromStdString(it->first.as<std::string>())] = QString::fromStdString(it->second.as<std::string>());
		else if ( it->second.IsSequence() )
		{
			QStringList list;
			foreach(const YAML::Node &seq, it->second)
				list << QString::fromStdString(seq.as<std::string>());
			settings[QString::fromStdString(it->first.as<std::string>())] = list;
		}
	}*/
}
