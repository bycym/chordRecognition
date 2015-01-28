#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

#include "sys/types.h"
#include "dirent.h"
#include "sys/stat.h"
#include "unistd.h" // for getopt


#include "sndfile.h"
//#include "../lib-src/libsndfile/src/sndfile.h"

#include "genchords.h"
#include "PCP.h"
#include "PitchName.h"
#include "Chordsequence.h"
#include "Sounddata.h"
#include "utilities.h"
#include "BeatDetection.h"
#include "Key.h"



void dispatch(void);
void printmenu(void);

void printhelp(void) {
    std::cout << ("genchords -a PCPAlgoNr -b beatfile|ms -k -n numchords -o -p -v level -w windowsize inputfile\n");
	std::cout << 
            "\t-a PCPAlgoNr" \
			":\tchose the algorithm that is used to convert the spectrum to a PCP. " \
			"1: one peak per mountain; 2: all peaks; 3: integrate" 
			<< std::endl <<
            "\t-b Beatfile|MillisecondsPerBeat" \
			":\t if the argument is an integer it is used as the span between two chords. " \
            "\telse take the timespan of the chords from the specified beatfile." \
            " Beatfile must have the time in seconds on each line. Defaults to 500"
			<< std::endl <<			
			"\t-o" \
			":\t optimize chord sequence to have less changes "
			<< std::endl <<					
			"\t-p" \
			":\t print the Probability of each chord"
			<< std::endl <<					
			"\t-n Numchords" \
			":\t the x best chords will be printed for each time period to the labelfile. default=1 (only the best)"
			<< std::endl <<			
            "\t-w Windowsize" \
            ":\t Windowsize of the autocorrelation. Must be a power of 2." \
			" The smaller the window size, the less low frequencies will be detected. Defaults to 2048"
			<< std::endl <<
			"\t-k"\
			":\tcompute the key of the song, and only use chords that corrspond to this key."
			<< std::endl <<
			"\t-h"\
			":\tprint this help"
			<< std::endl <<
			"\t-v level"\
			":\tverbosity level. 1=labelfile, 2=scorefile, 4=pcpfile. default=1"			
			<< std::endl;
	
}

void usage(void) {
	printhelp();
	//exit(EXIT_FAILURE);
}

// verbosity falgs
const int LABELFILE = 1;
const int SCOREFILE = 2;
const int PCPFILE   = 4;

// command options and arguments
bool oKey = false;
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


