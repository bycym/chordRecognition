#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileOperator_ = new FileOperator(this);
    playSound_ = new PlaySound(this);
    sndData_ = new SoundData();
    ui->playButton->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString imp = "sample.wav";

    if(fileOperator_->performLoadOperation(imp, sndData_)) {
        ui->successLabel->setText("OK");
        ui->playButton->setVisible(true);
        ui->successLabel->setStyleSheet("QLabel { color: green }");
    }
    else {
        ui->successLabel->setText("ERROR");
        ui->playButton->setVisible(false);
        ui->successLabel->setStyleSheet("QLabel { color: red }");
    }

    getFeatures(sndData_);
}

void MainWindow::on_openButton_clicked()
{

    if(fileOperator_->open(sndData_))
    {
        ui->successLabel->setText("OK");
        ui->playButton->setVisible(true);
        ui->successLabel->setStyleSheet("QLabel { color: green }");
    }
    else
    {

        ui->successLabel->setText("ERROR");
        ui->playButton->setVisible(false);
        ui->successLabel->setStyleSheet("QLabel { color: red }");
    }
}

void MainWindow::on_playButton_clicked()
{
    if(!playSound_->Play(sndData_))
    {
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Nem sikerult lejatszani a faljt.");
        mb.setInformativeText("Raw audio format not supported by backend, cannot play audio.");
        mb.exec();
    }
}
