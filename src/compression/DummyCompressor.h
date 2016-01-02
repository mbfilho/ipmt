#include "Compressor.h"

class DummyCompressor : public Compressor {
public:
	DummyCompressor(FILE* output);	
	void writeByte(int arg);
	void flushAndClose();
};

