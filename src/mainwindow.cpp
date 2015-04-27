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
    delete sndData_;
    delete playSound_;
    delete fileOperator_;
    delete nnf_;
    for(auto x : database_)
        delete x;
    database_.clear();

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString imp = "sample.wav";

    if(fileOperator_->performLoadOperation(imp, sndData_)) {
        ui->successLabel->setText("OK");
        ui->playButton->setVisible(true);
        ui->successLabel->setStyleSheet("QLabel { color: green }");
        sndData_->info();
    }
    else {
        ui->successLabel->setText("ERROR");
        ui->playButton->setVisible(false);
        ui->successLabel->setStyleSheet("QLabel { color: red }");
    }

    coreFunction();

}

void MainWindow::on_openButton_clicked()
{

    if(fileOperator_->open(sndData_))
    {
        ui->successLabel->setText("OK");
        ui->playButton->setVisible(true);
        ui->successLabel->setStyleSheet("QLabel { color: green }");
        sndData_->info();
    }
    else
    {

        ui->successLabel->setText("ERROR");
        ui->playButton->setVisible(false);
        ui->successLabel->setStyleSheet("QLabel { color: red }");
    }

    coreFunction();
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

void MainWindow::info()
{
    ui->treeView->reset();

    QVector<QString> info = sndData_->infoQt();

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *parentItem = model->invisibleRootItem();

    for (int i = 0; i < info.size(); ++i) {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(info[i]));
        parentItem->appendRow(item);
    }

    ui->treeView->setModel(model);
    ui->treeView->show();
}


void MainWindow::coreFunction()
{
    getFeatures(sndData_);
    info();

}

void MainWindow::on_readDir_clicked()
{
    if(fileOperator_->openDir(database_))
    {
        ui->dbSuccessLabel->setText("OK");
        ui->dbSuccessLabel->setStyleSheet("QLabel { color: green }");
    }
    else
    {
        ui->dbSuccessLabel->setText("ERROR");
        ui->dbSuccessLabel->setStyleSheet("QLabel { color: red }");
    }
}

void MainWindow::on_neuralNetwork_Button_clicked()
{
    bool ok = false;
    int numInput = 10;
    int numOutput = 10;
    int numHiddenLayer = 1;
    int numHiddenNeuron = 35;
    double learningrate = 0.001;

    nnf_ = new NeuralNetworkForm(this);
    nnf_->show();

    numInput = nnf_->numInput();
    numOutput = nnf_->numOutput();
        numHiddenLayer = nnf_->numHiddenLayer();
        numHiddenNeuron = nnf_->numHiddenNeuron();
        learningrate = nnf_->learningrate();
        if(!(numInput == -1 ||
                numOutput == -1 ||
                numHiddenLayer == -1 ||
                numHiddenNeuron == -1 ||
                learningrate == -1))
        {
            ok = true;
            qDebug() << "Everything ok!";
        }
        else
        {
            qDebug() << "Problem with parameters";
            QMessageBox mb;
            mb.setIcon(QMessageBox::Critical);
            mb.setText("Nem sikerült létrehozni.");
            mb.setInformativeText("Rossz paraméterek!");
            mb.exec();
        }


}
