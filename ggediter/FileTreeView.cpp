#include "FileTreeView.h"

#include <QDir>
#include <QDebug>

FileTreeView::FileTreeView(QWidget *parent) : QTreeView(parent)
{
    _model = nullptr;
    connect(this,SIGNAL(clicked(const QModelIndex &)), this, SLOT(OnClicked(const QModelIndex&)));
    connect(this,SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OnDoubleClicked(const QModelIndex&)));

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}


QIcon FileIcon(const QString & ext_){
    auto ext = ext_.toLower();
    QString iconName;
    if(ext == "c"){
        iconName = "c";
    }
    else if(ext == "cc" || ext == "cxx" || ext == "cpp"){
        iconName = "cpp";
    }
    else if(ext == "cs"){
        iconName = "csharp";
    }else if(ext == "py"){
        iconName = "python";
    }
    else{
        iconName = "default";
    }

    QString resPath = QString(":res/FileIco/%1").arg(ext);
    return QIcon(resPath);
}

void FileTreeView::Load(const QString & rootDir)
{

    QDir dir(rootDir);

    if (!dir.exists())
       return ;

   dir.setFilter(QDir::Dirs|QDir::Files);
   dir.setSorting(QDir::DirsFirst);
   QFileInfoList list = dir.entryInfoList();
   _model = new QStandardItemModel(this);
   //_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("序号") << QStringLiteral("名称"));     //设置列头
    _model->setHorizontalHeaderLabels(QStringList()<< QStringLiteral("名称"));     //设置列头
   for (int i=0; i<list.size(); ++i ) {
       auto fi = list[i];
       auto fileName = fi.fileName();
       QIcon icon;
       if(fi.isDir()){
           icon = QIcon(":/res/folder_32px.png");
       }else{
           auto dotPos = fileName.lastIndexOf(".") ;
           if(dotPos==-1){
                icon = FileIcon("");
           }else{
               auto ext = fileName.mid(dotPos+1);
               icon = FileIcon(ext);
           }
       }
       auto item = new QStandardItem(icon,fileName);
       item->setData(QVariant::fromValue(fi));
       _model->appendRow(item);

   }
    this->setModel(_model);

}

void FileTreeView::OnClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    auto fi = item->data().value<QFileInfo>();
    auto path = fi.absoluteFilePath();
    qDebug()<<"FileTreeView::OnClicked";
    emit ClickFile(path);
}
void FileTreeView::OnDoubleClicked(const QModelIndex & index)
{
    auto item = _model->itemFromIndex(index);
    auto fi = item->data().value<QFileInfo>();
    auto path = fi.absoluteFilePath();
    qDebug()<<"FileTreeView::OnDoubleClicked";
    emit DoubleClickFile(path);
}


