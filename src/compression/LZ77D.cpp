#include "LZ77D.h"

LZ77D::LZ77D(const char* filename) : input(filename){
	WB = input.getBunchOfBits(32);
	WL = input.getBunchOfBits(32);
	bitsWB  = (WB == 0 ? 1 : 32 - __builtin_clz(WB));
	bitsWL  = (WL == 0 ? 1 : 32 - __builtin_clz(WL));
	window = new SlidingWindow(WB+WL);

	for(int i = 0; i < WB; ++i)
		window->append(0);
}

void LZ77D::close() {
	input.close();
}

int LZ77D::readInt() {
	int ret = 0;
	for(int i = 0; i < 4; ++i) 
		ret |= readByte() << (i * 8);
	return ret;	
}

int LZ77D::readByte() {
	if(window->getSize() == WB) 
		readToken();
	int byte = window->get(WB);
	window->slide(1);

	return byte;
}

void LZ77D::readToken() {
	int pos, size, mismatching;	
	pos = input.getBunchOfBits(bitsWB);
	size = input.getBunchOfBits(bitsWL);
	mismatching = input.getBunchOfBits(8);
		
	for(int i = 0; i < size; ++i) {
		int newByte = window->get(i+pos);
		window->append(newByte);
	}

	window->append(mismatching);
}
