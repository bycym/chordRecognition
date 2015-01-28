#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "genchords/src/Sounddata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString imp = "sample.wav";
    Sounddata snd(imp);
}
