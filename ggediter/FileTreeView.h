#ifndef FILETREEVIEW_H
#define FILETREEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

class FileTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FileTreeView(QWidget *parent = nullptr);

    void Load(const QString & rootDir);

signals:
    void ClickFile(const QString & file);
    void DoubleClickFile(const QString & file);
private slots:
        void OnClicked(const QModelIndex &index);
        void OnDoubleClicked(const QModelIndex & index);
private:
    QStandardItemModel *_model;

};

#endif // FILETREEVIEW_H
