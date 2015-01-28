#ifndef __GENCHORDS__CHORDTYPE_H__
#define __GENCHORDS__CHORDTYPE_H__

/* ******************************
 * genchords - Chordtype Class
 *
 * ******************************/

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "PCP.h"
#include "PitchName.h"
#include "Key.h"

class Chordtype {

	public:
		Chordtype(){};
		Chordtype(int *intervals, int intervallength, std::string nname, int root=0);
		Chordtype(const PCP & npcp, const std::string & nname);
  
  	// return the chord shifted to the root root
		PCP getpcp(int root=0) const;
		
		std::string tostring() const;
		
		std::string name;  	
		

	private:
  // characteristic intervals
		PCP pcp;
};



class Chord {
	public:
		Chord(const Chordtype & ct, int root) {
			this->ct = ct;
			this->root = root;
		}
		
		Chord() {};
		
		
		int getroot() const {return root;}
		Chordtype getchordtype() const {return ct;}
		std::string getname() const {return ct.name;}
		std::vector<int> getcontainedpitchnums() const;
		std::string fullname(bool flats = false) const {
			return PitchName(root, flats) + ct.name;
		}
		
		// compares the name of the cordtype, not the ct itself!!
		bool operator==(const Chord & c2) const {
			return (root==c2.getroot()) && (ct.name.substr(0,3)==c2.ct.name.substr(0,3));
		}
		bool operator!=(const Chord & c2) const {
			return not this->operator==(c2);
		}
		
		std::string tostring() const{
			return std::string("") + PitchName(root) + ct.name;
		};
		
		
	private:
		int root;
		Chordtype ct;				
};


class ChordProb{
	public:
		ChordProb(const float probability, const Chord & chord): mProbability(probability) {mChord=chord;}
		float mProbability;
		Chord mChord;
};




std::ostream& operator<< (std::ostream & s, const Chord & c);


// creates a vector of standard chordtypes (major, minor)
std::vector<Chordtype> standardChordtypes();

// creates a vector of chords that are commonly used in a song with key keyChords
// example: if key is C Chords are (CMaj, FMaj, GMaj, Dmin, Emin, Amin)
std::vector<ChordProb> keyChords(Key key);



std::vector<ChordProb> guessChord(PCP pcp, std::vector<Chordtype> chordtypes, float boostroot, unsigned int num=8);


std::vector<ChordProb> guessChord(PCP pcp, std::vector<Chord> & chords, float boostroot, unsigned int num=8);
std::vector<ChordProb> guessChord(PCP pcp, std::vector<ChordProb> & chords, float boostroot, unsigned int num=8);

typedef std::map< std::string, PCP > PCPM;

PCPM learn(PCPTrack pt, std::string labelfile);
#endif



