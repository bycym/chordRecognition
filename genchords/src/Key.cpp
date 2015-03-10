//
// C++ Implementation: Key
//
// Description: 
//
//
// Author: Veronika Zenz <veronika.zenz@google.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
// Modyfied: Áron Benkóczy <benkoczyaron@gmail.com> 2015
//

#include <iostream>

#include "PCP.h"
#include "Chordtype.h"
#include "Key.h"
#include "utilities.h"



std::ostream& operator<< (std::ostream & s, const Key & k) {
	return s << k.tostring();
};

/* *************************
 * computes the key of the whole song, 
 * assumes that there are no modulations.
 *
 * **************************/
/// own SoundData
// Key getkey(Sounddata & sd, int windowSize) {
Key getkey(SoundData *& sd, int windowSize) {
// 	PCP p = PCP(mSrcBuffer,
// 				mFrames,
// 				windowSize,
// 				mSamplerate,
// 				1, // algorithm
// 				1000 // Bloecke von 1 Sekunde
// 			   );
    int samplerate = sd->samplerate();
    long frames = sd->frames();

    const float * srcBuffer = sd->audio_data_f_.data();
    /*for(int i = 0; i < sd->audio_data_f_.size(); i++)
    {
        cout << "s: " << srcBuffer[i] << endl;
    }*/



	int frontlength=30; // laenge die am anfang und ende des stueckes genommen werden soll in sekunden
	PCP p;
	if (frames>=samplerate*frontlength*2) {	
		PCP p1 = PCP(srcBuffer, samplerate*frontlength, windowSize, samplerate, 2, 5000);
        std::cout << p1.bargraph();
        std::cout << p1.tostring();
		PCP p2 = PCP(srcBuffer + frames - samplerate*frontlength, samplerate*frontlength, windowSize, samplerate, 2, 5000);
        std::cout << p2.bargraph();
        std::cout << p2.tostring();
		normalize(p1.pcp, PCPLEN);
		normalize(p2.pcp, PCPLEN);	
		float fpcp[PCPLEN];
		float countPeaks[PCPLEN];
		for (int i=0; i<PCPLEN; i++) {
			fpcp[i] = p1.pcp[i] + p2.pcp[i];
			countPeaks[i] = p1.pcpCountPeaks[i] + p2.pcpCountPeaks[i];
		}
		p = PCP(fpcp, countPeaks);
	}
	else {
		p = PCP(srcBuffer, frames, windowSize, samplerate, 2, 5000);
	}
	
	
	
	std::cout << p.bargraph();
	std::cout << p.tostring();


	// MEINE EIGENEN
	//               	 C          D         E    F         G         A         H
	float dur[PCPLEN] = {1.0, 0.0, 0.6, 0.0, 0.7, 0.6, 0.0, 0.9, 0.0, 0.6, 0.0, 0.6};
	float moll[PCPLEN]= {1.0, 0.0, 0.6, 0.8, 0.0, 0.5, 0.0, 0.9, 0.6, 0.0, 0.7, 0.3};
	
	
// 	// KRUMHANSL
// 	//               	    C             D            E        F             G             A            H
// 	//float dur[PCPLEN] = {6.500, 2.300, 3.500, 2.400, 4.500, 4.300, 2.500, 5.300, 2.400, 3.600, 2.30, 2.90};
// 	//float moll[PCPLEN]= {6.4, 2.6, 3.5, 5.5, 2.5, 3.5, 2.4, 4.7, 4.0, 2.6, 3.3, 3.0};
// 	float dur[PCPLEN]  = {1.000, 0.000, 0.290, 0.024, 0.520, 0.480, 0.048, 0.710, 0.024, 0.310, 0.00, 0.14};
// 	float moll[PCPLEN] = {1.000, 0.050, 0.270, 0.770, 0.024, 0.270, 0.000, 0.570, 0.400, 0.050, 0.22, 0.15};
	// 	
	
	
	
	normalize(dur, 12);
	normalize(moll, 12);
	
	PCP pcp_dur = PCP(dur);	
	PCP pcp_moll = PCP(moll);


	std::vector<Chordtype> keychordtypes;
	keychordtypes.push_back(Chordtype(pcp_dur, "dur"));
	keychordtypes.push_back(Chordtype(pcp_moll, "moll"));
	std::vector<ChordProb> cps = guessChord(p, keychordtypes, 1.05, 3);
	
	
	for (unsigned int i=0; i<cps.size(); i++) {
		std::cout << cps[i].mChord << " " << cps[i].mProbability << std::endl;
	}
	if (cps.size()>0) {
		return Key(cps[0].mChord.getroot(), cps[0].mChord.getname());
// 		std::cout << cps[0].mChord << std::endl;
// 		// bei moll gib die korrespondierende durtonart zurueck (a-moll -> C)
// 		if (cps[0].mChord.getname() == "moll") return cps[0].mChord.getroot()+3;
// 		else return cps[0].mChord.getroot();
    }
	return Key();
}

