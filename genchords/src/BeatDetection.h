//
// C++ Interface: BeatDetection
//
// Description: 
//
//
// Author: Veronika Zenz <veronika.zenz@google.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __GENCHORDS__BEATDETECTION_H__
#define __GENCHORDS__BEATDETECTION_H__
/* Simple Sound Energy */
int SSE (int windowSize, int samplerate, const float * mData, int mDataLen, float *mProcessed, bool variableC);
/* Frequency Selected Sound Energy */
int FSE (int windowSize, int samplerate, const float * mData, int mDataLen, float *mProcessed);

std::string printBeatStructure(float * beats, int countbeats);

#endif