void genchords(std::string inputfile) {
	
	int ifilename = 0;
	if (inputfile.rfind("/")!= (std::string::npos)) {
		ifilename = inputfile.rfind("/");
	}
	std::string basename = inputfile.substr(ifilename,inputfile.rfind(".")-ifilename) ;
	
	std::cout << "basename: " << basename << std::endl;
	std::string ofscore = outdir + basename + ".sco";
	std::string oflabel = outdir + basename + ".txt";
	std::string ofpcp = outdir + basename + ".pcp";
	std::string ofbeat = outdir + basename + ".beat.txt";
	
	std::cout << "Creating Sounddata..." << std::endl;
	Sounddata sd(inputfile);

    if (windowSize==0) {
        windowSize = idealWindowSize(sd.samplerate());
	}
	
	Key key;
	if (oKey) {
        key = getkey(sd,windowSize);
		std::cout << "Computed Key: " << key << std::endl;
	}
	
	
    if (oGenerateBeatfile) {
		std::cout << "Detecting Beats..." << std::endl;
        if (oGenerateBeatfile == 1) {
			std::cout << "\t using beatroot" << std::endl;
			std::string beatcommand = "java -jar lib-src/beatroot/beatroot-0.5.3.jar -b -o " + ofbeat + " " + inputfile;
			/*if (system(beatcommand.c_str()) == -1) {
				std::cerr << "Genchords: Failed to execute beatcommand:" << std::endl << beatcommand << std::endl;
				//exit(EXIT_FAILURE);
				return 1;
			}*/
		}
		else {
			float * processed = new float[(sd.frames()/2048)];
			int countbeats=0;
            if (oGenerateBeatfile == 2) {
				countbeats = FSE(2048, sd.samplerate(), sd.srcbuffer(), sd.frames(), processed);
			} else {
				countbeats = SSE(2048, sd.samplerate(), sd.srcbuffer(), sd.frames(), processed, true);
			}
			savestring(printBeatStructure(processed, countbeats), ofbeat);
			delete[] processed;
		}
	}
	
	std::cout << "Creating PCPTrack..." << std::endl;
	PCPTrack pt;
    if (oBeatfile) {
        pt = PCPTrack(sd, pcpalgo,  beatfile, windowSize);
    } else if (oGenerateBeatfile) {
        pt = PCPTrack(sd, pcpalgo, ofbeat, windowSize);
	} else {
        pt = PCPTrack(sd, pcpalgo, blockSizeMSec, true, windowSize);
	}	
	
	std::cout << pt.size() << std::endl;
	
	if (verbositylevel & PCPFILE) {
		std::cout << "Creating PCP-File " << ofpcp << "..." << std::endl;
		savestring(pt.tostring(), ofpcp);
	}
	
	if (verbositylevel & (LABELFILE | SCOREFILE)) {
	
		std::cout << "Creating Chordsequence..." << std::endl;
		Chordsequence cs = Chordsequence(pt, key);
		if (oOptimize) {
			cs = cs.optimize();
		}
		
		
		if (verbositylevel & LABELFILE) {
			std::cout << "Creating Audacity labelfile " << oflabel << "..." << std::endl;
			savestring(cs.audacity_textspur(number_of_chords, oProbability), oflabel);
		}
		
		if (verbositylevel & SCOREFILE) {
			std::cout << "Creating CSound scorefile " << ofscore << "..." << std::endl;
			cs.csound_sco(ofscore);
			std::cout << std::endl <<
					"To create a wav file of the chords issue the following command: " << std::endl <<
					"src/scripts/chordmix.sh \"" << ofscore << "\" \"" << inputfile << "\" ." << std::endl;
		}
	}
}