/***
 *
 * Original getkey(..)
 *

Key getkey(Sounddata & sd, int windowSize) {
// 	PCP p = PCP(mSrcBuffer,
// 				mFrames,
// 				windowSize,
// 				mSamplerate,
// 				1, // algorithm
// 				1000 // Bloecke von 1 Sekunde
// 			   );

    int samplerate = sd.samplerate();
    long frames = sd.frames();
    const float * srcBuffer = sd.srcbuffer();

    int frontlength=30; // laenge die am anfang und ende des stueckes genommen werden soll in sekunden
    PCP p;
    if (frames>=samplerate*frontlength*2) {
        PCP p1 = PCP(srcBuffer, samplerate*frontlength, windowSize, samplerate, 2, 5000);
// 		std::cout << p1.bargraph();
// 		std::cout << p1.tostring();
        PCP p2 = PCP(srcBuffer + frames - samplerate*frontlength, samplerate*frontlength, windowSize, samplerate, 2, 5000);
// 		std::cout << p2.bargraph();
// 		std::cout << p2.tostring();
        normalize(p1.pcp, PCPLEN);
        normalize(p2.pcp, PCPLEN);
        float fpcp[PCPLEN];
        float countPeaks[PCPLEN];
        for (int i=0; i<PCPLEN; i++) {
            fpcp[i] = p1.pcp[i] + p2.pcp[i];
            countPeaks[i] = p1.pcpCountPeaks[i] + p2.pcpCountPeaks[i];
        }
        p = PCP(fpcp, countPeaks);
    }
    else {
        p = PCP(srcBuffer, frames, windowSize, samplerate, 2, 5000);
    }



    std::cout << p.bargraph();
    std::cout << p.tostring();


    // MEINE EIGENEN
    //               	 C          D         E    F         G         A         H
    float dur[PCPLEN] = {1.0, 0.0, 0.6, 0.0, 0.7, 0.6, 0.0, 0.9, 0.0, 0.6, 0.0, 0.6};
    float moll[PCPLEN]= {1.0, 0.0, 0.6, 0.8, 0.0, 0.5, 0.0, 0.9, 0.6, 0.0, 0.7, 0.3};


// 	// KRUMHANSL
// 	//               	    C             D            E        F             G             A            H
// 	//float dur[PCPLEN] = {6.500, 2.300, 3.500, 2.400, 4.500, 4.300, 2.500, 5.300, 2.400, 3.600, 2.30, 2.90};
// 	//float moll[PCPLEN]= {6.4, 2.6, 3.5, 5.5, 2.5, 3.5, 2.4, 4.7, 4.0, 2.6, 3.3, 3.0};
// 	float dur[PCPLEN]  = {1.000, 0.000, 0.290, 0.024, 0.520, 0.480, 0.048, 0.710, 0.024, 0.310, 0.00, 0.14};
// 	float moll[PCPLEN] = {1.000, 0.050, 0.270, 0.770, 0.024, 0.270, 0.000, 0.570, 0.400, 0.050, 0.22, 0.15};
    //



    normalize(dur, 12);
    normalize(moll, 12);

    PCP pcp_dur = PCP(dur);
    PCP pcp_moll = PCP(moll);


    std::vector<Chordtype> keychordtypes;
    keychordtypes.push_back(Chordtype(pcp_dur, "dur"));
    keychordtypes.push_back(Chordtype(pcp_moll, "moll"));
    std::vector<ChordProb> cps = guessChord(p, keychordtypes, 1.05, 3);


    for (unsigned int i=0; i<cps.size(); i++) {
        std::cout << cps[i].mChord << " " << cps[i].mProbability << std::endl;
    }
    if (cps.size()>0) {
        return Key(cps[0].mChord.getroot(), cps[0].mChord.getname());
// 		std::cout << cps[0].mChord << std::endl;
// 		// bei moll gib die korrespondierende durtonart zurueck (a-moll -> C)
// 		if (cps[0].mChord.getname() == "moll") return cps[0].mChord.getroot()+3;
// 		else return cps[0].mChord.getroot();
    }
    return Key();
}
*/

