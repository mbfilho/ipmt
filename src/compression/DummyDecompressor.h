#include "Decompressor.h"
#include "Global.h"

class DummyDecompressor : public Decompressor {
public:
	DummyDecompressor(const char* filename);

	int readInt();
	char readByte();
	void close();

private:
	FILE* file;
};
