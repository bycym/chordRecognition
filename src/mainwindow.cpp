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
    train_ = false;
    databaseRead_ = false;
    sampleRead_ = false;
    sndDataFeatures_ = NULL;
    databaseFeatures_ = new std::vector<GetFeatures *>();




    //////////////////////////////////
    /// database dev stuff
    //////////////////////////////////

    void dataBaseDev();
    dev_databaseFeatures_ = new QVector<GetFeatures *>();
    dev_database_;
    flip = true;
    ui->switch_label->setText("use file");

    //////////////////////////////////
    /// database dev stuff EEEENNDDD
    //////////////////////////////////
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
    delete databaseFeatures_;
    for(auto x : dev_database_)
        delete x;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString imp = "sample.wav";

    if(fileOperator_->performLoadOperation(imp, sndData_)) {
        ui->successLabel->setText("OK");
        ui->playButton->setVisible(true);
        ui->successLabel->setStyleSheet("QLabel { color: green }");
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

void MainWindow::info(QVector<QString> info)
{
    ui->treeView->reset();

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *parentItem = model->invisibleRootItem();

    for (int i = 0; i < info.size(); ++i) {
        QStandardItem *item = new QStandardItem(QString("%0").arg(info[i]));
        parentItem->appendRow(item);
    }

    ui->treeView->setModel(model);
    ui->treeView->show();
}


void MainWindow::coreFunction()
{
    if(sndDataFeatures_ != NULL)
    {
        std::cout << "delete sndDataFeatures_" << endl;
        delete sndDataFeatures_;
    }

    sndDataFeatures_= new GetFeatures(sndData_, true);
    //sndData_->info();
    info(sndData_->infoQt());

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
            databaseFeatures_->push_back(new GetFeatures(a, false));
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

    /// végig menni xszer a cikk alapján az adatbázison, majd tesztelni, hogy megy
    /// majd pedig mehet az ablakozott pcp-s cucc
    /// a pcp-ből ott van a vektor ami kell a pcptrackben
    ///


    /// if all stipulation ok
    /// like read the database and read input file
    /// then we can create the neural network
    if((sampleRead_ || sampleDBRead_) && databaseRead_)
    {
        bool ok = false;
        int cycle = 1;
        int numInput = 10;
        int numOutput = 10;
        int numHiddenLayer = 1;
        int numHiddenNeuron = 35;
        double learningrate = 0.001;


        // number of hidden layer
        if(!ui->numHiddenLayer_lineEdit->text().isEmpty())
        {
            bool ok = false;
            int result = ui->numHiddenLayer_lineEdit->text().toInt(&ok);
            if(ok)
                numHiddenLayer = result;
            else{
                qDebug() << "Bad value numHiddenLayer";
            }
        }

        //number of neuron
        if(!ui->numNeuron_lineEdit->text().isEmpty())
        {
            bool ok = false;
            int result = ui->numNeuron_lineEdit->text().toInt(&ok);
            if(ok)
                numHiddenNeuron = result;
            else{
                qDebug() << "Bad value numHiddenNeuron";
            }
        }

        // learning rate
        if(!ui->learningrate_lineEdit->text().isEmpty())
        {
            bool ok = false;
            double result = ui->learningrate_lineEdit->text().toDouble(&ok);
            if(ok)
                learningrate = result;
            else{
                qDebug() << "Bad value learningrate";
            }
        }

        // cycle rate
        if(!ui->trainCycle_lineEdit->text().isEmpty())
        {
            bool ok = false;
            double result = ui->trainCycle_lineEdit->text().toInt(&ok);
            if(ok)
                cycle = result;
            else{
                qDebug() << "Bad value train cycle";
            }
        }

        ui->numHiddenLayer_label->setText(QString::number(numHiddenLayer));
        ui->numNeuron_label->setText(QString::number(numHiddenNeuron));
        ui->learningrate_label->setText(QString::number(learningrate));
        ui->trainCycle_label->setText(QString::number(cycle));

        /// if it didn't train yet
        if(!train_)
        {

            QVector<QString> resultInfo;
            resultInfo.push_back(QString::fromStdString(sndData_->waveFileName()));
            cout << "numOutput: " << numOutput <<endl;
            resultInfo.push_back("numOutput: "+QString::fromStdString(std::to_string(numOutput)));
            cout << "numHiddenLayer: " << numHiddenLayer <<endl;
            resultInfo.push_back("numHiddenLayer: "+QString::fromStdString(std::to_string(numHiddenLayer)));
            cout << "numHiddenNeuron: " << numHiddenNeuron <<endl;
            resultInfo.push_back("numHiddenNeuron: "+QString::fromStdString(std::to_string(numHiddenNeuron)));
            cout << "learningrate: " << learningrate <<endl;
            resultInfo.push_back("learningrate: "+QString::fromStdString(std::to_string(learningrate)));

            info(resultInfo);


            //nnf_ = new NeuralNetworkForm(this);
            //nnf_->show();

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


            /// tag init
            tags_.clear();
            tags_.push_back("a");
            tags_.push_back("am");
            //tags_.push_back("b");
            tags_.push_back("bm");
            tags_.push_back("c");
            tags_.push_back("d");
            tags_.push_back("dm");
            tags_.push_back("e");
            tags_.push_back("em");
            tags_.push_back("f");
            tags_.push_back("g");

            for(auto t : tags_)
                cout << t << ", ";
            cout << endl;


            int PCPLEN = 12;

            neuralnetworks_ = new NeuralNetworks(PCPLEN, tags_.size(),numHiddenLayer,numHiddenNeuron,learningrate);
            neuralnetworks_->setTag(tags_);


            for(int i  = 0; i < cycle; i++)
                train();

            train_ = true;
            ui->neuralNetworklabel->setText("Trained");
            ui->neuralNetworklabel->setStyleSheet("QLabel { color: green }");

            QMessageBox mb;
            mb.setIcon(QMessageBox::Information);
            mb.setText("Kész a tanítás.");
            mb.setInformativeText("Mégegyszer futtatva kiértékel.");

            mb.exec();

        }
        /// if it trained then dev it!
        else if(train_)
        {
            if(flip)
            {
                if(sampleRead_)
                    devel();
                else
                {
                    QMessageBox mb;
                    mb.setIcon(QMessageBox::Critical);
                    mb.setText("Nem lett beolvasva kiértékelni való fájl.");
                    mb.setInformativeText("FAIL");
                    mb.exec();
                }
            }
            else
            {
                if(sampleDBRead_)
                    dataBaseDev();
                else
                {
                    QMessageBox mb;
                    mb.setIcon(QMessageBox::Critical);
                    mb.setText("Nem lett beolvasva kiértékelni való adatbázis.");
                    mb.setInformativeText("FAIL");
                    mb.exec();
                }
            }
        }
        else
        {
            QMessageBox mb;
            mb.setIcon(QMessageBox::Information);
            mb.setText("Nem lett betanítva az adatbázis.");
            mb.setInformativeText("Train FAIL");

            mb.exec();
            ui->neuralNetworklabel->setText("ERROR");
            ui->neuralNetworklabel->setStyleSheet("QLabel { color: red }");

        }
    }
    else
    {
        ui->neuralNetworklabel->setText("ERROR");
        ui->neuralNetworklabel->setStyleSheet("QLabel { color: red }");
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

void MainWindow::train()
{


    std::vector<double> target;
    target.clear();
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);
    target.push_back(0.0);



    /// !!! MOST IMPORTANT THING EVER !!!
    std::random_shuffle(databaseFeatures_->begin(),databaseFeatures_->end());


    // D.B. teaching
    for(int i = 0; i < databaseFeatures_->size(); i++)
    {
        cout << endl << endl << endl <<"i : " << i << endl;




        //databaseFeatures_->at(i);
        std::vector<double> input;


        for(auto &t : target)
            t = 0.0;


//        cout << "tag:" << databaseFeatures_->at(i)->dbTag <<" - output tag:"<< neuralnetworks_->getTag()<<endl;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(0))
            target.at(0) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(1))
            target.at(1) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(2))
            target.at(2) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(3))
            target.at(3) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(4))
            target.at(4) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(5))
            target.at(5) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(6))
            target.at(6) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(7))
            target.at(7) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(8))
            target.at(8) = 1.0;
        if(databaseFeatures_->at(i)->dbTag == tags_.at(9))
            target.at(9) = 1.0;


        for(int j = 0; j < databaseFeatures_->at(i)->pcptrack.size(); j++)
        {

            for(auto pcp : databaseFeatures_->at(i)->pcptrack.at(j).pcp)
            {
                input.push_back((double)pcp);
            }

            /*
            for(int pp = 0; pp < 12; pp++)//
            {
                input.push_back(databaseFeatures_->at(i)->pcptrack.at(j).pcp[pp]);
            }

            cout << "input size: " << input.size() << endl;
            for(auto x : input)
                cout << x << ", ";
            cout << endl;

            cout << endl << endl << endl;
*/
/*
            cout << "target size: " << target.size() << endl;
            for(auto x : target)
                cout << x << ", ";
            cout << endl;
*/
            //cout << "update input" << endl;
            neuralnetworks_->updateInputs(input);

            //cout << "compute outputs" << endl;
            neuralnetworks_->computeOutputs();

            //cout << "update error signal" << endl;
/*
            cout << "target size: " << target.size() << endl;
            for(auto x : target)
                cout << x << ", ";
            cout << endl;
*/
            neuralnetworks_->updateErrorSignal(target);

            //cout << "update weights" << endl;
            neuralnetworks_->updateWeights();


/*
            cout <<endl<< "outputs:"<<endl;
            for(auto o : neuralnetworks_->outputs())
            {
                cout << o << ", ";
            }
            cout <<endl;
*/
            cout << "tag:" << databaseFeatures_->at(i)->dbTag <<" - output tag:"<< neuralnetworks_->getTag()<<endl;
            /*
            //qDebug() << QString::fromStdString(nn->getTag());
            cout << "-------------------------------------"<<endl;
*/


            input.clear();

        }

        //delete feature;
    }

    train_ = true;
    cout << endl << "train done" << endl;

}

