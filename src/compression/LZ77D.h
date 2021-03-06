#ifndef LZ77D_H
#define LZ77D_H
#include "InputBuffer.h"
#include "Decompressor.h"

#define MAX_WINDOW_SIZE (1<<20)
class LZ77D : public Decompressor {
public:
	LZ77D(FILE* input, int wb, int wl);	
	
	void close(); 
private:
	InputBuffer input;
	
	int WB, WL;
	int bitsWL, bitsWB;

	uchar* window;
	int windowOffset;	

	int availableBytes;
	

	int decompressNextByte();
	void readToken();
};

#endif
