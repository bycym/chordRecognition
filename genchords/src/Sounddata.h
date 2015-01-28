//
// C++ Interface: Sounddata
//
// Description: 
//
//
// Author: Veronika Zenz <veronika.zenz@google.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __GENCHORDS__SOUNDDATA_H__
#define __GENCHORDS__SOUNDDATA_H__
#include <iostream>
#include <string>
#include <QString>

class Sounddata {
	public:
		Sounddata() {};
        Sounddata(const std::string soundfilename);
        Sounddata(QString soundfilename);
		~Sounddata();
		
        void init(const std::string soundfilename);
        void init(const QString soundfilename);
		
		int samplerate() const { return mSamplerate;}
		long frames() const { return mFrames; }
		const float * srcbuffer() const { return mSrcBuffer; }
		float getsample(long nr) const;
		bool initialized() const {return (mFrames > 0);}
			

	private:
        void import(const std::string soundfilename);
        void import(const QString soundfilename);

        void importMP3(const std::string mp3name);
        void importMP3(const QString mp3name);
		
		// make copy constructor unusable by making it private
		Sounddata(const Sounddata &) { std::cerr << "Sounddata Copy Constructor should not be used";}
		void operator= (const Sounddata &) {
			std::cerr << "Sounddata Assignment operator should not be used"; 
		}
				
		float * mSrcBuffer;
		long mFrames;
		int mSamplerate;
};

void printSfInfo(std::string soundfilename);
#endif