void MainWindow::devel()
{

    std::vector<int> tagsCount;
    for(int i = 0; i < tags_.size(); i++)
    {
        tagsCount.push_back(0);
    }
    /*
    int a_string = 0;
    int am_string = 0;
    int bm_string = 0;
    int c_string = 0;
    int d_string = 0;
    int dm_string = 0;
    int e_string = 0;
    int em_string = 0;
    int f_string = 0;
    int g_string = 0;
    */
    int tagsum = 0;

/*
    for(int i = 0; i<neuralnetworks_->numOutput(); i++)
    {
        std::cout << neuralnetworks_->tag(i) << ",";
    }
    std::cout << endl;
*/
    std::string resultTag = "";
    std::vector<double> input;
    for(int j = 0; j < sndDataFeatures_->pcptrack.size(); j++)
    {

        for(auto pcp : sndDataFeatures_->pcptrack.at(j).pcp)
        {
            input.push_back((double)pcp);
        }

/*
        cout << endl << "input size: " << input.size() << endl;
        for(auto x : input)
            cout << x << ", ";
        cout << endl;
*/
        //cout << "update input" << endl;
        neuralnetworks_->updateInputs(input);

        //cout << "compute outputs" << endl;
        neuralnetworks_->computeOutputs();
        //std::cout << "tag: " << neuralnetworks_->getTag() << std::endl;
        std::string tmptag = neuralnetworks_->getTag();

/*
        double max = 0;
        int maxi = 0;


        cout <<endl<< "outputs:"<<endl;
        for(int kk = 0; kk < neuralnetworks_->outputs().size(); kk++)
        {
            if(neuralnetworks_->outputs().at(kk) > max)
            {
                max = neuralnetworks_->outputs().at(kk);
                maxi = kk;
            }
            cout << neuralnetworks_->outputs().at(kk) << ", ";
        }
        cout <<endl;
        cout << "max: " << max<<endl;
*/
        /*
        if(tmptag == tags_[0])
            a_string++;
        else if(tmptag == tags_[1])
            am_string++;
        else if(tmptag == tags_[2])
            bm_string++;
        else if(tmptag == tags_[3])
            c_string++;
        else if(tmptag == tags_[4])
            d_string++;
        else if(tmptag == tags_[5])
            dm_string++;
        else if(tmptag == tags_[6])
            e_string++;
        else if(tmptag == tags_[7])
            em_string++;
        else if(tmptag == tags_[8])
            f_string++;
        else if(tmptag == tags_[9])
            g_string++;
        */

        for(int k = 0; k < tags_.size(); k++)
        {
            if(tmptag == tags_[k])
            {
                tagsCount[k]++;
                tagsum++;
            }
        }

        resultTag+= tmptag;
        resultTag+= ", ";
//        qDebug() << QString::fromStdString(neuralnetworks_->getTag());
        input.clear();
    }

    cout << endl;

    /// print out everything!
    QVector<QString> resultInfo;

    cout << "................................................................." << endl;
    resultInfo.push_back(".................................................................");
    int max = 0;
    int maxIndex = 0;

    for(int i = 0; i < tags_.size(); i++)
    {
        std::string line = "";

        if(tagsCount[i] > max)
        {
            max = tagsCount[i];
            maxIndex = i;
        }
        cout << tags_[i] << ": " << tagsCount[i] << "\t|";
        std::string tmp = tags_[i];
        line += tmp + std::to_string(tagsCount.at(i)) + "\t|";

        for(int j = 0; j < tagsCount[i]; j++){
            cout << "#";
            line += "#";
        }
        cout << endl;
        resultInfo.push_back(QString::fromStdString(line));
    }

    cout << "................................................................." << endl << endl;
    resultInfo.push_back(QString::fromStdString(sndData_->waveFileName()));
    resultInfo.push_back(".................................................................");


    std::cout << "************ Key of the song with Neural Network ************" << std::endl;
    std::cout <<  tags_[maxIndex] << std::endl;
    std::cout << "*************************************************************" << std::endl << std::endl;
    std::cout << "************ Key of the song with Genchords ************" << std::endl;
    std::cout << "The key:" << endl;
    std::cout << sndDataFeatures_->key << endl;
    std::cout << endl;
    std::cout <<  sndDataFeatures_->cs.audacity_textspur(1, false) << std::endl;
    std::cout << "*************************************************************" << std::endl << std::endl;
    cout << endl << "(" << tagsum << ") result: " << resultTag << endl;



    resultInfo.push_back("***** Key of the song with Neural Network *****");
    resultInfo.push_back(QString::fromStdString(tags_[maxIndex]));
    resultInfo.push_back(QString::fromStdString(resultTag));
    resultInfo.push_back("***********************************************");
    resultInfo.push_back("***** Key of the song with Neural Network *****");
    resultInfo.push_back("The key:");
    resultInfo.push_back(QString::fromStdString(sndDataFeatures_->key.tostring()));
    resultInfo.push_back("");
    resultInfo.push_back(QString::fromStdString(sndDataFeatures_->cs.audacity_textspur(1, false)));
    resultInfo.push_back("***********************************************");


    info(resultInfo);

    /*
    cout << "a: " << a_string;
    cout << "| \t\t";
    for(int i = 0; i < a_string; i++)
        cout << "#";
    cout << endl;

    cout << "am: " << am_string << endl;
    cout << "| \t\t";
    for(int i = 0; i < am_string; i++)
        cout << "#";
    cout << endl;

    cout << "bm: " << bm_string << endl;
    cout << "| \t\t";
    for(int i = 0; i < bm_string; i++)
        cout << "#";
    cout << endl;

    cout << "c: " << c_string << endl;
    cout << "d: " << d_string << endl;
    cout << "dm: " << dm_string << endl;
    cout << "e: " << e_string << endl;
    cout << "em: " << em_string << endl;
    cout << "f: " << f_string << endl;
    cout << "a: " << g_string << endl;
*/
}

