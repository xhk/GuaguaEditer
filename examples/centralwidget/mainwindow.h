#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"
#include <ScintillaEdit.h>

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

private:
    static const QString kTableTopLayout;
    static const QString kTableBottomLayout;

    Ui::CMainWindow *ui;

    ads::CDockManager* DockManager;
    ads::CDockAreaWidget* StatusDockArea;
    ads::CDockWidget* TimelineDockWidget;

    ScintillaEdit *_edit;

    void NewEdit();
};
#endif // MAINWINDOW_H
