#include <iostream>
#include <sstream>
#include <unistd.h> // for getopt
#include <dirent.h> // for reading directories
#include <map>


#include "Sounddata.h"
#include "PCP.h"
#include "Chordsequence.h"
#include "utilities.h"

#include <stdlib.h>
#include <string.h>
#include <cstdio>

void usage(void) {
    std::cout << "learnchords -a ALGONR -b BEAT[FILE|DIR] LABEL[FILE|DIR] WAV[FILE|DIR]" << std::endl;
    exit(EXIT_FAILURE);
}


PCPM total_ctm;

// command options and arguments
bool oBeatfile = false;
std::string beatfile;
int blockSizeMSec = 100;
int windowSize = 0;
int pcpalgo=1;


void learnchords(std::string soundfile, std::string labelfile) {

	std::cout << "Creating Sounddata..." << std::endl;
	Sounddata sd(soundfile);

	if (windowSize==0) {
		windowSize = idealWindowSize(sd.samplerate());
	}
	
	
	std::cout << "Creating PCPTrack..." << std::endl;
	PCPTrack pt;
    if (oBeatfile) {
        pt = PCPTrack(sd, pcpalgo,  beatfile, windowSize);
	} else {
		pt = PCPTrack(sd, pcpalgo, blockSizeMSec, true, windowSize);
	}	
		

	std::cout << "Learn chords... " << std::endl;
	PCPM ctm = learn(pt, labelfile);
	
	typedef PCPM::iterator CI;
	for (CI p = ctm.begin(); p!=ctm.end(); ++p) {
		std::cout << p->first <<  std::endl;
		std::cout << (p->second).bargraph() << std::endl;
		std::cout << (p->second).tostring() << std::endl;
		total_ctm[p->first].addPCP((p->second), 0);
	}
	
	
}

/*
int main(int argc, char* argv[]) {


	
	int c;
	bool optError=false;
    int tempBlockSizeMSec = 0;
	while ((c=getopt(argc, argv, "a:b:d:kw:n:phv:")) != EOF) {
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
					std::cerr << "blockSize " << tempBlockSizeMSec << " not supported" << std::endl;
				} else {
					blockSizeMSec = tempBlockSizeMSec;
				}			
				break;
			case 'w':
				windowSize =  (int)strtol(optarg, &ep, 10);
				if (*ep!='\0') {
					std::cerr << "invalid window size ." << optarg << "." << std::endl;
                    usage();
				}
				break;
			case '?':
				optError = true;
		}
	}
	
	// genau zwei argumente
	if (optError || optind+2!=argc) {
        usage();
	}

	std::string labelfile = argv[optind];
	std::string soundfile = argv[optind+1];
	
		
	DIR* pDir;
	if ((pDir = opendir(soundfile.c_str())) == 0) {
        // ein file
		learnchords(soundfile, labelfile);
	}
	else {
		
		// save summary information
		std::ostringstream ost;
		ost << "files\t" << soundfile << std::endl;
		ost << "pcpalgo\t" << pcpalgo << std::endl;
        ost << "beatfile\t" << beatfile << std::endl;
		ost << "labelfile\t" << labelfile << std::endl;
		ost << "windowsize\t" << windowSize << std::endl;
		std::cout << ost.str() << std::endl;
		
		std::string sounddir = soundfile;
        // alle Files im directory
        std::string beatdir = beatfile;
		std::string labeldir = labelfile;
		struct dirent* pDirEntry;
		while ((pDirEntry = readdir(pDir)) != 0) {
			// Skip the current and the parent directory
			if (strcmp(pDirEntry->d_name, ".") == 0) continue;
			if (strcmp(pDirEntry->d_name, "..") == 0) continue;
			soundfile = pDirEntry->d_name;
			int index = soundfile.rfind(".");
			if (index > 0 && ( soundfile.substr(index) == ".wav" || soundfile.substr(index) == ".mp3")) {
				labelfile = soundfile;
				labelfile.replace(index, std::string::npos, ".txt");
                if (oBeatfile) {
                    beatfile = beatdir + "/" + labelfile;
				}
				labelfile = labeldir + "/" + labelfile;
				std::cout << "######################## " << soundfile << "############################" << std::endl;
				learnchords(sounddir + "/" + soundfile, labelfile);
				std::cout << "########################################################################" << std::endl;
			}
		}	
	
		std::cout << std::endl << std::endl << std::endl;
		std::cout << "########################################################################" << std::endl;
		std::cout << "##############################  TOTAL ##################################" << std::endl;
		std::cout << "########################################################################" << std::endl;
		typedef PCPM::iterator CI;
		for (CI p = total_ctm.begin(); p!=total_ctm.end(); ++p) {
			normalize((p->second).pcp, PCPLEN);
			std::cout << p->first <<  std::endl;
			std::cout << (p->second).bargraph() << std::endl;
			std::cout << (p->second).tostring() << std::endl;
		}				
	}
	
	return(EXIT_SUCCESS);	
	
	
}
*/