void MainWindow::on_reInitNeuralNetwork_clicked()
{
    if(neuralnetworks_ != NULL)
    {
        std::cout << "delete sndDataFeatures_" << endl;
        neuralnetworks_ = NULL;

        QMessageBox mb;
        mb.setIcon(QMessageBox::Information);
        mb.setText("Neural Network");
        mb.setInformativeText("Törölve");
        mb.exec();
        ui->neuralNetworklabel->setText("Deleted");
        ui->neuralNetworklabel->setStyleSheet("QLabel { color: Blue }");
        train_ = false;
        ui->numHiddenLayer_label->setText("0");
        ui->numNeuron_label->setText("0");
        ui->learningrate_label->setText("0");
        ui->trainCycle_label->setText("0");


    }
    else
    {
        QMessageBox mb;
        mb.setIcon(QMessageBox::Information);
        mb.setText("Neural Network");
        mb.setInformativeText("Nincs mit törölni.");
        mb.exec();
        ui->neuralNetworklabel->setText("Nothing happened");
        ui->neuralNetworklabel->setStyleSheet("QLabel { color: Blue }");
    }
}

//////////////////////////////////////////////////////
/// Read dev directory!
//////////////////////////////////////////////////////
void MainWindow::on_readDir_dirDevel_clicked()
{
    if(fileOperator_->openDir(dev_database_))
    {
        ui->dbSuccessLabel_2->setText("OK");
        ui->dbSuccessLabel_2->setStyleSheet("QLabel { color: green }");
        sampleDBRead_ = true;


        dev_databaseFeatures_->clear();
        for(auto a : dev_database_)
        {
            dev_databaseFeatures_->push_back(new GetFeatures(a, false));
        }
        qDebug() << dev_databaseFeatures_->size() << " piece of features calculated";
    }
    else
    {
        ui->dbSuccessLabel_2->setText("ERROR");
        ui->dbSuccessLabel_2->setStyleSheet("QLabel { color: red }");
        sampleDBRead_ = false;
    }
}


