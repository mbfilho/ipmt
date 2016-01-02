#ifndef DESERIALIZER_H
#define DESERIALIZER_H
#include "Global.h"
#include "Decompressor.h"

class Deserializer { 
public:
	Deserializer(Decompressor* decompressor);

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
