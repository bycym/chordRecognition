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

    databaseRead_ = false;
    sampleRead_ = false;

    databaseFeatures_ = new QVector<GetFeatures *>();
/*
    connect(NeuralNetworkForm, SIGNAL(setParameters(
                                                    const int numInput,
                                                    const int numOutput,
                                                    const int numHiddenLayer,
                                                    const int numHiddenNeuron,
                                                    const double learningrate)),
                         this, SLOT(createNeuralNetwork(
                                         const int numInput,
                                         const int numOutput,
                                         const int numHiddenLayer,
                                         const int numHiddenNeuron,
                                         const double learningrate)));

*/
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
        coreFunction();

    }
    else
    {
        ui->successLabel->setText("ERROR");
        ui->playButton->setVisible(false);
        ui->successLabel->setStyleSheet("QLabel { color: red }");
        sampleRead_ = false;
    }


}

void MainWindow::on_openButton_clicked()
{

    if(fileOperator_->open(sndData_))
    {
        ui->successLabel->setText("OK");
        ui->playButton->setVisible(true);
        ui->successLabel->setStyleSheet("QLabel { color: green }");
        sndData_->info();
        coreFunction();
    }
    else
    {

        ui->successLabel->setText("ERROR");
        ui->playButton->setVisible(false);
        ui->successLabel->setStyleSheet("QLabel { color: red }");
        sampleRead_ = false;

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
    sndDataFeatures_= NULL;
    sndDataFeatures_= new GetFeatures(sndData_);
    info();
    sampleRead_ = true;

}

void MainWindow::on_readDir_clicked()
{
    if(fileOperator_->openDir(database_))
    {
        ui->dbSuccessLabel->setText("OK");
        ui->dbSuccessLabel->setStyleSheet("QLabel { color: green }");
        databaseRead_ = true;


        databaseFeatures_->clear();

        for(auto a : database_)
        {
            databaseFeatures_->push_back(new GetFeatures(a));
        }
        qDebug() << databaseFeatures_->size() << " piece of features calculated";
    }
    else
    {
        ui->dbSuccessLabel->setText("ERROR");
        ui->dbSuccessLabel->setStyleSheet("QLabel { color: red }");
        databaseRead_ = false;
    }
}

void MainWindow::on_neuralNetwork_Button_clicked()
{
    /// if all stipulation ok
    /// like read the database and read input file
    /// then we can create the neural network
    if(sampleRead_ && databaseRead_)
    {
        bool ok = false;
        int numInput = 10;
        int numOutput = 10;
        int numHiddenLayer = 1;
        int numHiddenNeuron = 35;
        double learningrate = 0.001;

        nnf_ = new NeuralNetworkForm(this);
        nnf_->show();

        /*
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
            */

        NeuralNetworks * nn = new NeuralNetworks(10);

        int value = 1;
        for(int i = 0; i < value; i++)
        {
            cout << "i : " << i << endl;
            std::vector<double> input;
            //nn->updateInputs(input);
            // melyik pcp track-es cuccot kell beletuszkolni?
            // a pcp.h-ból?


            /// tag init
            std::vector<std::string> tags;
            tags.push_back("a");
            tags.push_back("am");
            tags.push_back("b");
            tags.push_back("bm");
            tags.push_back("c");
            tags.push_back("d");
            tags.push_back("dm");
            tags.push_back("e");
            tags.push_back("em");
            tags.push_back("f");
            tags.push_back("g");
            nn->setTag(tags);


            //nn->computeOutputs();
            //nn->updateErrorSignal();
            //nn->updateWeights();
            cout << nn->getTag();
            qDebug() << QString::fromStdString(nn->getTag());
        }

    }
    else
    {
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Hiányzik egy feltétel!.");

        qDebug() << "One of the stipulation to create Neural"
                    << "Network isn't done.";
        QString problem ="Problem:\n";
        if(!sampleRead_)
        {
            qDebug() << "input file didn't read yet!";
            problem += "Input file didn't read yet!\n";
        }
        if(!databaseRead_)
        {
            qDebug() << "database didn't read yet!";
            problem += "Database didn't read yet!\n";
        }
        qDebug() << "One of the stipulation to create Neural"
                    << "Network isn't done.";

        mb.setInformativeText(problem);

        mb.exec();
    }



}


NeuralNetworks * MainWindow::createNeuralNetwork(const int numInput, const int numOutput, const int numHiddenLayer, const int numHiddenNeuron, const double learningrate)
{
    NeuralNetworks * nn = new NeuralNetworks(numInput, numOutput, numHiddenLayer, numHiddenNeuron, learningrate);

    return nn;
}
