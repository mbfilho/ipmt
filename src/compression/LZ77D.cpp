#include "LZ77D.h"

LZ77D::LZ77D(FILE* inputFile, int wb, int wl) : input(inputFile){
	WB = wb, WL = wl;
	bitsWB  = (WB == 0 ? 1 : 32 - __builtin_clz(WB));
	bitsWL  = (WL == 0 ? 1 : 32 - __builtin_clz(WL));
	window = new uchar[MAX_WINDOW_SIZE];

	windowOffset = availableBytes = 0;
	for(int i = 0; i < WB; ++i)
		window[i] = 0;
}

void LZ77D::close() {
	input.close();
}

int LZ77D::readByte() {
	if(!availableBytes) {
		if(windowOffset + WB + WL >= MAX_WINDOW_SIZE) {
			for(int i = 0; i < WB; ++i) {
				window[i] = window[windowOffset+i];
			}
			windowOffset = 0;
		}
		readToken();
	}
	int byte = window[windowOffset+WB];
	++windowOffset, --availableBytes;

	return byte;
}

void LZ77D::readToken() {
	int pos, size, mismatching;	
	pos = input.getBunchOfBits(bitsWB);
	size = input.getBunchOfBits(bitsWL);
	mismatching = input.getBunchOfBits(8);
		
	for(int i = 0; i < size; ++i) {
		int newByte = window[i+pos+windowOffset];
		window[windowOffset+WB+i] = newByte;
	}

	window[windowOffset+WB+size] = mismatching;
	availableBytes = size + 1;
}
