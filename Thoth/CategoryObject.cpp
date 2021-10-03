
/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtDebug>

#include "CategoryObject.h"

// Serialization
QDataStream & operator<<(QDataStream &out, const Category &cat)
{
	out << cat.id;
	out << cat.name;
	out << cat.desc;
	out << cat.isHidden;

	return out;
}

// Deserialization
QDataStream & operator>>(QDataStream &in, Category &cat)
{
	in >> cat.id;
	in >> cat.name;
	in >> cat.desc;
	in >> cat.isHidden;

	return in;
}


CategoryObject::CategoryObject(QObject *parent)
	: QObject(parent)
{
}

CategoryObject::~CategoryObject()
{
	if (_changed) save();
}

void CategoryObject::setId(int id, const QString & name)
{
	_id = id;
	_name = name;
}

int CategoryObject::id() const
{
	return _id;
}

QString CategoryObject::name() const
{
	return _name;
}

Category CategoryObject::category(int id) const
{
	return _catMap.value(id, Category());
}

Category CategoryObject::add(const QString & name, const QString & desc)
{
	Category cat;
	cat.id = _nextChildId++;
	cat.name = name;
	cat.desc = desc;

	_catMap[cat.id] = cat;
	_changed = true;

	return cat;
}

const QList<Category> CategoryObject::allCategories() const
{
	 return _catMap.values();
}

bool CategoryObject::open(const QString & classJsonPath)
{
	QFile f(classJsonPath);

	if (!f.open(QFile::ReadOnly | QFile::Text))
	{
		qWarning() << "Fail to open the Class JSON file : " << classJsonPath;
		return false;
	}

	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(f.readAll(), &err);
	f.close();

	if (json.isNull())
	{
		qWarning() << err.errorString();
		return false;
	}

	QJsonObject  obj = json.object();

	if (!obj.contains("classes"))
	{
		qWarning() << "No Kari Object Class file : " << classJsonPath;
		return false;
	}

	_catMap.clear();

	QJsonArray classes = obj.value("classes").toArray();
	_nextChildId = 0;

	for (int i = 0; i < classes.count(); i++)
	{
		Category cat;
		QJsonObject obj = classes.at(i).toObject();
		cat.id = obj.value("classId").toInt();
		cat.name = obj.value("name").toString();
		cat.desc = obj.value("desc").toString();
		cat.isHidden = obj.value("isHidden").toBool();


		_catMap[cat.id] = cat;

		if (_nextChildId < cat.id) _nextChildId = cat.id;
	}

	_nextChildId++;
	_changed = false;

	_filePath = classJsonPath;
	return true;
}

bool CategoryObject::save(const QString & classJsonPath)
{
	QString path = classJsonPath.isEmpty() ? _filePath : classJsonPath;

	if (path.isEmpty()) return false;

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) return false;

	QDataStream out(&file);

	out << _catMap;

	file.close();
	_filePath = path;
	_changed = false;

	return true;
}
