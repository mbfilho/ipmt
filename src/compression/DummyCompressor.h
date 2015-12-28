#include "Compressor.h"
#include "Global.h"

class DummyCompressor : public Compressor {
public:
	DummyCompressor(const char* filename);	
	void writeInt(int arg);
	void writeByte(char arg);
	void flushAndClose();
private:
	FILE* file;
};

