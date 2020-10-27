#include "WorkSpace.h"
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>

WorkSpace::WorkSpace()
{

}

void WorkSpace::Load(){

    QFile jsonFile("lang.json");
    jsonFile.open(QFile::ReadOnly);
    QTextStream tsJosn(&jsonFile);
    auto strJson = tsJosn.readAll();
    QJsonDocument jd = QJsonDocument::fromJson(strJson.toUtf8());

    auto projectList = jd["projectList"].toArray();
    int nSize = projectList.size();
    for(int i=0;i<nSize;++i){
        Project prj;
        auto prjJson = projectList[i].toObject();
        prj.rootPath = prjJson["rootPath"].toString();
        this->projectList.push_back(prj);
    }

    auto openedFileList = jd["openedFileList"].toArray();
    int openedSize = openedFileList.size();
    for(int i=0;i<openedSize;++i){
        OpenedFile of;
        auto ofJosn = openedFileList[i].toObject();
        of.filePath = ofJosn["filePath"].toString();
        this->openedFileList.push_back(of);
    }

}
