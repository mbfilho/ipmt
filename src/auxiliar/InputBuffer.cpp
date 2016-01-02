#include "InputBuffer.h"

InputBuffer::InputBuffer(FILE* inputFile) {
	file = inputFile;

	maxSize = 1024;
	buffer = new ull[maxSize];
	size = curPos = 0;
	nextBit = 0;
}

void InputBuffer::close() {
	fclose(file);
}

int InputBuffer::getNextBit() {
	fillIfEmpty();

	int bit = 0;
	if(buffer[curPos] & nextBit) bit = 1;
	nextBit <<= 1;

	return bit;
}

ull InputBuffer::getBunchOfBits(int howMany) {
	ull res = 0;
	for(int i = 0; i < howMany; ++i)
		res |= ull(getNextBit()) << i;
	return res;
}

void InputBuffer::fillIfEmpty() {
	if(!nextBit) { //bitPos = 64
		++curPos;
		if(curPos >= size) {
			curPos = 0;	
			fillBuffer();
		}

		nextBit = 1;
	}
}

void InputBuffer::fillBuffer() {
	int read = fread(buffer, sizeof(ull), maxSize, file);
	assert(read); 
	size = read;
}
