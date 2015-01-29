#include "fileoperator.h"

FileOperator::FileOperator(QWidget *parent) :
    QWidget(parent)
{
    fileName = "NO.DATA";
}

FileOperator::~FileOperator()
{
    delete sndData;
}

void FileOperator::init()
{

}

bool FileOperator::open()
{
    QString path = QDir::homePath();
    if(fileName != ""){
        QFileInfo info(fileName);
        path = info.absolutePath();
    }
    QString fn = QFileDialog::getOpenFileName(
                this,
                "Fájl megnyitása...",
                path,
                "Hangfájl (*.wav);;Mindenfájl (*.*)"
                );
    bool success;
    if(!fn.isEmpty())
        success = performLoadOperation(fn);
    return success;
}

bool FileOperator::performLoadOperation(QString fn)
{

    bool success = false;

    if(sndData != NULL){
        sndData->init(fn);
    }
    else{
        sndData = new Sounddata(fn);
    }

    if(sndData->frames()>0){
        success = true;
        cout << "frames: " << sndData->frames() << endl;
        cout << "samplerate: " << sndData->samplerate() << endl;
    }

    if(success)
        fileName = fn;
    else{
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Nem sikerült a megnyitás.");
        mb.setInformativeText("Próbálj valami okosat tenni.");
        mb.exec();
    }
    return success;
}
