//
// C++ Implementation: labeldiff
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
#include <iomanip> /* fuer Manipulatoren (outputformatierungen wie setprecision */
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <string.h>
#include <cmath>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "unistd.h" // for getopt

#include "Labelfile.h"

#define MATRIX_LENGTH 1
#define MATRIX_COUNT 2


struct cfm_content {
	int count;
	float sec;
};

// Typedef fuer Confusion Matrix
typedef std::map<std::string, cfm_content> CFL;
typedef std::map< std::string, CFL > CFM;


char * cmd="<not yet set>";
float countMatchAll = 0;
int matrixoutput = 1;

void bailout(std::string message) {
	std::cerr << message << std::endl;
	exit(1);
}

void message(std::string m) {
	std::cout << m << std::endl;
}


std::string enharmonic(std::string pitch) {
	if (pitch=="Db") return "C#";
	if (pitch=="Eb") return "D#";
	if (pitch=="Fb") return "E";
	if (pitch=="Gb") return "F#";
	if (pitch=="Ab") return "G#";
	if (pitch=="Bb") return "A#";
	if (pitch=="Cb") return "B";
	else return pitch;
}

std::string enharmonicChange(std::string s) {
	//std::cout << "enharmonic Change: " << s <<" -> ";
	const char * pitchnames[7][2] = {{"Db", "C#"}, {"Eb", "D#"}, {"Fb", "E"}, {"Gb", "F#"}, {"Ab", "G#"}, {"Bb", "A#"}, {"Cb", "B"}};
	for (int i=0; i<7; i++) {
		int index = s.find(std::string(pitchnames[i][0]));
		while (index>=0) {
			//std::cout << "rep" << pitchnames[i][0] << " at index " << index;
			s.replace(s.find(pitchnames[i][0]), strlen(pitchnames[i][0]), pitchnames[i][1]);
			index = s.find(std::string(pitchnames[i][0]));
		}
	}
	return s;
	//std::cout << s << std::endl;
}

bool compareStyle(std::string s1, std::string s2) {
	/* just compare maj and min (first three chars) ignore additional signs like 7, sus, .. */
	if (s1.substr(0,3) == s2.substr(0,3)) return true;
	//return (s1==s2);
	return false;
}


/* vergleicht zwei Akordnamen. 
   Beruecksichtigt enharmonische verwechslung 
   Ausserdem:
     *	Matcht jeden Akkord
	 x/Y	Match Akkord X und Y
	__TODO__: 
	 	Grossbuchstabe Matcht Grossbuchstabe+Maj
	 	Kleinbuchstabe matcht Kleinbuchstabe+min
		Whitespaces vor dem vergleichen entfernen
*/
bool compareChords(std::string c1, std::string c2) {
	//std::cout << std::endl;
	if (c1=="*" or c2=="*") return true;
	int lastindex1 = 0;
	int seperatorindex1 = -1;
	do {
		seperatorindex1 = c1.substr(lastindex1).find("/");
		std::string c11;
		if (seperatorindex1 > 0) c11 = c1.substr(lastindex1, seperatorindex1);
		else c11 = c1.substr(lastindex1);
		// std::cout << "c11: " << c11;
		int lastindex2 = 0;
		int seperatorindex2 = -1;
		do {
			seperatorindex2 = c2.substr(lastindex2).find("/");
			std::string c22;
			if (seperatorindex2 > 0) c22 = c2.substr(lastindex2, seperatorindex2);
			else c22 = c2.substr(lastindex2);
			// std::cout << " c22: " << c22 ;
			int pos1 = 1;
			int pos2 = 1;
			if (c11.length()>1 && (c11[1] == 'b' || c11[1]=='#')) pos1=2;
			if (c22.length()>1 && (c22[1] == 'b' || c22[1]=='#')) pos2=2;
			std::string pitchname1=c11.substr(0, pos1);
			std::string pitchname2=c22.substr(0, pos2);
			//std::cout << pitchname1 << " " << pitchname2 << " " << enharmonic(pitchname1 )<< " " << enharmonic(pitchname2) << std::endl;
			if (enharmonic(pitchname1) == enharmonic(pitchname2) && compareStyle(c11.substr(pos1),c22.substr(pos2))) return true;
			lastindex2 += seperatorindex2+1;
		} while (seperatorindex2 > 0);
		
		lastindex1 += seperatorindex1+1;
	} while (seperatorindex1 > 0);
	return false;
}

