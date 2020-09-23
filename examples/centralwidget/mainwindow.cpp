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

#define var auto


using namespace ads;

CDockAreaWidget *area = nullptr;

ScintillaEdit * CMainWindow::NewEdit(const char *data, QString fileName)
{
    auto w = new ScintillaEdit();
    // 2号页边，宽度为20，显示行号
    w->setMarginTypeN(2, SC_MARGIN_NUMBER);
    w->setMarginWidthN(2,20);
    w->setCodePage(SC_CP_UTF8);

    if (data) {
        w->setText(data);
    }
    
    auto dockMap = DockManager->dockWidgetsMap();
    QString newName = "";
    
    if (fileName.isEmpty()) {
        int index = 1;

        for (auto it = dockMap.begin(); it != dockMap.end(); ++it) {
            auto name = it.key();
            name = name.replace("New ", "");
            int n = name.toInt();
            if (n >= index) {
                index = n + 1;
            }
        }
        newName = QString("New %1").arg(index);
    }
    else {
        newName = fileName;
    }
    
    CDockWidget* CentralDockWidget = new CDockWidget(newName);
    CentralDockWidget->setWidget(w);

    area = DockManager->addDockWidget(ads::CenterDockWidgetArea, CentralDockWidget,area);

    return w;
}


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    LoadEncodingMenu();

    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    DockManager = new CDockManager(this);

    NewEdit(NULL, "");

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
    NewEdit(NULL,"");
}

void CMainWindow::on_actionOpen_triggered()
{
    QString strFilePath = QFileDialog::getOpenFileName(NULL, "标题", ".", "*.*");
    QString strFileName = "";
    QFile file(strFilePath); 
    var lastPos = strFilePath.lastIndexOf("/");
    if (lastPos != -1) {
        strFileName = strFilePath.mid(lastPos+1);
    }
    else {
        strFileName = strFilePath;
    }

    if (file.open(QFile::ReadOnly))
    {
        auto len = file.size();
        //QByteArray arr = file.read(1024);
            //qDebug() << arr;
        char *buf = new char[len];
        qint64 lineLength = file.read(buf, len);
        if (lineLength != -1)
        {
            NewEdit(buf, strFileName);
        }
        delete[]buf;
    }

}

void CMainWindow::on_actionSave_triggered()
{

}

void CMainWindow::on_actionSave_as_triggered()
{

}

void CMainWindow::on_actionSave_All_triggered()
{

}

void CMainWindow::LoadEncodingMenu()
{
    struct Encoding {
        const char* name;
        int codePage;
    } knownEncodings[] = {
        { "ascii", 65001 },
        { "utf-8", 65001 },
        { "latin1", 1252 },
        { "latin2", 28592 },
        { "big5", 950 },
        { "gbk", 936 },
        { "shift_jis", 932 },
        { "euc-kr", 949 },
        { "cyrillic", 1251 },
        { "iso-8859-5", 28595 },
        { "iso8859-11", 874 },
        { "1250", 1250 },
        { "windows-1251", 1251 },
    };

    for (const Encoding& enc : knownEncodings) {
        ui->menuEncoding->addAction(enc.name);
        _codePageMap.insert(enc.name, enc.codePage);
    }
    
    connect(ui->menuEncoding, SIGNAL(triggered(QAction*)), this, SLOT(trigerEncodingMenu(QAction*)));
}

void CMainWindow::trigerEncodingMenu(QAction* act)
{
    var txt = act->text();
    var it = _codePageMap.find(txt);
    if (it == _codePageMap.end()) {
        return;
    }

    var dockWidget = DockManager->focusedDockWidget();
    var widget = dockWidget->widget();
    var edit = dynamic_cast<ScintillaEdit*>(widget);
    if (edit) {
        edit->setCodePage(it.value());
    }
    act->setCheckable(true);
    act->setChecked(true);
}