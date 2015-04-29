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
    GetFeatures(SoundData*& sndData);
    ~GetFeatures();

    int windowSize;
    Key key;
    PCPTrack pcptrack;
    Chordsequence cs;
    int pcpalgo;
    int blockSizeMSec;



};

bool getFeatures_OLD(SoundData*& sndData);

#endif // GETFEATURES_H
