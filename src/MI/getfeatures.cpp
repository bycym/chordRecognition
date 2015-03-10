#include "getfeatures.h"

bool getFeatures(SoundData*& sd)
{

    // command options and arguments
    bool oKey = true; //false;
    bool oBeatfile = false;
    std::string beatfile;
    int oGenerateBeatfile = 0;
    int blockSizeMSec = 100;
    int windowSize = 0;
    bool oOptimize = false;
    bool oProbability = false;
    unsigned int number_of_chords = 1; // default output 1 chord
    int verbositylevel = 1;
    int pcpalgo=3;
    std::string outdir = "tmp/";
    std::string argfile;



    std::string basename = "sample.wav";
    std::cout << "basename: " << basename << std::endl;
        std::string ofscore = outdir + basename + ".sco";
        std::string oflabel = outdir + basename + ".txt";
        std::string ofpcp = outdir + basename + ".pcp";
        //std::string ofbeat = outdir + basename + ".beat.txt";
        std::string ofbeat = basename + ".beat.txt";

    if (windowSize==0) {
            windowSize = idealWindowSize(sd->nAvgBytesPerSec());
            cout << "windowsSize: " << windowSize << endl;
    }

    Key key;
    /*** TODO!!!!! */
    if (oKey) {
        key = getkey(sd,windowSize);
        std::cout << "Computed Key: " << key << std::endl;
    }


    // the dimension of the pointers in and out
    //int N = 2;
    int N = 2;



    float * processed = new float[(sd->frames()/2048)];
    int countbeats=0;
    if (oGenerateBeatfile == 2) {
        countbeats = FSE(2048, sd->samplerate(), sd->audio_data_f_.data(), sd->frames(), processed);
    } else {
        countbeats = SSE(2048, sd->samplerate(), sd->audio_data_f_.data(), sd->frames(), processed, true);
    }
    savestring(printBeatStructure(processed, countbeats), ofbeat);
    delete[] processed;

    std::cout << "Creating PCPTrack..." << std::endl;
    PCPTrack pt;
    if (oBeatfile) {
        pt = PCPTrack(sd, pcpalgo,  beatfile, windowSize);
    } else if (oGenerateBeatfile) {
        pt = PCPTrack(sd, pcpalgo, ofbeat, windowSize);
    } else {
        pt = PCPTrack(sd, pcpalgo, blockSizeMSec, true, windowSize);
    }

    std::cout << "pt.size() = " <<pt.size() << std::endl;




    return false;
}


