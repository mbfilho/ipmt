#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H
#include "Global.h"

class Decompressor {
public:
	Decompressor();

	virtual int readBits(int howMany);
	virtual void close() = 0;

protected:
	virtual int decompressNextByte() = 0;
	
private:
	ull nextBunchOfBits;
	int nextBitsCount;
};

#endif
