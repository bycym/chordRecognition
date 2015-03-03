#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>


#include "Chordtype.h"
#include "PCP.h"
#include "utilities.h"
#include "Labelfile.h"


// //                          c         d         e    f         g         a    bb
// const float pcp_maj[12] = {1.0, 0.0, 0.0, 0.0, 0.7, 0.0, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0};
// const float pcp_min[12] = {1.0, 0.0, 0.0, 0.7, 0.0, 0.0, 0.0, 0.9, 0.0, 0.0, 0.0, 0.0};
// const float pcp_maj7[12] = {1.0, 0.0, 0.0, 0.0, 0.7, 0.0, 0.0, 0.9, 0.0, 0.0, 1.0, 0.0};
// const float pcp_min7[12] = {1.0, 0.0, 0.0, 0.7, 0.0, 0.0, 0.0, 0.9, 0.0, 0.0, 1.0, 0.0};

// // learnchords 11.11.2006
// //                           c           d          e     f          g           a     bb
// const float pcp_maj[12] =  {1.0, 0.00, 0.03, 0.04, 0.30, 0.17, 0.0, 0.36, 0.02, 0.15, 0.06, 0.02};
// const float pcp_min[12] =  {1.0, 0.11, 0.13, 0.30, 0.08, 0.26, 0.09, 0.34, 0.38, 0.0, 0.06, 0.17};
// const float pcp_maj7[12] = {1.0, 0.00, 0.03, 0.04, 0.30, 0.17, 0.0, 0.36, 0.02, 0.15, 0.46, 0.2};
// const float pcp_min7[12] = {1.0, 0.11, 0.13, 0.30, 0.08, 0.26, 0.09, 0.34, 0.38, 0.0, 0.46, 0.2};

//                           c           d          e     f           g           a     bb
const float pcp_maj[12] =  {1.0, 0.00, 0.05, 0.05, 0.24, 0.15, 0.01, 0.39, 0.02, 0.16, 0.00, 0.02}; // 129
const float pcp_min[12] =  {1.0, 0.00, 0.05, 0.30, 0.03, 0.14, 0.04, 0.26, 0.25, 0.00, 0.00, 0.02}; //129
const float pcp_maj7[12] =  {1.0, 0.00, 0.05, 0.05, 0.24, 0.15, 0.01, 0.39, 0.02, 0.16, 0.46, 0.02}; // 129
const float pcp_min7[12] =  {1.0, 0.00, 0.05, 0.30, 0.03, 0.14, 0.04, 0.26, 0.25, 0.00, 0.46, 0.02}; //129





const Chordtype g_maj = Chordtype(PCP(pcp_maj), "Maj");
const Chordtype g_maj7 = Chordtype(PCP(pcp_maj7), "Maj7");
const Chordtype g_min = Chordtype(PCP(pcp_min), "Min");
const Chordtype g_min7 = Chordtype(PCP(pcp_min7), "Min7");

Chordtype::Chordtype(int *intervals, int intervallength, std::string nname, int root) {

  float fpcp [PCPLEN];
  for (int i=0; i<PCPLEN; i++) {
      fpcp[i]=0;
  }
  for (int i=0; i<intervallength; i++) {
    fpcp[intervals[i]] = 1;
  }

  pcp = PCP(fpcp);
  name = nname;
}

Chordtype::Chordtype(const PCP & npcp, const std::string & nname) {
    pcp = npcp;
    name = nname;
}

std::ostream& operator<< (std::ostream & s, const Chord & c) {
    return s << PitchName(c.getroot()) << c.getchordtype().name;
}


// return the chord shifted to the root root
PCP Chordtype::getpcp(int root) const {
    if (root == 0) {
        return pcp;
    }

    float fpcp[PCPLEN];
    for (int i=0; i<PCPLEN; i++) {
        fpcp[(i+root)%PCPLEN] = pcp.pcp[i];
    }
    return PCP(fpcp);
}

