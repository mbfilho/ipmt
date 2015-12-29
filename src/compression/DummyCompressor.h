#include "Compressor.h"

class DummyCompressor : public Compressor {
public:
	DummyCompressor(const char* filename);	
	void writeByte(int arg);
	void flushAndClose();
};

