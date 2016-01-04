#include "Decompressor.h"

Decompressor::Decompressor() {
	nextBunchOfBits = 0;
	nextBitsCount = 0;
}

int Decompressor::readBits(int howMany) {
	while(nextBitsCount < howMany) {
		int byte = decompressNextByte();
		nextBunchOfBits |= ull(byte) << nextBitsCount;
		nextBitsCount += 8;
	}
	int ret = int(nextBunchOfBits & ((1ULL << howMany) - 1));
	nextBunchOfBits >>= howMany;
	nextBitsCount -= howMany;
	
	return ret;
}
