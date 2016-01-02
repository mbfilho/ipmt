#ifndef DESERIALIZER_H
#define DESERIALIZER_H
#include "Global.h"
#include "Decompressor.h"
#include "IpmtConfiguration.h"

class Deserializer { 
public:
	Deserializer(IpmtConfiguration& config);

	int deserializeInt();
	char deserializeChar();

private:
	Decompressor* decompressor;	
	
	ull lastToken;
	int lastTokenSize;

	Byte curByte;
	int availableBits;

	bool readBit();
	int readBunchOfBits(int howMany);
};

#endif
