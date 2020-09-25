#include "LangModule.h"
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>

void LangModule::Load() 
{
	QFile jsonFile("lang.json");
	jsonFile.open(QFile::ReadOnly);
	QTextStream tsJosn(&jsonFile);
	auto strJson = tsJosn.readAll();
	QJsonDocument jd = QJsonDocument::fromJson(strJson.toUtf8());
	
	auto langs = jd["langs"].toArray();
	int nSize = langs.size();
	for (int i = 0; i < nSize; ++i) {
		Lang l;
		auto lang = langs[i].toObject();
		if (lang.contains("name")) {
			l.name = lang.value("name").toString();
		}
		if (lang.contains("ext")) {
			auto kwArr = lang["ext"].toArray();
			for (auto kw : kwArr) {
				l.ext.insert(kw.toString());
			}
		}
		if (lang.contains("commentLine")) {
			l.commentLine = lang.value("commentLine").toString();
		}
		if (lang.contains("commentStart")) {
			l.commentStart = lang.value("commentStart").toString();
		}
		if (lang.contains("commentEnd")) {
			l.commentEnd = lang.value("commentEnd").toString();
		}

		if (lang.contains("keywords")) {
			auto kwArr = lang["keywords"].toArray();
			for (auto kw : kwArr) {
				l.keywords.push_back(kw.toString());
			}
		}

		if (lang.contains("types")) {
			auto kwArr = lang["types"].toArray();
			for (auto kw : kwArr) {
				l.types.push_back(kw.toString());
			}
		}

		_langs.push_back(l);
	}
}