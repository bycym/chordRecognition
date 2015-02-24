#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "../IOMusicHandler/fileoperator.h"
#include "../IOMusicHandler/sounddata.h"
#include <fftw3.h>


class Core : public QObject
{
    Q_OBJECT
private:

public:
    explicit Core(QObject *parent = 0);
    ~Core();



signals:

public slots:
};

#endif // CORE_H

