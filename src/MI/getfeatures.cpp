#include "getfeatures.h"

void printmenu(void) {
    std::cout << "************* Menu ****************" << std::endl;
    std::cout << "(l)oad" << "\t" << "load a new song." << std::endl;
    std::cout << "(i)nfo" << "\t" << "print song information (length, channels, ...)" << std::endl;
    std::cout << "(k)ey" << "\t" << "print key of the song" << std::endl;
    std::cout << "getsample" << "\t" << "print a sample value from the audio data" << std::endl;
    std::cout << "pcp" << "\t" << "print the Pitch Class Profile of a sequence of blocks" << std::endl;
    std::cout << "chord" << "\t" << "print the Chord of a sequence of blocks" << std::endl;
    std::cout << "p&c" << "\t" << "print PCP and Chord of a sequence of blocks" << std::endl;
    std::cout << "label(f)ile"  << "\t" <<
            "write chords in a label file that can be imported to audacity" << std::endl;
    std::cout << "(s)corefile" << "\t" << "write chords to a scorefile that can be processed by csound"
             << std::endl;
    std::cout << "(m)enu" << "\t" << "print the menu" << std::endl;
    std::cout << "(q)uit" << "\t" << "quit the program" << std::endl;
    std::cout << std::endl;
    std::cout << "*********************************" << std::endl << std::endl;
}

void outdated(std::map<std::string, bool> & uptodate) {
    uptodate["PCPTrack"] = false;
    uptodate["Sounddata"] = false;
    uptodate["Chordsequence"] = false;
}


GetFeatures::~GetFeatures()
{

}

