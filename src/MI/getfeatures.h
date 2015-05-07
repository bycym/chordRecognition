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
#include "../../genchords/src/PCP.h"
#include "../../genchords/src/BeatDetection.h"
#include "../../genchords/src/Chordsequence.h"
#include "../../genchords/src/Chordtype.h"
#include <set>
#include <map>




class GetFeatures
{
public:
    /*!
     * \brief GetFeatures
     * PCP, Key, Chordsequence calculator
     * \param sndData input data
     * \param sample is id database element or the sample
     * sample = true, it's a sample
     * sample = false, it's a database element
     */
    GetFeatures(SoundData*& sndData, bool sample);
    ~GetFeatures();

    int windowSize;
    Key key;
    PCPTrack pcptrack;
    Chordsequence cs;
    int pcpalgo;
    int blockSizeMSec;
    std::string dbTag;


};

bool getFeatures_OLD(SoundData*& sndData);

#endif // GETFEATURES_H
