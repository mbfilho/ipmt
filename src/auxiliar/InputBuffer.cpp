#include "InputBuffer.h"

InputBuffer::InputBuffer(FILE* inputFile) {
	file = inputFile;

	maxSize = 1024;
	buffer = new ull[maxSize];
	size = curPos = 0;
	nextBit = 0;
	nextBitPos = 64;
}

void InputBuffer::close() {
	fclose(file);
}

int InputBuffer::getNextBit() {
	fillIfEmpty();

	int bit = 0;
	if(buffer[curPos] & nextBit) bit = 1;
	nextBit <<= 1;
	++nextBitPos;

	return bit;
}

ull InputBuffer::getBunchOfBits(int howMany) {
	ull bunch = 0;
	int curSize = 0;

	while(curSize < howMany) {	
		fillIfEmpty();

		int read = MIN(64 - nextBitPos, howMany - curSize);
		ull mask = (read == 64) ? -1 : (1ULL << read) - 1;
		bunch |=  ((buffer[curPos] >> nextBitPos) & mask) << curSize; 
		curSize += read;

		nextBitPos += read;
		nextBit = (1ULL << nextBitPos);
	}

	return bunch;
}

void InputBuffer::fillIfEmpty() {
	if(nextBitPos == 64) { //bitPos = 64
		++curPos;
		if(curPos >= size) {
			curPos = 0;	
			fillBuffer();
		}

		nextBit = 1;
		nextBitPos = 0;
	}
}

void InputBuffer::fillBuffer() {
	int read = fread(buffer, sizeof(ull), maxSize, file);
	assert(read); 
	size = read;
}
