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
#include "WorkSpace.h"
#include "FileTreeView.h"

#define var auto


using namespace ads;


void CMainWindow::OpenFolder(const QString & dir)
{
    QString displayName = "";
    auto pos = dir.lastIndexOf("/");
    if(pos == -1){
        displayName = dir;
    }else{
        displayName = dir.mid(pos+1);
    }

    FileTreeView* fileTree = new FileTreeView(this);
    fileTree->Load(dir);
    //fileTree->setFrameShape(QFrame::NoFrame);

    CDockWidget* DataDockWidget = new CDockWidget(displayName);
    DataDockWidget->setWidget(fileTree);
    DataDockWidget->resize(150, 100);
    DataDockWidget->setMinimumSize(150, 100);
    DataDockWidget->setMaximumWidth(200);
    if(_lastFileArea == nullptr)
        _lastFileArea = DockManager->addDockWidget(ads::LeftDockWidgetArea, DataDockWidget, _lastEditArea);
    else
        _lastFileArea = DockManager->addDockWidget(ads::CenterDockWidgetArea, DataDockWidget, _lastFileArea);
    //ui->menuView->addAction(DataDockWidget->toggleViewAction());

    //_lastFileArea->resize(200,800);

    connect(fileTree,SIGNAL(ClickFile(const QString &)), this, SLOT(OnClickedFile(const QString &)));
    connect(fileTree,SIGNAL(DoubleClickFile(const QString &)), this, SLOT(OnDoubleClickedFile(const QString &)));

}

void CMainWindow::LoadWorkSpace(){
    WorkSpace::Inst().Load();
    auto & ws = WorkSpace::Inst();
    CDockAreaWidget *fileArea = nullptr;
    for (auto & prj : ws.projectList ) {
        OpenFolder(prj.rootPath);
    }

    if(fileArea){
        fileArea->setMaximumWidth(200);
    }else{

    }

    // last time opened files
    for(auto & of : ws.openedFileList){
        Open(of.filePath);
    }

    if(ws.openedFileList.isEmpty()){
        NewEdit(NULL, "");
    }
}


FileEditCtrl* CMainWindow::NewEdit(const char *data, QString filePath)
{
    auto w = new FileEditCtrl();

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
    _lastEditArea = DockManager->addDockWidget(ads::CenterDockWidgetArea, CentralDockWidget,_lastEditArea);

    connect(w, SIGNAL(uriDropped(const QString&)), this, SLOT(OnDropUri(const QString&)));

   // close event
    connect(CentralDockWidget, SIGNAL(closed()), this, SLOT(OnEditerDockWidgetClose()));

    _editerList.push_back(w);

    w->Init();
    if (data) {
        w->setText(data);
    }


    return w;
}

void CMainWindow::OnEditerDockWidgetClose(){
    qDebug() << "editer close";
    QObject *s = sender();
    auto dw = (CDockWidget*)s;
    auto editer = (FileEditCtrl*)dw->widget();
    auto it = _editerList.begin();
    for ( ;it!=_editerList.end();++it) {
        if( editer == (*it)){
            _editerList.erase(it);
            break;
        }
    }

}


CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    _lastFileArea = nullptr;
    _lastEditArea = nullptr;
    //支持拖放
    setAcceptDrops(true);

    LoadFileMenu();

    LoadEncodingMenu();

//    auto unDoAction = ui->menuEdit->addAction("UnDo", this, NULL, QKeySequence("Ctrl+Z"));
    //auto reDoAction = ui->menuEdit->addAction("ReDo");
    auto unDoAction = new QAction("UnDo", this);
    auto reDoAction = new QAction("ReDo", this);
    ui->menuEdit->addAction(unDoAction);
    ui->menuEdit->addAction(reDoAction);

    connect(ui->menuEdit, SIGNAL(triggered(QAction*)), this, SLOT(OnEditMenuTriggered(QAction*)));

    

    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    DockManager = new CDockManager(this);

    connect(DockManager, SIGNAL(uriDropped(const QString&)), this, SLOT(OnDropUri(const QString&)));


    LoadWorkSpace();

    // Set central widget
    //QPlainTextEdit* w = new QPlainTextEdit();
    //w->setPlaceholderText("This is the central editor. Enter your text here.");

    //auto* CentralDockArea = DockManager->setCentralWidget(CentralDockWidget);
    //CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    // create other dock widgets


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

void CMainWindow::OnEditMenuTriggered(QAction *act){

    auto edit = GetCurrentEdit();
    if(!edit){
        return;
    }

    auto actTxt = act->text();
    if(actTxt == "UnDo"){
        auto edit = GetCurrentEdit();
        if(edit){
            edit->undo();
        }
    }else if(actTxt == "ReDo") {
        edit->redo();
    }
}


void CMainWindow::on_actionNew_triggered()
{
    NewEdit(NULL,"");
}

void CMainWindow::on_actionOpen_triggered()
{
    QString strFilePath = QFileDialog::getOpenFileName(NULL, "Open", "", "*.*");
    if(strFilePath.isEmpty()){
        return ;
    }

    Open(strFilePath);

}

void CMainWindow::Open(const QString& strFilePath)
{

    QFile file(strFilePath);

    if (!file.open(QFile::ReadOnly))
    {
        auto str = QString("%1 Open Failed").arg(strFilePath);
        QMessageBox::warning(NULL, "Error", str, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return ;
    }

    auto len = file.size();
    //QByteArray arr = file.read(1024);
    //qDebug() << arr;
    char* buf = new char[len+1];
    qint64 lineLength = file.read(buf, len);
    buf[len] = '\0';
    if (lineLength != -1)
    {
        NewEdit(buf, strFilePath);
    }
    delete[]buf;
}

void CMainWindow::on_actionSave_triggered()
{
    var edit = GetCurrentEdit();
    if (!edit) {
        return;
    }

    edit->Save();
}

void CMainWindow::on_actionSave_as_triggered()
{

}

void CMainWindow::on_actionSave_All_triggered()
{

}

void CMainWindow::LoadFileMenu()
{
    auto action = ui->menuView->addAction("Open Folder");
    action->setShortcut(QKeySequence("Ctrl+K,Ctrl+O"));
    ui->menuView->addAction(action);
    connect(ui->menuView, SIGNAL(triggered(QAction*)), this, SLOT(OnFileMenuTriggered(QAction *)));
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

void CMainWindow::OnFileMenuTriggered(QAction *act){
    auto actTxt = act->text();
    if(actTxt == "Open Folder"){
        auto dir = QFileDialog::getExistingDirectory();
        OpenFolder(dir);
    }
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
    QUrl url(uri);
    auto strPath = url.toLocalFile();
    Open(strPath);
}


void CMainWindow::closeEvent(QCloseEvent * event)
{
    // check unsave editer
    auto dockMap = DockManager->dockWidgetsMap();


    auto op = QMessageBox::information(this, tr("Exit Tip"), tr("请先停止运行"), tr("确定"), tr("取消"), 0, 1);

    // save worksapce

}

void CMainWindow::OnClickedFile(const QString &filePath)
{
    Open(filePath);
}

void CMainWindow::OnDoubleClickedFile(const QString & filePath)
{

}
