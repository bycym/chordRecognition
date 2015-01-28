//
// C++ Interface: Chordsequence
//
// Description: 
//
//
// Author: Veronika Zenz <zenz@snoopy-tm>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __GENCHORDS__CHORDSEQUENCE_H__
#define __GENCHORDS__CHORDSEQUENCE_H__

#include "vector"
#include "string"
#include "Chordtype.h"

class ChordItem {
	public:
		ChordItem(const float startMs, const std::vector<ChordProb>  & cps): mStartMs(startMs), mCps(cps) {};
		ChordItem(const float startMs, const Chord c) {
			mStartMs = startMs;
			mCps.push_back(ChordProb(1, c));
		}
		float mStartMs; // start time in milliseconds
		std::vector<ChordProb> mCps;
};

class Chordsequence {
	public:
		Chordsequence() { mChords = std::vector<ChordItem>();};
		Chordsequence(int time) { mChords = std::vector<ChordItem>(); mTime = time;};
		Chordsequence(const PCPTrack & pcptrack, Key key);
		
		void add(ChordItem c) {
			mChords.push_back(c);
		};
		void setKey(Key k) {
			mKey = k;
		}
		
		Chordsequence optimize() ;
		
		/* output functions */
		std::string audacity_textspur(unsigned int num, bool showprobability) const;
		void csound_sco(std::string & filename) const;
		void autocomp_txt(std::string & filename) const;
		unsigned int nofelements() const {return mChords.size();}
		ChordItem at(const int index) const {return mChords.at(index); }
		ChordProb at(const int index, const int num) const{
			return mChords.at(index).mCps.at(num);
		};
		float timeAt(const int index) const {return mChords.at(index).mStartMs;};
		ChordProb getBest(const int index) const {return mChords.at(index).mCps.at(0); }
		
	private:
		std::vector<ChordItem> mChords;
		int mTime; // time between two chords in milliseconds
		Key mKey; // Keyindex. -1 means unknown
};

#endif
