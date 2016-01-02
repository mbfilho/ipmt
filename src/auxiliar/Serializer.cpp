#include "Serializer.h"

Serializer::Serializer(Compressor* compressor): compressor(compressor) {
}

void Serializer::serializeInt(int value) {
	int encodedSize;
	ull encodedInt = encodeInt(value, &encodedSize);
	writeToCompressor(encodedInt, encodedSize);
}

void Serializer::serializeChar(char ch) {
	writeToCompressor(ch, 8);	
}

ull Serializer::encodeInt(int arg, int *encodedSize) {
	int size = SIZE_IN_BITS(arg);
	int sizeOfSize = SIZE_IN_BITS(size);

	//token eh o monte de bits gerados da codificação de 'arg'	
	ull token = 
		(1ULL<<sizeOfSize)-1 //representação unária de size
		| (ull(size) << (sizeOfSize + 1))//separador '0' + representação (binária) de size.
		| (ull(ui(arg)) << (2*sizeOfSize+1)); //acrescenta o 'arg' propriamente dito

	*encodedSize = 2*sizeOfSize+1+size;
	return token;
}

void Serializer::writeToCompressor(ull token, int tokenSize) {
	lastToken |= (token << lastTokenSize);
	int filledBits = MIN(64 - lastTokenSize, tokenSize);
	token >>= filledBits;
	tokenSize -= filledBits;
	if(lastTokenSize == 64) {
		compressor->writeInt(int(lastToken));
		lastToken >>= 32;
		compressor->writeInt(int(lastToken));
		lastToken = 0;
		lastTokenSize = 0;
	}

	if(tokenSize) {
		lastToken = token;
		lastTokenSize = tokenSize;
	}
}

void Serializer::flushAndClose() {
	if(lastTokenSize) {
		compressor->writeInt(int(lastToken));
		lastToken >>= 32;
		compressor->writeInt(int(lastToken));
	}
	compressor->flushAndClose();
}