std::string summary() {
	std::ostringstream ost;
	ost << "files\t" << argfile << std::endl;
	ost << "key\t" << oKey << std::endl;
	ost << "optimize\t" << oOptimize << std::endl;
    ost << "pcpalgo\t" << pcpalgo << std::endl;
    if (not oBeatfile)	ost << "beat\t" << blockSizeMSec  << std::endl;
    else ost << "beatfile\t" << beatfile << std::endl;
	ost << "numchords\t" << number_of_chords << std::endl;
	ost << "outdir\t" << outdir << std::endl;
    ost << "windowsize\t" << windowSize << std::endl;
	return ost.str();
}

		
int main(int argc, char* argv[]) {	
	
	
	
	//c und c++ ausgaben sollen vermischt werden können
	std::cout.sync_with_stdio();
	
	
	/* *************************************************************** */
	/* ********************** ARGUMENTBEHANDLUNG ********************* */
	/* *************************************************************** */
	
	// falls kein argument uebergeben wurde arbeite interaktiv
	if (argc==1) {
		dispatch();
		return 0;
	}
	
	int c;
	bool optError=false;
    int tempBlockSizeMSec = 0;
	while ((c=getopt(argc, argv, "a:b:d:kw:n:ophv:")) != EOF) {
		char *ep;
		switch(c) {
			case 'a':
                pcpalgo =  (int)strtol(optarg, &ep, 10);
                if (*ep!='\0' || pcpalgo < 1 || pcpalgo > 3) {
					std::cerr << "invalid algorithm." << optarg << "." << std::endl;
					usage();
				}
				break;
			case 'b':
                tempBlockSizeMSec =  (int)strtol(optarg, &ep, 10);
				if (*ep!='\0') {
                    oBeatfile = true;
                    beatfile = optarg;
                } else if (tempBlockSizeMSec < 100) {
                    oGenerateBeatfile = tempBlockSizeMSec;
				} else {
                    blockSizeMSec = tempBlockSizeMSec;
				}			
				break;
			case 'd':
				extern int UTILITIES_DEBUGLEVEL;
				UTILITIES_DEBUGLEVEL =  (int)strtol(optarg, &ep, 10); // set extern variable
				if (*ep!='\0') {
					std::cerr << "invalid debuglevel." << optarg << "." << std::endl;
					usage();
				}
				break;
				
			case 'h':
				printhelp();
				return 0;
				break;
			case 'k':
				oKey = true;
				break;
			case 'o':
				oOptimize = true;
				break;
			case 'p':
				oProbability = true;
				break;
			case 'w':
                windowSize =  (int)strtol(optarg, &ep, 10);
				if (*ep!='\0') {
					std::cerr << "invalid window size ." << optarg << "." << std::endl;
					usage();
				}
				break;
			case 'n':
				number_of_chords =  (int)strtol(optarg, &ep, 10);
				if (*ep!='\0') {
					std::cerr << "invalid number of chords to output ." << optarg << "." << std::endl;
					usage();
				}
				break;
			case 'v':
				verbositylevel = (int)strtol(optarg, &ep, 10);
				if (*ep!='\0') {
					std::cerr << "invalid verbosity level ." << optarg << "." << std::endl;
					usage();
				}
				break;
			case '?':
				optError = true;
		}
	}
	
	// ein oder zwei argumente
	if (optError || optind+2<argc || optind+1>argc) {
		usage();
	}

	argfile = argv[optind];
	if (optind+2<= argc) {
		outdir = argv[optind+1];
	}
	
	/* *************************************************************** */
	/* ******************* ENDE ARGUMENTBEHANDLUNG ******************* */
	/* *************************************************************** */
	
	
		
	DIR* pDir;
	if ((pDir = opendir(argfile.c_str())) == 0) {
		/* ein file */
		genchords(argfile);
		std::cout << summary() << std::endl;
	}
	else {
		/* alle Files im directory */
        std::string beatdir = beatfile;
		struct dirent* pDirEntry;
		while ((pDirEntry = readdir(pDir)) != 0) {
			// Skip the current and the parent directory
			if (strcmp(pDirEntry->d_name, ".") == 0) continue;
			if (strcmp(pDirEntry->d_name, "..") == 0) continue;
			std::string soundfile = pDirEntry->d_name;
			int index = soundfile.rfind(".");
			if (index > 0 && ( soundfile.substr(index) == ".wav" || soundfile.substr(index) == ".mp3")) {
                if (oBeatfile) {
                    beatfile = soundfile;
                    beatfile.replace(index, std::string::npos, ".txt");
                    beatfile = beatdir + "/" + beatfile;
				}
				std::cout << "######################## " << soundfile << "############################" << std::endl;
				genchords(argfile + "/" + soundfile);
				std::cout << "########################################################################" << std::endl;
			}
		}	
		// save summary information
		savestring(summary(), outdir + "/log.log");
					
	}
	
	return 0;	
}

void outdated(std::map<std::string, bool> & uptodate) {
	uptodate["PCPTrack"] = false;
	uptodate["Sounddata"] = false;
	uptodate["Chordsequence"] = false;
}

std::string loadfile() {
	std::string filename;
	std::ifstream inf;
	std::cout << "filename: ";
	std::cin >> filename;
	inf.open(filename.c_str());
	while (!inf) {
		std::cout << filename << " does not exist." << "New filename: ";
		std::cin >> filename;
		inf.clear();
		if (inf.is_open()) {
			inf.close();
		}
		inf.open(filename.c_str());
	}
	inf.close();
	std::cout << "File loaded successfully" << std::endl << std::endl;
	return filename;	
}