std::string printcfmatrix(CFM confusionmatrix, std::set< std::string> keys, int type) {
	std::ostringstream ost;
	ost << std::endl <<  "**********  CONFUSION MATRIX *************" << std::endl << std::endl;
	ost << "rows=arg1 columns = arg2" << std::endl << std::endl;
	for (std::set<std::string>::const_iterator k=keys.begin(); k!=keys.end(); k++) {
		ost <<"\t|" << *k;
		//std::cout <<"\t";	
	}
	ost << std::endl;
	ost << std::string((keys.size()*2+1)*4, '-') <<std::endl;

	
	for (std::set<std::string>::const_iterator k1=keys.begin(); k1!=keys.end(); k1++) {
		CFL line = confusionmatrix[*k1];
		ost << *k1;
		for (std::set<std::string>::const_iterator k=keys.begin(); k!=keys.end(); k++) {
			ost << "\t";
			if (type==MATRIX_COUNT) ost << line[*k].count;
			if (type==MATRIX_LENGTH) ost << std::fixed << std::setprecision(2) << line[*k].sec;			
		}
		ost << std::endl;
	}
	ost << std::endl << "*********************************************" << std::endl;
	return ost.str();
}



/* ****************************************
 * Berechnet die Sekunden in denen v1 und v2 
 * verscheidene values haben
 * die vektoren muessen aufsteigen nach sec sortiert sein und mindestens je zwei elemente
 * enthalten.
 * die labelfiles mussen mit einer zeile endzeit und irgendeinem text aufhöhren
 *
 * Beispiel
 * v1: (0,C), (0.5, D), (1.5, G), (4, A), (8, -)
 * v2: (0,C), (1, D), (1.5, G), (2, D), (3, G), (3.5, A), (8, -)
 * sollte 2.0 zurueckgeben
 *
 * ****************************************/
float labeldiff(std::vector<msec_val_pair> & v1, std::vector<msec_val_pair> & v2, const char * outputfile=(char *) 0) {
	
	std::ostringstream ost;
	
	if (v1.size() < 2 || v2.size() < 2) return -1;
	
	std::map< std::string, cfm_content> matrixline;
	CFM confusionmatrix;
	std::set< std::string> keys;
	
	unsigned int pos1 = 0;
	unsigned int pos2 = 0;
	float curtime = 0;
	float countdiff = 0;
	countMatchAll = 0;
	
	float startdiff = 0;
	bool diff = false;
	
	ost <<"v1.size(): " << v1.size() << std::endl;
	ost <<"v2.size(): " << v2.size() << std::endl;
	
	
	
// 	// falls die vektoren nicht zur gleichen Zeit enden, verlaengere den kuerzeren vektor
// 	msec_val_pair mEnde = {fmax(v2.at(v2.size()-1).sec, v1.at(v1.size()-1).sec), ""};
// 	if (v1.at(v1.size()-1).sec != mEnde.sec) v1.insert(v1.end(), mEnde);
// 	if (v2.at(v2.size()-1).sec != mEnde.sec) v2.insert(v2.end(), mEnde);
	
	ost << "pos1\tpos2\ttime\tval1\tval2\tlen\tdiff\tstart\tsum" << std::endl;
	while (pos1+1 < v1.size() && pos2+1 < v2.size()) {	
		// length =  next curtime - this curtime
		float len = ((v1[pos1+1].sec < v2[pos2+1].sec) ? v1[pos1+1].sec : v2[pos2+1].sec) -curtime;
		
		if (v1[pos1].val == "*" || v2[pos2].val == "*") {
			countMatchAll += len;
		}
		
		// falls wir jetzt wieder gleich sind und davor verschieden : rechne ab
		bool equal = compareChords(v1[pos1].val,v2[pos2].val);
		
		if (equal && diff) {
			//if ((curtime-startdiff) > 0.2 ){
				countdiff += curtime - startdiff;
			//}
			diff = false;
		}
		// bisher waren wir gleich, jetzt verschieden : flag setzen und zeitpunkt
		// ab dem wir verschiedene werte haben speichern
		else if (!equal && ! diff) {
			startdiff = curtime;
			diff = true;
		}	
		
		ost << std::setprecision(3) << pos1 << "\t" << pos2 << "\t" << curtime << "\t" 
				<< v1[pos1].val << "\t" << v2[pos2].val << "\t" 
				<< std::fixed << std::setprecision(2) << len << "\t"
				<< diff << "\t" << startdiff << "\t" << countdiff << std::endl;	
		
		
		if (v1[pos1].val != "*" && v2[pos2].val != "*" && pos1!=0 ) {
			//Confusionmatrix mit enharmonisch eindeutigen eintraegen fuellen
			std::string label1 = enharmonicChange(v1[pos1].val);
			std::string label2 = enharmonicChange(v2[pos2].val);
			
			matrixline = confusionmatrix[label1];
			matrixline[label2].count ++;
			matrixline[label2].sec += len;
				/* std::cout << "matrixline[" << v1[pos1].val << "][" << v2[pos2].val <<"] = "
			<< matrixline[v2[pos2].val].count << " " << matrixline[v2[pos2].val].sec 
			<< std::endl;
				*/
			confusionmatrix[label1] = matrixline;
			keys.insert(label1);
			keys.insert(label2);
		}
		
		// springe zur kleineren der beiden nächsten zeiteinträge	
		if (fabs(v1[pos1+1].sec-v2[pos2+1].sec)<0.001) {pos1++; pos2++; curtime = v2[pos2].sec;}
		else if (v1[pos1+1].sec < v2[pos2+1].sec) {pos1++; curtime = v1[pos1].sec;}
		else {pos2++; curtime = v2[pos2].sec;}		
		
	}
	
	if (diff) {countdiff += curtime - startdiff;}
	
	if (matrixoutput) {
		ost << printcfmatrix(confusionmatrix, keys, matrixoutput);
	}
	ost << std::endl;
			
	if (outputfile) {
		std::ofstream out(outputfile);
		if (!out) bailout(std::string("Unable to open outputfile ") + outputfile);
		out << ost.str();
		out.close();
	}
	else {
		std::cout << ost.str();
	}
	
	
	return countdiff;
}


