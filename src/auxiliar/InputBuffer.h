#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H
#include "Global.h"

class InputBuffer {
public:
	InputBuffer(const char* fileName);

	int getNextBit();
	ull getBunchOfBits(int howMany);

	void close();
private:
	ull* buffer;
	int maxSize;
	int size;
	int curPos;

	ull nextBit;
	int bitPos;

	FILE* file;
	const char* fileName;

	void fillBuffer(); 
	void fillIfEmpty(); 
};


#endif