void dispatch() {
	std::string soundfilename = "";
	
	Sounddata sd;
	Key key;
	PCPTrack pcptrack;	
	Chordsequence cs;
	
	std::map<std::string, bool> uptodate;
	// initialisiere uptodate-map mit false werten
	outdated(uptodate);
	
	// init options
	std::set<std::string> chordOptions;
	chordOptions.insert("chord");
	chordOptions.insert("p&c");
	chordOptions.insert("labelfile");
	chordOptions.insert("f");
	chordOptions.insert("s");
	chordOptions.insert("scorefile");
	chordOptions.insert("key");
	chordOptions.insert("k");

	std::set<std::string> pcptrackOptions = chordOptions;
	pcptrackOptions.insert("pcp");
	
	std::set<std::string> sounddataOptions = pcptrackOptions;
	sounddataOptions.insert("getsample");
	
	
	
	soundfilename = loadfile();
	
	printmenu();
	std::string option = "";
	
	while (true) {
				
		std::cin >> option;
		
		// sounddata updaten falls noetig
		if (not uptodate["Sounddata"] && sounddataOptions.find(option)!=sounddataOptions.end()) {
			std::cout << "...init sounddata..." << std::endl;
			sd.init(soundfilename);
            windowSize = idealWindowSize(sd.samplerate());
			uptodate["Sounddata"] = true;	
		}
		if (not uptodate["PCPTrack"] && pcptrackOptions.find(option) != pcptrackOptions.end()) {
			std::cout << "...init pcptrack..." << std::endl;
            pcptrack = PCPTrack(sd, pcpalgo, blockSizeMSec, true, windowSize);
			uptodate["PCPTrack"] = true;
		}				
		if (not uptodate["Chordsequence"] && chordOptions.find(option) != chordOptions.end()) {
			std::cout << "...init chordsequence..." << std::endl;
            key = getkey(sd,windowSize);
			cs = Chordsequence(pcptrack, key);
			uptodate["Chordsequence"] = true;
		}				
		
		
		if (option =="m" || option =="menu") {
			printmenu();
		}
		else if (option == "l" | option == "load") {
			soundfilename = loadfile();
			//std::cout << "block size in Milliseconds (optional)";
            //std::cin >> blockSizeMSec;
			outdated(uptodate);
		}
		else if (option == "i" || option == "info") {
			std::cout << soundfilename << std::endl;
			printSfInfo(soundfilename);
		}		
		else if (option == "k" | option == "key") {
			std::cout << "********* Key of the song ************" << std::endl;
			std::cout <<  key << std::endl;
			std::cout << "*********************************" << std::endl << std::endl;	
		}
		
		else if (option == "f" || option == "labelfile") {
			std::string filename = "";
			std::cout << "outputfilename: ";
			std::cin >> filename;	
			
			savestring(cs.audacity_textspur(1, false), filename);
			std::cout << "output written to file " << filename << std::endl << std::endl;	
			std::cout << "*********************************" << std::endl << std::endl;	
		}
		else if (option == "pcp" || option == "chord" || option == "p&c" || option == "getsample") {
			long nr;
			int sum;
			std::cout << "start block: ";
			std::cin >> nr;
			std::cout << "nr of blocks: ";
			std::cin >> sum;
			if (sum < 1) sum = 1;
			for (int i=0; i<sum; i++) {
				if (option == "pcp" or option == "p&c") {
					std::cout << "PCP[" << nr+i << " = " << pcptrack.startTime(nr+i) << "ms ] = " << std::endl;
					std::cout << pcptrack.at(nr+i).bargraph();
					std::cout << pcptrack.at(nr+i).tostring();
				}
				if (option == "chord" or option == "p&c") {
					ChordItem ci = cs.at(nr+i);
					if (ci.mCps.size()>0) {
						std::cout << "Chord[" << nr+i <<"] = " << ci.mCps.at(0).mChord << std::endl;
					} else std::cout << "no chord recognized" << std::endl;
				}
				if (option == "getsample") {
					std::cout << "Sample[" << nr+i << "] = " << sd.getsample(nr+i) << std::endl;
				}
			}
			std::cout << std::endl;
		}
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
		else if (option == "s" || option == "scorefile") {
			std::string filename = "";
			std::cout << "outputfilename: ";
			std::cin >> filename;
			//std::cout << "restrict chordtypes to key (yes/.*)";
			//std::string restrict;
			//std::cin >> restrict;
			// create input file for autocomp
			//Chordsequence cs = Chordsequence(pcptrack, (restrict=="yes")? key : -1);
			cs.csound_sco(filename);
			std::cout << "output written to file " << filename << "[.sco|.wav]" 
					<< std::endl << std::endl;	
			std::cout << "*********************************" << std::endl << std::endl;	
		}
		else if (option == "q" or option == "quit") {
			std::cout << "good bye ...." << std::endl << std::endl;	
			break;
		}
		else {
			std::cout << "unknown option " << option << std::endl <<
					"Press 'm' to see the menu" <<  std::endl;
		}
	}
	std::cout << std::endl;
}

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

void debugm(char * mg) {
  printf("%s\n", mg);
  fflush(stdout);
}

