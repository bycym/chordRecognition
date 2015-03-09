//
// C++ Interface: Key
//
// Description: 
//
//
// Author: Veronika Zenz <veronika.zenz@google.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef __GENCHORDS__KEY_H__
#define __GENCHORDS__KEY_H__



#include <string>
#include <iostream>
#include "PitchName.h"
#include <QDebug>
// Own SoundData:
#include "../../src/IOMusicHandler/sounddata.h"

class Key {
	public:
		Key():root(-1), type() {};
		Key(int troot, std::string ttype): root(troot), type(ttype) {};
		
		std::string tostring() const{
			return std::string("") + PitchName(root) + type;
		};
		
		int root;
		std::string type;
};

std::ostream& operator<< (std::ostream & s, const Key & k);

/// own SoundData
// Key getkey(Sounddata & sd, int windowSize);
Key getkey(SoundData *& sd, int windowSize);
	
	
#endif
