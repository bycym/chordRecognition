/* ***************************************************************************************
 *
 * This Module implements different algorithms for beat detection
 * The algorithms were taken from The Tutorial
 * "Frederic Patin, Beat Detection Algorithms" which is available on www.gamedev.net
 *
 * Author: Veronika Zenz <veronika.zenz@google.com>, (C) 2006
 * ****************************************************************************************/
 
#include <cmath>
#include "FFT.h"

#include <iostream>
#include <sstream>
#include <iomanip>
//#include "Sounddata.h"

/* Simple Sound Energy Algorithm #2 + #3*/ 

/* **********************************
 * mProcessed contains the timing of beats.
 * Return value is the number of detected beats
 * **********************************/

int SSE (int windowSize, int samplerate, const float * mData, int mDataLen, float *mProcessed, bool variableC) {

 float instantSoundEnergy;
 float averageLocalEnergy;
 
 /* historylen: should be (samplingFreq/windowSize)*memoryInSeconds. memory is typically 1 second ->
    for samplingFreq 441000 and windowSize 1024 agood history length is 43 */
 int historylen = samplerate/windowSize * 1; // 1 second
 
 float * energyHistoryBuffer = new float[historylen];
 
 /* the energyHistory works like a ring stack of size historylen; historyPosition saves the top, where
 the next value will be written. */
 int historyTop = 0;
 
 float variance = 0;
 float C = 1.3;
 
 for (int i = 0; i < mDataLen / windowSize; i++)
	 mProcessed[i] = float(0.0);
 
 int start=0;
 int count=0;
 int countBeats=0;
 
 int death=int(samplerate*0.2); // innerhalbe 0.2 sekunden nach einem beat sei man fuer neue beats taub
 int lastbeat = 0;
 while (start + windowSize <= mDataLen) {
	 instantSoundEnergy = 0;
	 for (int i=0; i<windowSize; i++) {
		 instantSoundEnergy+=fabs(mData[start+i]);
	 }
	 
	 averageLocalEnergy = 0;
	 for (int i=0; i<historylen; i++) {
		 averageLocalEnergy += fabs(energyHistoryBuffer[i]);
	 }
	 averageLocalEnergy /= historylen;
	 
	 if (variableC) {
		 for (int i=0; i<historylen; i++) {
			 variance += fabs(energyHistoryBuffer[i]-averageLocalEnergy);
		 }
		 variance /= historylen;
		
		 C = (-0.0025714*variance) + 1.5142857;
	 }
	 
	 if (count > historylen) {
// 		std::cout << std::setprecision(4) << float(start)/samplerate << "\t" << instantSoundEnergy << "\t" 
// 				<< averageLocalEnergy << "\t"
// 				<< (instantSoundEnergy > C*averageLocalEnergy) << std::endl;
		if (instantSoundEnergy > C*averageLocalEnergy && start > lastbeat+death) {
			float sec = float(start)/samplerate;
			mProcessed[countBeats] = sec;
			countBeats++;
			lastbeat=start;
		}
	 }
	 
	 energyHistoryBuffer[historyTop] = instantSoundEnergy; 
	 
	 start += windowSize;		 
	 count++;
	 historyTop = (historyTop+1) % historylen;
 }
 delete[]energyHistoryBuffer;
 
 return countBeats;
}



/* Frequency Selected Sound Energy */
int FSE (int windowSize, int samplerate, const float * mData, int mDataLen, float *mProcessed) {

	int subbands = 64;
	float C = 100;
	int historylen = samplerate/windowSize * 1; // 1 second
	
	float * energyHistoryBuffer = new float[subbands*historylen];
	std::cout << "history buffer has size: " << subbands*historylen << "." << std::endl;
	float *in = new float[windowSize];
	float *out = new float[windowSize];		
	float *instantSoundEnergy = new float[subbands];
	float *averageLocalEnergy = new float[subbands];	
	
	
	int historyTop = 0;	
	int start=0;
	int countBeats=0;
	
	for (int i = 0; i < mDataLen / windowSize; i++)
		mProcessed[i] = float(0.0);
	
	for (int i=0; i<subbands*historylen; i++) {
		energyHistoryBuffer[i]=0.0;
	}
	
	int death=int(samplerate*0.2); // innerhalbe 0.2 sekunden nach einem beat sei man fuer neue beats taub
	int lastbeat = 0;
 
	while (start + windowSize <= mDataLen) {
		
		for (int i = 0; i < windowSize; i++)
			in[i] = mData[start + i];
		//analysefrequencies(eaSPECTRUM, eaENHANCED_AUTOCORRELATION, windowSize, in, windowSize, out);
		
		WindowFunc(eaENHANCED_AUTOCORRELATION, windowSize, in);
		PowerSpectrum(windowSize, in, out);
		
		for (int i=0; i<subbands; i++) {
			instantSoundEnergy[i] = 0;
			for (int j=0; j<(windowSize/subbands); j++) {
				instantSoundEnergy[i] += fabs(out[i*j]);
			}
			instantSoundEnergy[i]=instantSoundEnergy[i]*(float(subbands)/windowSize);
		}
		
		
		for (int i=0; i<subbands; i++) {
			averageLocalEnergy[i]=0;
			for (int j=0; j<historylen; j++) {
				averageLocalEnergy[i] += energyHistoryBuffer[historylen*i+j];
			}
			averageLocalEnergy[i]/=subbands;
		}
		if (start > (historylen*windowSize/2)) {
			bool beat = false;
			for (int i=0; i<subbands; i++) {
				float sec = float(start)/samplerate;
				if (instantSoundEnergy[i] > averageLocalEnergy[i]*C  && start > lastbeat+death) {
					//std::cout << sec <<"\t" << i << "\t" << instantSoundEnergy[i] << "\t>" << averageLocalEnergy[i] << std::endl;
					beat = true;
					
					mProcessed[countBeats] = sec;
				}
			}
			if (beat) {
				countBeats++;
				lastbeat = start;
			}
		}	
// 		std::cout << "start: " << start << std::endl;
		
		for (int i=0; i<subbands; i++) {
			energyHistoryBuffer[historylen*i  + historyTop] = instantSoundEnergy[i]; 
			//std::cout << "historyBuffer[" << historylen*i + historyTop<< "]" << std::endl;
		}
	 
		start += windowSize;		 
		historyTop = (historyTop+1) % historylen;		
	}
	
	delete[]in;
	delete[]out;
	delete[]averageLocalEnergy;
	delete[]instantSoundEnergy;
	delete[]energyHistoryBuffer;
 
	return countBeats;
}


std::string printBeatStructure(float * beats, int countbeats) {	
	std::ostringstream ost;
	for (int i=0; i<countbeats; i++) {
		std::ostringstream ost2;
		ost2 << std::setprecision(4) << beats[i];
		std::string ssec = ost2.str();
		if (ssec.find(".")!=ssec.npos) ssec.replace(ssec.find("."), 1, ",");
		ost << ssec << "\t1" << std::endl;
	}
	return ost.str();
		
}
