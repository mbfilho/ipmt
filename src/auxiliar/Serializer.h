#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "Global.h"
#include "Compressor.h"
#include "IpmtConfiguration.h"

class Serializer {
public:
	Serializer(IpmtConfiguration& config);

	void serializeInt(int value);
	void serializeChar(char ch);

private:
	Compressor* compressor;	
	
	ull lastToken;
	int lastTokenSize;

	ull encodeInt(int arg, int* encodedSize);

	void writeToCompressor(ull token, int tokenSize);
};

#endif
