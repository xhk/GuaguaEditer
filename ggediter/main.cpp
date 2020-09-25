#include <mainwindow.h>
#include <QApplication>
#include "LangModule.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LangModule::Inst().Load();

    CMainWindow w;
    w.show();
    return a.exec();
}
