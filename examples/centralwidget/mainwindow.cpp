#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QWidgetAction>
#include <QLabel>
#include <QCalendarWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QPlainTextEdit>

#include "DockAreaWidget.h"
#include "DockAreaTitleBar.h"
#include "DockAreaTabBar.h"
#include "FloatingDockContainer.h"
#include "DockComponentsFactory.h"

using namespace ads;

CDockAreaWidget *area = nullptr;

void CMainWindow::NewEdit()
{
    auto w = new ScintillaEdit();
    // 2号页边，宽度为20，显示行号
    w->setMarginTypeN(2, SC_MARGIN_NUMBER);
    w->setMarginWidthN(2,20);

    //w->setText("aaaaaa");

    auto names = DockManager->perspectiveNames();
    QString newName = "";
    int index = 1;
    for(int i=0;i<names.size();++i){
        auto name = names[i];
        name = name.replace("New ", "");
        int n = name.toInt();
        if(n>index){
            index = n+1;
        }
    }
    newName = QString("New %1").arg(index);

    CDockWidget* CentralDockWidget = new CDockWidget(newName);
    CentralDockWidget->setWidget(w);

    area = DockManager->addDockWidget(ads::CenterDockWidgetArea, CentralDockWidget,area);
}


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    DockManager = new CDockManager(this);

    NewEdit();

    // Set central widget
    //QPlainTextEdit* w = new QPlainTextEdit();
    //w->setPlaceholderText("This is the central editor. Enter your text here.");

    //auto* CentralDockArea = DockManager->setCentralWidget(CentralDockWidget);
    //CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    // create other dock widgets
    QTreeView* fileTree = new QTreeView();
    fileTree->setFrameShape(QFrame::NoFrame);
    /*QFileSystemModel* fileModel = new QFileSystemModel(fileTree);
    fileModel->setRootPath(QDir::currentPath());
    fileTree->setModel(fileModel);*/
    CDockWidget* DataDockWidget = new CDockWidget("File system");
    DataDockWidget->setWidget(fileTree);
    DataDockWidget->resize(150, 100);
    DataDockWidget->setMinimumSize(150, 100);
    //DataDockWidget->setMaximumWidth(150);
    auto* fileArea = DockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea, DataDockWidget);
    ui->menuView->addAction(DataDockWidget->toggleViewAction());

    //fileArea->resize(200,800);
    fileArea->setMaximumWidth(200);


    /*
    QTableWidget* table = new QTableWidget();
    table->setColumnCount(3);
    table->setRowCount(10);
    CDockWidget* TableDockWidget = new CDockWidget("Table");
    TableDockWidget->setWidget(table);
    TableDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    TableDockWidget->resize(250, 150);
    TableDockWidget->setMinimumSize(200,150);
    DockManager->addDockWidget(DockWidgetArea::BottomDockWidgetArea, TableDockWidget, fileArea);
    ui->menuView->addAction(TableDockWidget->toggleViewAction());
*/
    /*QTableWidget* propertiesTable = new QTableWidget();
    propertiesTable->setColumnCount(3);
    propertiesTable->setRowCount(10);
    CDockWidget* PropertiesDockWidget = new CDockWidget("Properties");
    PropertiesDockWidget->setWidget(propertiesTable);
    PropertiesDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    PropertiesDockWidget->resize(250, 150);
    PropertiesDockWidget->setMinimumSize(200,150);
    DockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, PropertiesDockWidget, CentralDockArea);
    ui->menuView->addAction(PropertiesDockWidget->toggleViewAction());
    */

}

CMainWindow::~CMainWindow()
{
    delete ui;
}


void CMainWindow::on_actionNew_triggered()
{
    NewEdit();
}

void CMainWindow::on_actionOpen_triggered()
{

}