void usage() {
	std::cerr << cmd << " [-m (0-2)] (labelfile1 labelfile2 | labeldirectory1 labeldirectory2)" << std::endl;
	exit(1);
}



int filtertxt(const struct dirent* d) {
	std::string filename = d->d_name;
	int index = filename.rfind(".");
	if (index > 0 && filename.substr(index) == ".txt") return 1;
	return 0;
}


int main(int argc, char ** argv) {
	
	/* ******************************************** */
	/* ********** Argumentbehandlung ****** */
	/* ******************************************** */
	
    int mode = 0777;
	cmd = argv[0];
	DIR* pDir;
	
	int c;
	bool optError=false;
	while ((c=getopt(argc, argv, "m:")) != EOF) {
		char *ep;
		switch(c) {
			case 'm':
				matrixoutput =  (int)strtol(optarg, &ep, 10);
				if (*ep!='\0' || matrixoutput < 0 || matrixoutput > 2) {
					std::cerr << "invalid argument for option m:" << optarg << "." << std::endl;
					usage();
				}
				break;
			case '?':
				optError = true;
		}
	}
	
	// zwei oder drei argumente
	if (optError || optind+2>argc || optind+3<argc ) {
		usage();
	}

	char * argfile1 = argv[optind];
	char * argfile2 = argv[optind+1];
	
	/* check 3rd argument outputdir. If not set, outputs will be done to stdout
	else for each labelfile an labeldiff (prefix: ld_) file will be created in the directory outputdir
	the summary of the results will be saved in "<outputdir>/gesamt.txt" . */
	std::string outdir="";
	bool oOutdir = false;
	if (optind+3==argc) {
		oOutdir = true;
		outdir = std::string(argv[optind+2]) + "/";
		/* create if it doesn't exist */
		if ((opendir(outdir.c_str())) == 0) {
            if ((mkdir(outdir.c_str(), mode))==-1) {
				std::cerr << "Failed to create output direcotry " << outdir << ". Using stdout instead." << std::endl;
				oOutdir = false;
			}
		}	
	}
	
	
	/* ******************************************** */
	/* ********** Vergleiche zwei labelfiles ****** */
	/* ******************************************** */
	if ((pDir = opendir(argfile1)) == 0) {
		std::vector<msec_val_pair> v1 = readinLabelfile(argfile1);
		std::vector<msec_val_pair> v2 = readinLabelfile(argfile2);
		float diff = labeldiff(v1, v2);
		float length = fmin(v2[v2.size()-1].sec, v1[v1.size()-1].sec);
		// zeiten in denen eines der beiden labels * ist von der gesamtlänge abziehen
		length = length-countMatchAll;
		
		std::cout << "difference absolute: " << diff << std::endl;
		std::cout << "difference relative: " << (diff/length)*100 <<"%" << std::endl;
		std::cout << "hits relative: " << ((length-diff)/length)*100 <<"%" << std::endl;
		return 0;
	}
	
	
	/* ******************************************** */
	/* vergleiche alle labelfiles in den directories*/
	/* ******************************************** */
	else {
		struct dirent **namelist;
		int n;

		n = scandir(argfile2, &namelist, &filtertxt, alphasort);
		if (n < 0) {
			usage();
		}
		
		
		
		std::ostringstream ost;
		float ok_gesamt=0;
		float ok_rel_gesamt=0;
		float length_gesamt = 0;
		int nofiles = 0;
		//ost << "songlength\tok rel.\tfilename" << std::endl;
		for (int i=0; i<n; i++) {
			std::string labelfile1 = namelist[i]->d_name;
			std::string labelfile2 = std::string(argfile2) + "/" + labelfile1;
			message(labelfile2);
			
			std::vector<msec_val_pair> v1 = readinLabelfile((std::string(argfile1) + "/" + labelfile1).c_str());
			std::vector<msec_val_pair> v2 = readinLabelfile(labelfile2.c_str());
			if (v1.size()>0 && v2.size()>0) {
				float diff = 0;
				if (oOutdir) {
					std::string outfile = std::string(outdir + "ld_" + labelfile1);
					diff = labeldiff(v1, v2, outfile.c_str());	
				}
				else {
					diff = labeldiff(v1, v2);
				}
				float length = fmin(v2[v2.size()-1].sec, v1[v1.size()-1].sec);
				// zeiten in denen eines der beiden labels * ist von der gesamtlänge abziehen
				length = length-countMatchAll;
				
				
				float ok_rel = ((length-diff)/length)*100;
				ok_rel_gesamt += ok_rel;
				ok_gesamt += length-diff;
				length_gesamt += length;
				
				ost << std::setprecision(4) << length << "\t" << std::setprecision(4) << ok_rel << "\t"<< labelfile1 <<  std::endl;	
				message("\tOK");
				nofiles++;
			}
			else message(std::string("skipping file ") + labelfile1);
			free(namelist[i]);
		}	
		free(namelist);
		
		//ost << "============================================" << std::endl;
		ost << std::setprecision(4) << length_gesamt << "\t" << std::setprecision(4) << (ok_gesamt/length_gesamt)*100   << "\tgesamt" << std::endl;
		ost << std::setprecision(4) << length_gesamt  << "\t" << (ok_rel_gesamt/nofiles) << "\tgesamtprozent/anzFiles" << std::endl;
		
		if (oOutdir) {
			std::ofstream out((outdir + "gesamt.txt").c_str());
			if (!out) bailout(std::string("Unable to open outputfile gesamt.txt"));
			out << ost.str();
			out.close();		
		}
		else std::cout << ost.str();
	}
		
}


void test() {
	msec_val_pair mv1 []= {
		{0.3, "C"},
		{0.5, "D"},
		{1.5, "G"},
		{4, "A"},
		{8, ""}
	};
	
	msec_val_pair mv2 []= {
		{0, "X"},
		{0.3, "C"},
		{1, "F"},
		{1.5, "G"},
		{2, "D"},
		{3, "G"},
		{3.5, "A"},
		{8, ""}
	};
	
	std::vector<msec_val_pair> v1(mv1, &mv1[sizeof(mv1)/sizeof(msec_val_pair)]);
	std::vector<msec_val_pair> v2(mv2, &mv2[sizeof(mv2)/sizeof(msec_val_pair)]);
	
	std::cout << "difference: " << labeldiff(v1, v2) << std::endl;	
}