std::string Chordtype::tostring() const {
    std::ostringstream ost;
    ost << name << std::endl << pcp.tostring();
    return ost.str();
}

std::vector<int> Chord::getcontainedpitchnums() const {
    std::vector<int> pitchnums;
    PCP pcp = ct.getpcp(root);
    for (int i=0; i<PCPLEN; i++) {
        if (pcp.pcp[i]) pitchnums.push_back(i);
    }
    return pitchnums;
}

std::vector<Chordtype> standardChordtypes() {
    std::vector<Chordtype> chordtypes;
    chordtypes.push_back(g_maj);
    chordtypes.push_back(g_min);
    chordtypes.push_back(g_maj7);
    chordtypes.push_back(g_min7);

  return chordtypes;
}

std::vector<ChordProb> keyChords(Key key) {
    std::vector<ChordProb> chords;
    int root = key.root;
    if (key.type == "moll") {
        root = key.root + 3;
    }
    chords.push_back(ChordProb(1 ,Chord(g_min, (root + 2) % PCPLEN))); //d
    chords.push_back(ChordProb(1 ,Chord(g_min, (root + 4) % PCPLEN))); //e
    chords.push_back(ChordProb(1 ,Chord(g_min, (root + 9) % PCPLEN))); //a

    chords.push_back(ChordProb(1 ,Chord(g_maj, (root + 0) % PCPLEN))); //C
    chords.push_back(ChordProb(1 ,Chord(g_maj, (root + 4) % PCPLEN))); //F
    chords.push_back(ChordProb(1 ,Chord(g_maj, (root + 7) % PCPLEN))); //G
    chords.push_back(ChordProb(1 ,Chord(g_maj7, (root + 7) % PCPLEN))); //G7

    chords.push_back(ChordProb(1 ,Chord(g_maj, (root + 2) % PCPLEN))); //D
    chords.push_back(ChordProb(0.98 ,Chord(g_maj, (root + 9) % PCPLEN))); //A
    chords.push_back(ChordProb(1 ,Chord(g_maj, (root + 5) % PCPLEN))); //E
    chords.push_back(ChordProb(0.99 ,Chord(g_maj, (root + 10) % PCPLEN))); //Bb

    return chords;
}


std::vector<ChordProb> guessChord(PCP pcp, std::vector<Chordtype>  chordtypes, float boostroot, unsigned int num) {

    std::vector<Chord> chords;
    for (unsigned int i=0; i<chordtypes.size(); i++) {
        for (int root=0; root<PCPLEN; root++) {
            chords.push_back(Chord(chordtypes[i], root));
        }
    }

    return guessChord(pcp, chords, boostroot, num);
}

std::vector<ChordProb> guessChord(PCP pcp, std::vector<Chord> & chords, float boostroot, unsigned int num) {
    std::vector<ChordProb> cp;
    for (unsigned int i=0; i<chords.size(); i++) {
        cp.push_back(ChordProb(1, chords[i]));
    }
    return guessChord(pcp, cp, boostroot, num);
}


