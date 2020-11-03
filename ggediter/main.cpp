#include <mainwindow.h>
#include <QApplication>
#include "LangModule.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LangModule::Inst().Load();

//    QMessageBox::information(NULL, ("Exit Tip"), argv[1], ("确定"), ("取消"), 0, 1);

    CMainWindow w;
    if(argc>1){
        QString strFile = a.arguments().at(1);
        w.Open(strFile);
    }
    w.show();



    return a.exec();
}