GetFeatures::GetFeatures(SoundData*& sd, bool sample)
{

    // verbosity falgs
    const int LABELFILE = 1;
    const int SCOREFILE = 2;
    const int PCPFILE   = 4;

    // command options and arguments
    bool oKey = false;
    bool oBeatfile = false;
    std::string beatfile;
    int oGenerateBeatfile = 0;
    bool oOptimize = false;
    bool oProbability = false;
    unsigned int number_of_chords = 1; // default output 1 chord
    int verbositylevel = 1;
    //std::string outdir = "tmp/";
    std::string outdir = "";
    std::string argfile;

    // database tag
    this->dbTag = sd->dbTag();
    windowSize = 0;
    blockSizeMSec = 100;
    pcpalgo=2;


    std::string basename = sd->waveFileName() ;

        std::cout << "basename: " << basename << std::endl;
        std::string ofscore = outdir + basename + ".sco";
        std::string oflabel = outdir + basename + ".txt";
        std::string ofpcp = outdir + basename + ".pcp";
        std::string ofbeat = outdir + basename + ".beat.txt";



    float * processed = new float[(sd->frames()/2048)];
    int countbeats=0;

    //soundfilename = loadfile();


    //printmenu();
    std::string option = "";

    while (true) {

        //std::cin >> option;

        // sounddata updaten falls noetig
        //if (not uptodate["Sounddata"] && sounddataOptions.find(option)!=sounddataOptions.end()) {
            std::cout << "...init sounddata..." << std::endl;
            //sd.init(soundfilename);

            windowSize = idealWindowSize(sd->samplerate());
            //uptodate["Sounddata"] = true;
        //}
        //if (not uptodate["PCPTrack"] && pcptrackOptions.find(option) != pcptrackOptions.end()) {
            std::cout << "...init pcptrack..." << std::endl;
            pcptrack = PCPTrack(sd, pcpalgo, blockSizeMSec, true, windowSize);
            //uptodate["PCPTrack"] = true;
        //}
        //if (not uptodate["Chordsequence"] && chordOptions.find(option) != chordOptions.end()) {

            if(sample){
                std::cout << "...init chordsequence..." << std::endl;
                key = getkey(sd,windowSize);
                cs = Chordsequence(pcptrack, key);


                std::cout << "************ Key of the song ************" << std::endl;
                std::cout <<  key << std::endl;
                std::cout << "*****************************************" << std::endl << std::endl;
/*
                std::string filename = "";
                std::cout << "outputfilename: ";
                filename = filename+sd->waveFileName()+"labelfile";

                savestring(cs.audacity_textspur(1, false), filename);
                std::cout << "output written to file " << filename << std::endl << std::endl;
                std::cout << "*********************************" << std::endl << std::endl;
*/
            }



/*
            std::cout << "...beat detection..." << std::endl;
            countbeats = SSE(2048, sd->samplerate(), sd->audio_data_f_.data(), sd->frames(), processed, true);
            cout << "SSE countbeats: " << countbeats << endl;
            savestring(printBeatStructure(processed, countbeats), ofbeat);
            delete[] processed;*/

            /*else if (option == "i" || option == "info") {
            std::cout << soundfilename << std::endl;
            printSfInfo(soundfilename);
        }*/
        //else if (option == "k" | option == "key") {

/*
            std::cout << "********* Key of the song ************" << std::endl;
            std::cout <<  key << std::endl;
            std::cout << "*********************************" << std::endl << std::endl;
        //}
*/



        /*
        if (option == "f" || option == "labelfile") {
            std::string filename = "";
            std::cout << "outputfilename: ";
            filename = filename+sd->waveFileName()+"labelfile";

            savestring(cs.audacity_textspur(1, false), filename);
            std::cout << "output written to file " << filename << std::endl << std::endl;
            std::cout << "*********************************" << std::endl << std::endl;
        }
        */



        //else if (option == "pcp" || option == "chord" || option == "p&c" || option == "getsample") {
            long nr;
            int sum;
            nr = 0; sum = pcptrack.size();
            //std::cout << "PCPTrack size (windows): " << pcptrack.size() << endl;

            /// pcptrack.at(i).tostring()
            /*
            cout << "pcptrack.at(i)" << endl;
            for(int i = 0; i < pcptrack.size(); i++)
            {
                cout << pcptrack.at(i).tostring() << endl;
            }

            */



            /// pcp bargraph
            /*

            for (int i=0; i<sum; i++) {
               // if (option == "pcp" or option == "p&c") {
                    //std::cout << "nr("<<nr<<") + i("<<i<<") = " << nr+i << endl;
                    std::cout << "PCP[" << nr+i << " = " << pcptrack.startTime(nr+i) << "ms ] = " << std::endl;
                    std::cout << pcptrack.at(nr+i).bargraph();
                    std::cout << pcptrack.at(nr+i).tostring();
               // }
               // if (option == "chord" or option == "p&c") {
                    ChordItem ci = cs.at(nr+i);
                    if (ci.mCps.size()>0) {
                        std::cout << "Chord[" << nr+i <<"] = " << ci.mCps.at(0).mChord << std::endl;
                  } else std::cout << "no chord recognized" << std::endl;
                //}
                //if (option == "getsample") {
                    //std::cout << "Sample[" << nr+i << "] = " << sd->getsample(nr+i) << std::endl;
                    std::cout << "i: " << i << endl;
                    std::cout << "Sample[" << nr+i << "] = " << sd->getSample(nr+i) << std::endl;
               // }//
            }
            std::cout << std::endl;


            */








        //}
        /*
            else if (option == "c" || option == "autocomp") {
                std::string filename = "";
                std::cout << "outputfilename: ";
                std::cin >> filename;
                // create input file for autocomp
                csK.autocomp_txt(filename);
                std::cout << "output written to file " << filename << "[.sco|.wav]"
                        << std::endl << std::endl;
                std::cout << "*********************************" << std::endl << std::endl;
            }
            */
        //else if (option == "s" || option == "scorefile") {












            /*
            std::string filename = "";
            std::cout << "outputfilename: ";
            //std::cin >> filename;
            filename = filename + sd->waveFileName() + ".TMP.wav";
            //std::cout << "restrict chordtypes to key (yes/.*)";
            //std::string restrict;
            //std::cin >> restrict;
            // create input file for autocomp
            //Chordsequence cs = Chordsequence(pcptrack, (restrict=="yes")? key : -1);

            //cs.csound_sco(filename);
            std::cout << "output written to file " << filename << "[.sco|.wav]"
                      << std::endl << std::endl;
            std::cout << "*********************************" << std::endl << std::endl;
        //}
        //else if (option == "q" or option == "quit") {




            */




//            std::cout << "good bye ...." << std::endl << std::endl;
            delete[] processed;

       //     break;
        //}
        //else {
         //   std::cout << "unknown option " << option << std::endl <<
          //               "Press 'm' to see the menu" <<  std::endl;
        //}
            break;
    }
    //std::cout << std::endl;
}


/*
bool getFeatures_OLD(SoundData*& sd)
{


    const int LABELFILE = 1;
    const int SCOREFILE = 2;
    const int PCPFILE   = 4;

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


    std::string basename = sd->waveFileName();
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
        cout << "FSE countbeats: " << countbeats << endl;
    } else {    
        countbeats = SSE(2048, sd->samplerate(), sd->audio_data_f_.data(), sd->frames(), processed, true);
        cout << "SSE countbeats: " << countbeats << endl;
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


*/
