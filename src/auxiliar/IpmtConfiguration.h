#ifndef IPMT_CONFIGURATION_H
#define IPMT_CONFIGURATION_H

#include "Global.h"

class IpmtConfiguration {
public:
	string mode;
	
	string indexType;
	string compression;
	int countFlag;
	int helpFlag;

	vector<string> patterns;
	string patternFileName;
	
	string indexFileName;
		
	string textFileName;

	IpmtConfiguration(); 

	bool validateConfig(); 

};
#endif