void MainWindow::dataBaseDev()
{


    int tagsum = 0;
    int db = 0;
    // D.B. teaching
    for(int i = 0; i < dev_databaseFeatures_->size(); i++)
    {
        cout << endl << endl << endl <<"i : " << i << endl;


        std::vector<double> input;


        for(int j = 0; j < databaseFeatures_->at(i)->pcptrack.size(); j++)
        {
            for(auto pcp : databaseFeatures_->at(i)->pcptrack.at(j).pcp)
            {
                input.push_back((double)pcp);
            }

            /*
            cout << "input size: " << input.size() << endl;
            for(auto x : input)
                cout << x << ", ";
            cout << endl;
            */
            //cout << "update input" << endl;
            neuralnetworks_->updateInputs(input);

            //cout << "compute outputs" << endl;
            neuralnetworks_->computeOutputs();


            cout << "tag:" << databaseFeatures_->at(i)->dbTag <<" - output tag:"<< neuralnetworks_->getTag() << endl;
            //qDebug() << QString::fromStdString(nn->getTag());


            if(databaseFeatures_->at(i)->dbTag == neuralnetworks_->getTag())
            {
                tagsum++;
            }
            db++;

            input.clear();

        }
    }

    cout << endl << "dev database done" << endl;
    //cout << "Percent: " << (tagsum/dev_databaseFeatures_->size()) << endl;
    cout << "Ok: " << tagsum << endl
            << " All pieace: " << db << endl
            << " dev_databaseFeatures_->size(): " << dev_databaseFeatures_->size() << endl;

    QVector<QString> resultInfo;
    resultInfo.push_back(QString::fromStdString(sndData_->waveFileName()));
    resultInfo.push_back("dev database done");
    resultInfo.push_back("Ok: "+QString::fromStdString(std::to_string(tagsum)));
    resultInfo.push_back("All pieace: "+QString::fromStdString(std::to_string(db)));
    resultInfo.push_back("dev_databaseFeatures_->size(): "+QString::fromStdString(std::to_string(dev_databaseFeatures_->size())));

    info(resultInfo);

}


