#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H
#include "Global.h"

class Decompressor {
public:
	Decompressor();

	virtual int readInt(int size);
	virtual void close() = 0;

protected:
	virtual int readByte() = 0;
	
private:
	ull nextBunchOfBits;
	int nextBitsCount;
};

#endif
