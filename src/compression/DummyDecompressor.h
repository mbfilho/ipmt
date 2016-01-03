#ifndef DUMMY_DECOMPRESSOR_H
#define DUMMY_DECOMPRESSOR_H

#include "Decompressor.h"
#include "Global.h"

class DummyDecompressor : public Decompressor {
public:
	DummyDecompressor(FILE* inputFile);

	virtual int readByte();
	void close();

private:
	FILE* file;
};
#endif
