#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "Global.h"
#include "Compressor.h"
#include "IpmtConfiguration.h"
#include "LZ77C.h"
#include "LZ78C.h"
#include "LZWC.h"
#include "DummyCompressor.h"

class Serializer {
public:
	Serializer(IpmtConfiguration& config);

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
