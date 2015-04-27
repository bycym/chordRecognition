#include "neuralnetworkform.h"
#include "ui_neuralnetworkform.h"

NeuralNetworkForm::NeuralNetworkForm(QWidget *parent) :
    QDialog(parent),
    ui_nnform(new Ui::NeuralNetworkForm)
{
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
