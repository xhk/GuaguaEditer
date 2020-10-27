#ifndef WorkSpace_H
#define WorkSpace_H

#include <QVector>

class Project{
public:
    QString rootPath;
    QVector<QString> expandPathList;
};

class OpenedFile{
public:
    QString filePath;
};

class WorkSpace
{
public:
    WorkSpace();
    static WorkSpace & Inst(){
        static WorkSpace inst;
        return inst;
    }

    void Load();

    void Save();

    QVector<Project> projectList;
    QVector<OpenedFile> openedFileList;

};

#endif // WorkSpace_H
