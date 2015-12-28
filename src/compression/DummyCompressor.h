#include "Compressor.h"
#include "Global.h"

class DummyCompressor : public Compressor {
public:
	DummyCompressor(const char* filename);	
	void writeInt(int arg);
	void writeArrayOfInts(const int* arg, int size);
	void writeByte(int arg);
	void writeText(const char* text, int size);
	void flushAndClose();
private:
	FILE* file;
};

