#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "Global.h"
#include "Compressor.h"

class Serializer {
public:
	Serializer(Compressor* compressor);

	void serializeInt(int value);
	void serializeChar(char ch);
	
	void flushAndClose();
private:
	Compressor* compressor;	
	
	ull lastToken;
	int lastTokenSize;

	ull encodeInt(int arg, int* encodedSize);

	void writeToCompressor(ull token, int tokenSize);
};

#endif