std::vector<ChordProb> guessChord(PCP pcp, std::vector<ChordProb> & chords, float boostroot, unsigned int num) {

    float maxdistance = 12; // bei linearer distanz und normalisierten vektoren
    // maxdistance = distance({1}*12, {0}*12)
    std::vector<ChordProb> vecCp;

    // normalisiere pcp
    float fpcp [12];
    for (int i=0; i<PCPLEN; i++) {
        fpcp[i] = pcp.pcp[i];
        fpcp[i] = pcp.pcp[i]*(pcp.pcpCountPeaks[i]+1);
    }
    normalize(fpcp, PCPLEN);

    // speichere die pitchnumber des staerksten tons
    //std::cout << std::endl << pcp.tostring()<< std::endl;
    int strongest = 0;
    for (int p=0; p<PCPLEN; p++) {
        if (fpcp[p]>fpcp[strongest]) {
            strongest = p;
        }
    }
    //std::cout << strongest << std::endl;


    for (unsigned int i=0; i<chords.size(); i++) {
        Chord c = chords[i].mChord;
        debugm(c.tostring(), 15);
        float dist = distance(fpcp, c.getchordtype().getpcp(c.getroot()).pcp, PCPLEN);
        float probability = (100.0*(maxdistance - dist))/maxdistance;

        probability*=chords[i].mProbability;

        // falls der staerkste ton = grundton ist, probability um boostroot erhoehen
        if (c.getroot() == strongest && boostroot!=1) {
            probability = probability*boostroot;
            //std::cout << "bossted chord " << c << std::endl;
        }

        //printf("%s\t%d\t%f\n", c.getchordtype().name.c_str(), c.getroot(), dist);
        unsigned int j=0;
        for (j=0; j<vecCp.size(); j++) {
            if (probability > vecCp[j].mProbability) {
                break;
            }
        }

        // chord ist schlechter als die bereits gespeicherten, aber der vektor ist noch zu klein
        if (j==vecCp.size() && vecCp.size()< num) {
            vecCp.push_back(ChordProb(probability, c));
        } else if (j<vecCp.size()) {
            vecCp.insert(vecCp.begin()+j,  ChordProb(probability, c));
            // anstatt hier gleich das neue letzte element zu entfernen machen wir das einmal zum schluss mit erase
        }
    }


        // vektor auf maximal num elemente reduzieren
    vecCp.erase(vecCp.begin()+num, vecCp.end());
    //if (vecCp.size>0) std::cout << "The Winner is " << vecCp.at(0).mChord << " with distance " << mindistance << std::endl;
    return vecCp;

}


std::string gettype(std::string s) {
    int pos = 1;
    if (s.length()>1 && (s[1] == 'b' || s[1]=='#')) pos=2;
    std::string type=s.substr(pos, s.npos);
    return type;
}


PCPM learn(PCPTrack pt, std::string labelfile) {
    typedef std::map< std::string, int > IM;

    PCPM ctm;
    IM counts;

    std::vector<msec_val_pair> labels = readinLabelfile(labelfile.c_str());
    unsigned int j=0;
    for (unsigned int i=0; i<labels.size()-1 && j<pt.size()-1; i++) {
        int root = PitchName2Index(labels[i].val);
        std::string type = gettype(labels[i].val);
// 		std::cout << labels[i].sec << ": " << root << type << " (" << labels[i].val << ")" << std::endl;
        if (root != -1 && labels[i].val.length() < 7) {
// 			std::cout << j << " " << pt.startTime(j) << " >= " << labels[i].sec*1000 << " && " <<
// 					pt.startTime(j+1) << " <= " << labels[i+1].sec*1000 << std::endl;
            while (j < pt.size()-1 && (labels[i].sec*1000 - pt.startTime(j)) > 0.1) {
                j++;
// 				std::cout << "shift";
            }
            while (j < pt.size()-1 // && (pt.startTime(j) - labels[i].sec*1000) < 0.1
                            && (labels[i+1].sec*1000 - pt.startTime(j+1)) > -0.1) {
// 				std::cout << pt.startTime(j) << " " << labels[i].val << std::endl;
                normalize(pt.at(j).pcp, PCPLEN);
                ctm[type].addPCP(pt.at(j), root);
                counts[type] ++;
                j++;
            }
        }
// 		std::cout << std::endl;
    }
    //std::cout << "------------------------" << std::endl;

    // normalisieren
    typedef PCPM::iterator CI;
    for (CI p = ctm.begin(); p!=ctm.end(); ++p) {
        normalize((p->second).pcp, PCPLEN);
        std::cout << p->first << " " << counts[p->first] << std::endl;
        //std::cout << (p->second).bargraph() << std::endl;
    }

    return ctm;
}
