
#include <iostream>
#include <fstream>
#include <sstream>

#include "Labelfile.h"

/* ***************************************
 * Liest ein File mit sec - val paaren einer
 * jede zeile muss folgendes format haben:
 * <sec><whitespace><val>
 * sec = <digit>+((,|\.)<digit>*)?
 * val = .*
 *
 * gibt einen vektor mit den paaren zurueck
 * ***************************************/
std::vector<msec_val_pair> readinLabelfile(const char * filename) {
	std::vector<msec_val_pair> v;
	msec_val_pair mv;
	
	std::ifstream fin(filename);
	
	if (!fin) {
		std::cerr << "File not found " << filename <<std::endl;
		return v;
	}
	
	
	
	
	std::string line;
	while (getline(fin, line)) {
		// zahlen mit Beistrich als komma sind erlaubt -> , in . umwandeln
		std::string::size_type st_ws = line.find_first_of(" \t");
		std::string::size_type st_komma = line.find_first_of(",");
		if (st_ws > st_komma) line[st_komma] = '.';
		if (line.length() > 1 && line[0] != '#') {
			std::istringstream ist(line);
			if (!(ist >> mv.sec)) {
				std::cerr << "wrong format: ." << line << "." << std::endl;
				throw "label format error";
			}
			mv.val = line.substr(st_ws+1);
			v.push_back(mv);
			// std::cout << "(" << mv.sec << ", " << mv.val << ")" << std::endl;
		}
	}
	
	// falls die vektoren nich bei zeitpunkt null beginnen, fuege null zeile ein
	msec_val_pair m0 = {0, ""};
	if (v.at(0).sec != 0) v.insert(v.begin(),m0);

	return v;
}
