#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"
#include <ScintillaEdit.h>

#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_All_triggered();

    void trigerEncodingMenu(QAction* act);

private:
    static const QString kTableTopLayout;
    static const QString kTableBottomLayout;

    Ui::CMainWindow *ui;

    ads::CDockManager* DockManager;
    ads::CDockAreaWidget* StatusDockArea;
    ads::CDockWidget* TimelineDockWidget;

    ScintillaEdit *_edit;

    ScintillaEdit * NewEdit(const char *data, QString fileName);

    void LoadEncodingMenu();

    QMap<QString, int> _codePageMap;
};
#endif // MAINWINDOW_H
