#ifndef DUMMY_COMPRESSOR_H
#define DUMMY_COMPRESSOR_H

#include "Compressor.h"

class DummyCompressor : public Compressor {
public:
	DummyCompressor(FILE* output);	

protected:
	void feedRawByte(Byte arg);
	void onClosing();
};

#endif
