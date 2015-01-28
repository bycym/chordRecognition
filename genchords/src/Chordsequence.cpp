//
// C++ Implementation: Chordsequence
//
// Description: 
//
//
// Author: Veronika Zenz <zenz@snoopy-tm>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include "PitchName.h"
#include "Chordsequence.h"


Chordsequence::Chordsequence(const PCPTrack & pcptrack, Key key) {
	PCP p;
	mTime = pcptrack.getblockSizeMs();	
	mKey = key;
	bool onlykeychords = (key.root>=0);
	
	std::vector<PCPItem> pcpVec = pcptrack.getPCPVec();
	std::vector<ChordProb> possiblechords;
	std::vector<ChordProb> cps;
	// nimm nur jene akkordtypen die der tonart des stueckes entsprechen
	if (onlykeychords) {
		possiblechords = keyChords(key); 
	}
	 
	float boostroot=1; // 1.05 
	for (unsigned int i=0; i<pcpVec.size(); i++) {
		p = pcpVec[i].mPCP;
		// p.printGraph();	
		if (onlykeychords) {
			cps = guessChord(p, possiblechords, boostroot);
		}
		else {
			cps = guessChord(p, standardChordtypes(), boostroot);
		}
		// std::cout << c << std::endl;
		if (cps.size()==0) std::cout << "no chords recognized at time " << pcpVec[i].mStartMs << std::endl;
		ChordItem ci = ChordItem(pcpVec[i].mStartMs, cps);
		mChords.push_back(ci);
	}
}

Chordsequence Chordsequence::optimize() {
	int neighbours = 2;
	int num = 5;
	
	// create permutation
	std::vector<std::vector<int> > permutation;
	std::vector<std::vector<int> > newpermutation;
	for (int x=0; x<num; x++) {
		std::vector<int> chain;
		chain.push_back(x);
		permutation.push_back(chain);
	}
	for (int i=0; i<neighbours*2; i++) {
		for (unsigned int p=0; p<permutation.size(); p++) {
			for (int x=0; x<num; x++) {
				std::vector<int> chain = permutation[p];
				chain.push_back(x);
				newpermutation.push_back(chain);
			}
		}
		permutation = newpermutation;
		newpermutation.clear();
	}
	
// 	// print permutation
// 	for (unsigned int p=0; p<permutation.size(); p++) {
// 		for (unsigned int j=0; j<permutation[p].size(); j++) {
// 			std::cout << permutation[p][j] << " ";
// 		}
// 		std::cout << std::endl;
// 	}
	std::cout << std::endl << permutation.size() << " " << nofelements() << std::endl;
	
	
	
	Chordsequence opt;
	opt.setKey(mKey);
	for (int i=0; i<neighbours; i++) {
		opt.add(at(i));
	}
	
	
	
	int start = neighbours;
	for (unsigned int i=0; i<nofelements()-(neighbours*2); i++) {
// 		std::cout << i << std::endl;
		float acousticProbMax=0;
		int bestindex=0;
		float acousticProb = 0;
		for (unsigned int p=0; p<permutation.size(); p++) {
			std::vector<int> chain = permutation[p];
			float totalprob =1;
			int countChanges = 0;
			Chord oldchord;
			// compute total probability
			for (unsigned int e=0; e<chain.size(); e++) {
				totalprob *= at(i+e, chain[e]).mProbability;
				if (e!=0 && at(i+e, chain[e]).mChord != oldchord) {
					countChanges++;
				}			
				oldchord = at(i+e, chain[e]).mChord;
			}
			acousticProb = totalprob * pow(0.9, countChanges);
					
// 			std::cout << p << "\t" << totalprob << "\t" << countChanges << std::endl;
			
			if (p==0) {
// 				std::cout << p << "\t" << totalprob << "\t" << countChanges << std::endl;
// 				for (unsigned int e=0; e<chain.size(); e++) {
// 					std::cout << at(i+e, chain[e]).mChord << " (" << at(i+e, chain[e]).mProbability << ") ";
// 				}
// 				std::cout << std::endl;
				
				acousticProbMax = acousticProb;
				if (countChanges==0) {
					// take this one
// 					std::cout << " take first one " << std::endl;
					break;
				}
			}
			if (acousticProb > acousticProbMax) {
// // 				for (unsigned int e=0; e<chain.size(); e++) {
// 						std::cout << at(i+e, chain[e]).mChord << " (" << at(i+e, chain[e]).mProbability << ") ";
// 				}
// 				std::cout << std::endl;
// 				std::cout << "!" << p << "\t" << totalprob << "\t" << countChanges << std::endl;
// 				std::cout << "!" << "taken" << std::endl;
				bestindex = p;
				acousticProbMax = acousticProb;
			}
			
		}
// 		std::cout << at(i+start, permutation[bestindex][neighbours]).mChord << " " << maxprob << " " << countChanges;
// 		for (int e=0; e<permutation[bestindex].size(); e++) {
// 			std::cout << permutation[bestindex][e] << " ";
// 		}
		
		Chord c = at(start, permutation[bestindex][neighbours]).mChord;
		if (permutation[bestindex][neighbours] != 0) {
			std::cout << timeAt(start) << " changed from " << getBest(start).mChord << " to " << c << "(" << permutation[bestindex][neighbours] << ".) bestindex= " << bestindex <<std::endl;
		}
		opt.add(ChordItem(timeAt(start), c));
//  		std::cout << std::endl << std::endl << std::endl;
		start++;	
	}
	
	for (unsigned int i=nofelements()-neighbours; i<nofelements(); i++) {
		opt.add(at(i));
	}
	
	return opt;
	
}


