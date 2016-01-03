#include "Decompressor.h"

Decompressor::Decompressor() {
	nextBunchOfBits = 0;
	nextBitsCount = 0;
}

int Decompressor::readInt(int size) {
	while(nextBitsCount < size) {
		int byte = (readByte());
		nextBunchOfBits |= ull(byte) << nextBitsCount;
		nextBitsCount += 8;
	}
	int ret = int(nextBunchOfBits & ((1ULL << size) - 1));
	nextBunchOfBits >>= size;
	nextBitsCount -= size;
	
	return ret;
}
