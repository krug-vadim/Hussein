#include "jsonserialization.h"

#include <QtCore/QFile>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include "../limbs/basictask.h"

JsonSerialization::JsonSerialization()
{
}

QByteArray JsonSerialization::serialize(BasicTask *root)
{
	QJsonObject object;
	QJsonArray childs;

	foreach(BasicTask *task, root->subtasks())
		serializeTask(childs, task);

	object.insert(QString("tasks"), QJsonValue(childs));

	QJsonDocument jsonDocument(object);

	return jsonDocument.toJson();
}

bool JsonSerialization::serialize(const QString &fileName, BasicTask *root)
{
	QFile file(fileName);

	if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
	{
		file.write(JsonSerialization::serialize(root));
		file.flush();
		file.close();
		return true;
	}
	else
		return false;
}

void JsonSerialization::deserialize(const QByteArray &json, BasicTask *root)
{
	QJsonParseError error;

	QJsonDocument jsonDocument = QJsonDocument::fromJson(json, &error);

	if ( error.error != QJsonParseError::NoError )
		return;

	if ( jsonDocument.isEmpty() )
		return;

	if ( !jsonDocument.isObject() )
		return;

	QJsonValue childsValue = jsonDocument.object().value(QString("tasks"));

	if ( childsValue.isNull()
	     || childsValue.isUndefined()
	     || !childsValue.isArray() )
	{
		return;
	}

	QJsonArray childs = childsValue.toArray();

	foreach(QJsonValue child, childs)
		root->appendSubtask( deserializeTask(child.toObject()) );
}

bool JsonSerialization::deserialize(const QString &fileName, BasicTask *root)
{
	QFile file(fileName);

	if ( file.open(QIODevice::ReadOnly|QIODevice::Text) )
	{
		QByteArray data = file.readAll();
		JsonSerialization::deserialize(data, root);
		file.close();
		return true;
	}
	else
		return false;
}

void JsonSerialization::serializeTask(QJsonArray &list, BasicTask *task)
{
	QJsonObject object;
	QJsonArray childs;

	object.insert(QString("description"), QJsonValue(task->description()));
	object.insert(QString("done"), QJsonValue(task->isDone()));
	object.insert(QString("expanded"), QJsonValue(task->isExpanded()));

	foreach(BasicTask *subTask, task->subtasks())
		serializeTask(childs, subTask);

	if ( !childs.empty() )
		object.insert(QString("tasks"), QJsonValue(childs));

	list.append(QJsonValue(object));
}


BasicTask *JsonSerialization::deserializeTask(const QJsonObject &object)
{
	if ( object.isEmpty() )
		return 0;

	BasicTask *task = new BasicTask();

	task->setDescription(object.value(QString("description")).toString());
	task->setDone(object.value(QString("done")).toBool());
	task->setExpanded(object.value(QString("expanded")).toBool());

	QJsonValue childsValue = object.value(QString("tasks"));

	if ( childsValue.isNull()
	     || childsValue.isUndefined()
	     || !childsValue.isArray() )
	{
		return task;
	}

	QJsonArray childs = childsValue.toArray();

	foreach(QJsonValue child, childs)
		task->appendSubtask( deserializeTask(child.toObject()) );

	return task;
}
