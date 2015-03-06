#ifndef GETFEATURES_H
#define GETFEATURES_H

#include "../IOMusicHandler/sounddata.h"
#include "../../genchords/src/FFT.h"
#include "../Utils/neuralutilities.h"
// http://www.fftw.org/download.html
#include <fftw3.h>

// Key from genchords
#include "../../genchords/src/Key.h"
#include "../../genchords/src/utilities.h"

/*
class GetFeatures
{
public:
    GetFeatures();
    ~GetFeatures();
};*/

bool getFeatures(SoundData*& sndData);

#endif // GETFEATURES_H
