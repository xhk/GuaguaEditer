#pragma once

#include <QString>
#include <QList>
#include <QVector>
#include <QSet>


class Lang {
public:
	QString name;
	QSet<QString> ext;
	QString commentLine;
	QString commentStart;
	QString commentEnd;
	QVector<QString> keywords;
	QVector<QString> types;
	
};

class LangModule
{
public:

	static LangModule& Inst() {
		static LangModule inst;
		return inst;
	}

	void Load();

	QVector<Lang> _langs;
};