//////////////////////////////////////////////////////
/// Read dev directory! <<<<<<<<< EEEENNDDD
//////////////////////////////////////////////////////

void MainWindow::on_switch_Button_clicked()
{
    flip = !flip;
    if(flip)
    {
        ui->switch_label->setText("use file");
        ui->switch_label->setStyleSheet("QLabel { color: green }");
    }
    else
    {
        ui->switch_label->setText("use directory");
        ui->switch_label->setStyleSheet("QLabel { color: blue }");
    }



}

void MainWindow::on_simple_Button_clicked()
{
    bool ok = false;
    int numInput = 10;
    int numOutput = 3;
    int numHiddenLayer = 1;
    int numHiddenNeuron = 5;
    double learningrate = 0.001;


    /// tag init
    std::vector<std::string> tag;
    tag.clear();
    tag.push_back("a");
    tag.push_back("b");
    tag.push_back("c");

    std::vector<double> input;
    input.push_back(1.0);
    input.push_back(0.0);
    input.push_back(0.0);

    std::vector<double> target;
    target.clear();
    target.push_back(1.0);
    target.push_back(0.0);
    target.push_back(0.0);

    neuralnetworks_ = new NeuralNetworks(input.size(), tag.size(), numHiddenLayer,numHiddenNeuron,learningrate);
    neuralnetworks_->setTag(tag);

    for(int i = 0; i < 10; i++)
    {
        cout << "update input" << endl;
        neuralnetworks_->updateInputs(input);
        cout << "compute outputs" << endl;
        neuralnetworks_->computeOutputs();
        cout << "update error signal" << endl;
        neuralnetworks_->updateErrorSignal(target);
        cout << "update weights" << endl;
        neuralnetworks_->updateWeights();

        cout <<endl<< "outputs:"<<endl;
        for(auto o : neuralnetworks_->outputs())
        {
            cout << o << ", ";
        }
        cout <<endl;
        cout << "tag:" << "a" <<" - output tag:"<< neuralnetworks_->getTag();
    }

}
