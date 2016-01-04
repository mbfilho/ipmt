#ifndef IPMT_CONFIGURATION_H
#define IPMT_CONFIGURATION_H

#include "Global.h"

class IpmtConfiguration {
public:
	string mode;
	
	int interrupt; //test-only!
	
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
	
	//Tamanho das janelas de buffer e lookAhead do LZ77
	int wb, wl; 

	CompressionAlgorithm getCompressionAlgorithm();

	IndexDataStructure getIndexDataStructure();
};
#endif
