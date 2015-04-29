#include "neuralnetworkform.h"
#include "ui_neuralnetworkform.h"

NeuralNetworkForm::NeuralNetworkForm(QWidget *parent) :
    QDialog(parent),
    ui_nnform(new Ui::NeuralNetworkForm)
{

    int numInput = 10;
    int numOutput = 10;
    int numHiddenLayer = 1;
    int numHiddenNeuron = 35;
    double learningrate = 0.001;
/*
    ui_nnform->numInput_lineEdit->setText(QString::number(numInput));
    ui_nnform->numOutput_lineEdit->setText(QString::number(numOutput));
    ui_nnform->numHiddenLayer_lineEdit->setText(QString::number(numHiddenLayer));
    ui_nnform->numHiddenLayer_lineEdit->setText(QString::number(numHiddenNeuron));
  */
    //ui_nnform->learningrate_lineEdit->setText(QString::number("0.001"));

    ui_nnform->setupUi(this);

}

NeuralNetworkForm::~NeuralNetworkForm()
{
    delete ui_nnform;
}

int NeuralNetworkForm::numInput()
{
    if(!ui_nnform->numInput_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numInput_lineEdit->text().toInt(&ok);
        if(ok)
            return result;
        else{
            qDebug() << "Bad value numInput";
            return -1;
        }
    }
    qDebug() << "Empty numInput";
    return -1;

}

double NeuralNetworkForm::learningrate()
{
    if(!ui_nnform->learningrate_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->learningrate_lineEdit->text().toInt(&ok);
        if(ok)
            return result;
        else{
            qDebug() << "Bad value learningrate";
            return -1;
        }
    }
    qDebug() << "Empty learningrate";
    return -1;

}

int NeuralNetworkForm::numOutput()
{
    if(!ui_nnform->numOutput_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numOutput_lineEdit->text().toInt(&ok);
        if(ok)
            return result;
        else{
            qDebug() << "Bad value numOutput";
            return -1;
        }
    }
    qDebug() << "Empty numOutput";
    return -1;

}

int NeuralNetworkForm::numHiddenLayer()
{
    if(!ui_nnform->numHiddenLayer_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numHiddenLayer_lineEdit->text().toInt(&ok);
        if(ok)
            return result;
        else{
            qDebug() << "Bad value numHiddenLayer";
            return -1;
        }
    }
    qDebug() << "Empty numHiddenLayer";
    return -1;

}

int NeuralNetworkForm::numHiddenNeuron()
{
    if(!ui_nnform->numNeuron_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numNeuron_lineEdit->text().toInt(&ok);
        if(ok)
            return result;
        else{
            qDebug() << "Bad value numHiddenNeuron";
            return -1;
        }
    }
    qDebug() << "Empty numHiddenNeuron";
    return -1;

}

/*!
 * \brief NeuralNetworkForm::on_buttonBox_accepted
 * if some parameter is empty use default value
 */
void NeuralNetworkForm::on_buttonBox_accepted()
{

    /*
    int numInput = 10;
    int numOutput = 10;
    int numHiddenLayer = 1;
    int numHiddenNeuron = 35;
    double learningrate = 0.001;
*/

    int numInput;
    int numOutput;
    int numHiddenLayer;
    int numHiddenNeuron;
    double learningrate;

    // number of input
    if(!ui_nnform->numInput_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numInput_lineEdit->text().toInt(&ok);
        if(ok)
            numInput = result;
        else{
            qDebug() << "Bad value numInput";
        }
    }
    else
        qDebug() << "Empty numInput";

    // number of output
    if(!ui_nnform->numOutput_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numOutput_lineEdit->text().toInt(&ok);
        if(ok)
            numOutput = result;
        else{
            qDebug() << "Bad value numOutput";
        }
    }
    else
        qDebug() << "Empty numOutput";

    // number of hidden layer
    if(!ui_nnform->numHiddenLayer_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numHiddenLayer_lineEdit->text().toInt(&ok);
        if(ok)
            numHiddenLayer = result;
        else{
            qDebug() << "Bad value numHiddenLayer";
        }
    }
    else
        qDebug() << "Empty numHiddenNeuron";

    //number of neuron
    if(!ui_nnform->numNeuron_lineEdit->text().isEmpty())
    {
        bool ok = false;
        int result = ui_nnform->numNeuron_lineEdit->text().toInt(&ok);
        if(ok)
            numHiddenNeuron = result;
        else{
            qDebug() << "Bad value numHiddenNeuron";
        }
    }
    else
        qDebug() << "Empty numNeuron";


    // learning rate
    if(!ui_nnform->learningrate_lineEdit->text().isEmpty())
    {
        bool ok = false;
        double result = ui_nnform->learningrate_lineEdit->text().toDouble(&ok);
        if(ok)
            learningrate = result;
        else{
            qDebug() << "Bad value learningrate";
        }
    }
    else
        qDebug() << "Empty learningrate";

    if(!ui_nnform->numInput_lineEdit->text().isEmpty())
    {
        emit this->setParameters(numInput, numOutput, numHiddenLayer, numHiddenNeuron, learningrate);
    }
    else
    {
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Nem sikerült létrehozni.");
        mb.setInformativeText("Legalább a numInputot meg kell adnod!");
        mb.exec();
    }
}
