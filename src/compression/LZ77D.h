#ifndef LZ77D_H
#define LZ77D_H
#include "InputBuffer.h"
#include "Decompressor.h"
#include "SlidingWindow.h"

class LZ77D : public Decompressor {
public:
	LZ77D(const char* filename);	
	
	void close(); 
	int readInt();
	int readByte();
private:
	InputBuffer input;

	int WB, WL;
	int bitsWL, bitsWB;
	
	SlidingWindow* window;	

	void readToken();
};

#endif
