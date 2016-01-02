#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H
#include "Global.h"

class InputBuffer {
public:
	InputBuffer(FILE* inputFile);

	int getNextBit();
	ull getBunchOfBits(int howMany);

	void close();
private:
	ull* buffer;
	int maxSize;
	int size;
	int curPos;

	ull nextBit;

	FILE* file;

	void fillBuffer(); 
	void fillIfEmpty(); 
};


#endif
