
#ifndef __GENCHORDS__LABELFILE_H__
#define __GENCHORDS__LABELFILE_H__

#include "string"
#include "vector"

struct msec_val_pair{
	float sec;
	std::string val;
};

std::vector<msec_val_pair> readinLabelfile(const char * filename);

#endif
