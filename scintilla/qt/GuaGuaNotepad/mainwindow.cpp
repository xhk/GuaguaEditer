#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _edit = new ScintillaEdit(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