/* num specifies how many chords per time period shall be printed.
if num=1 only the best chord will be printed
num = x>0 print x best chords
if num=0 print all chords
*/
std::string Chordsequence::audacity_textspur(unsigned int num, bool showprobability) const{
	Chord oldc;
	Chord newc;
	std::ostringstream ost;

	bool flats = (mKey.root>=0 && flatkey(mKey.root));
	if (mKey.root >= 0)	ost << "0\t" << mKey << std::endl;
	else ost << "0\t" << "-1" << std::endl;
	
	
	
	for (unsigned int i=0; i<mChords.size(); i++)	{	
		newc = getBest(i).mChord;
		if (newc!=oldc || num !=1) {	
			ost << std::fixed << std::setprecision(3) << mChords[i].mStartMs/1000 << "\t";
			ost << newc.fullname(flats);
			for (unsigned int ci = 1; 
						  ci<mChords[i].mCps.size() && ((ci<num || num==0) 
								  //|| (mChords[i].mCps[ci-1].mProbability - mChords[i].mCps[ci].mProbability) < 0.1
													   ); 
						  ci++) {
				ost << "/" << mChords[i].mCps[ci].mChord.fullname(flats);
			}
			if (showprobability) {
				ost << " %% " << mChords[i].mCps[0].mProbability ;
				for (unsigned int ci = 1; ci<mChords[i].mCps.size() && (ci<num || num==0); ci++) {
					ost << "/" << mChords[i].mCps[ci].mProbability;
				}
			}
			ost << std::endl;
			oldc = newc;
		}
	}
	// audacity liest die zeit in sekunden ein wobei kommstelle ein "," seind muss. Da . sonst nicht vorkommt
	// alle punkte durch beistrich ersetzen
	std::string out = ost.str();
	unsigned int pos = out.npos;
	while ((pos=out.find("."))!=out.npos) {out.replace(pos, 1, ","); }
	return out;
}

void Chordsequence::csound_sco(std::string & filename) const{
	
	std::ofstream fout(filename.c_str());
	
	// taken from autocomp.
	fout << "; Wavetables for piano" << 				std::endl <<
			"f 7 0 32768 10 1" <<						std::endl <<
			"f 8 0 256 1 \"src/scripts/fwavblnk.aiff\" 0 0 0" <<std::endl << std::endl;
			

	fout << ";ins\tstart\tlen\tamp\tfreq" <<std::endl;
	Chord c;

	float start = 0;
	int maj[3] = {0,4,7};
	int min[3] = {0,3,7};
	int * t;
	for (unsigned int i=0; i<mChords.size()-1; i++) {
		// falls an der stelle i ein neuer akkord -> gib alten akkord c aus
		c = getBest(i).mChord;
		int root = c.getroot();
		float length = mChords[i+1].mStartMs/1000 - mChords[i].mStartMs/1000;
		t = min;
		if (getBest(i).mChord.getname() == "Maj" || getBest(i).mChord.getname() == "Maj7") {
			t = maj;
		}
		for (int j=0; j<3; j++) {
			root = (c.getroot() + t[j]) % PCPLEN;
			int octave = 8 + (c.getroot() + t[j] >= PCPLEN);
			fout << "i1\t" << std::fixed << std::setprecision(2) << start <<"\t"<< length <<"\t" << int(3600.0/(j+1)) << "\t"<<
					octave << "." << (root<10? "0":"") << root
					<< "\t;" << c <<std::endl;
		}
		start += length;
	}
	fout << std::endl << "e" << std::endl;
	fout.close();
}

void Chordsequence::autocomp_txt(std::string & filename) const{
	std::ofstream fout(filename.c_str());
	
	fout << "BPM:" << 60000/mTime << std::endl << std::endl;
	
	for (unsigned int i=0; i<mChords.size(); i++) {
		if (i%4 == 0 && i != 0) fout << "| " << std::endl;
		fout << mChords[i].mCps.at(0).mChord << " ";
	}
	fout << "| ";
	fout.close();
}
