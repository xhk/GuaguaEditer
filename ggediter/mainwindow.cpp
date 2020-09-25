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
#include "SciLexer.h"

#define var auto


using namespace ads;

CDockAreaWidget *area = nullptr;

FileEditCtrl* CMainWindow::NewEdit(const char *data, QString filePath)
{
    auto w = new FileEditCtrl();
    w->Init();

    if (data) {
        w->setText(data);
    }
    
    auto dockMap = DockManager->dockWidgetsMap();
    QString newName = "";
    
    if (filePath.isEmpty()) {
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
        QString strFileName = "";
        var lastPos = filePath.lastIndexOf("/");
        if (lastPos != -1) {
            strFileName = filePath.mid(lastPos + 1);
        }
        else {
            strFileName = filePath;
        }

        newName = strFileName;
    }
    w->SetFileName(newName);
    w->SetFilePath(filePath);
    
    CDockWidget* CentralDockWidget = new CDockWidget(newName);
    CentralDockWidget->setWidget(w);
    w->dockWidget = CentralDockWidget;
    area = DockManager->addDockWidget(ads::CenterDockWidgetArea, CentralDockWidget,area);

    connect(w, SIGNAL(uriDropped(const QString&)), this, SLOT(OnDropUri(const QString&)));

   


    return w;
}


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    //支持拖放
    setAcceptDrops(true);

    LoadEncodingMenu();

    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    DockManager = new CDockManager(this);

    connect(DockManager, SIGNAL(uriDropped(const QString&)), this, SLOT(OnDropUri(const QString&)));

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

    setWindowTitle("GuaGuaEditer");
    
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
    QString strFilePath = QFileDialog::getOpenFileName(NULL, "Open", "", "*.*");

    Open(strFilePath);

}

void CMainWindow::Open(const QString& strFilePath)
{
    auto strPath = strFilePath;
    QString fileProtoHeader = "file:///";
    if (strPath.startsWith(fileProtoHeader)) {
        strPath = strPath.mid(fileProtoHeader.length());
    }
    QFile file(strPath);

    if (file.open(QFile::ReadOnly))
    {
        auto len = file.size();
        //QByteArray arr = file.read(1024);
        //qDebug() << arr;
        char* buf = new char[len+1];
        qint64 lineLength = file.read(buf, len);
        buf[len] = '\0';
        if (lineLength != -1)
        {
            NewEdit(buf, strPath);
        }
        delete[]buf;
    }
}

void CMainWindow::on_actionSave_triggered()
{
    var edit = GetCurrentEdit();
    if (!edit) {
        return;
    }

    var arr = edit->GetAllText();
    QString strFilePath = edit->GetFilePath();
    if (strFilePath.isEmpty()) {
        // new file
        strFilePath = QFileDialog::getSaveFileName(this, "Save", "", "*.*");
    }

    if (strFilePath.isEmpty()) {
        return;
    }

    edit->SetFilePath(strFilePath);

    QFile file(strFilePath);
    if (!file.open(QFile::WriteOnly)) {
        QString tip = edit->GetFilePath() + "open faild!";
        qDebug() << edit->GetFilePath() << "open faild!\n";
        QMessageBox::warning(this, "save error", tip, QMessageBox::Ok);
        return;
    }

    file.write(arr);
    file.close();
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
    var edit = dynamic_cast<FileEditCtrl*>(widget);
    if (edit) {
        edit->setCodePage(it.value());
    }
    act->setCheckable(true);
    act->setChecked(true);
}

FileEditCtrl* CMainWindow::GetCurrentEdit()
{
    var dockWidget = DockManager->focusedDockWidget();
    var widget = dockWidget->widget();
    var edit = dynamic_cast<FileEditCtrl*>(widget);
    if (edit && edit->focus()) {
        return edit;
    }
     
    return nullptr;
}

//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void CMainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

//当用户放下这个文件后，就会触发dropEvent事件
void CMainWindow::dropEvent(QDropEvent* event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    //往文本框中追加文件名
    foreach(QUrl url, urls) {
        QString file_name = url.toLocalFile();
        Open(file_name);
    }
}

void CMainWindow::OnDropUri(const QString& uri) {
    Open(uri);
}