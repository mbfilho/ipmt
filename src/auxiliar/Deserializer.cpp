#include "Deserializer.h"

Deserializer::Deserializer(Decompressor* decompressor) : decompressor(decompressor) {
	curByte = 0;
	availableBits = 0;
}

int Deserializer::deserializeInt() {
	int sizeOfSize = 0, size = 0;	

	while(readBit()) 
		++sizeOfSize; 
	size = (int) readBunchOfBits(sizeOfSize);	
	return (int) readBunchOfBits(size);
}

char Deserializer::deserializeChar() {
	return char(decompressor->readByte());
	return char(readBunchOfBits(8));
}

bool Deserializer::readBit() {
	if(!availableBits) {
		curByte = decompressor->readByte();
		availableBits = 8;
	}

	bool bit = (curByte & 1) != 0;
	curByte >>= 1;
	--availableBits;
	return bit;
}

int Deserializer::readBunchOfBits(int howMany) {
	int at = 0, left = howMany;
	
	for(int i = 0; i < howMany; ++i){
		at |= (readBit() << i);
	}
	return at;
	while(left) {
		//ler do byte atual
		int lidos = MIN(left, availableBits);
		at |= (curByte & ((1<<lidos)-1)) << (howMany - left);
		availableBits -= lidos;
		curByte >>= lidos;
		left -= lidos;

		while(left > 8) {
			at |= decompressor->readByte() << (howMany-left); 
			left -= 8;
		}

		if(!availableBits) {
			availableBits = 8;
			curByte = decompressor->readByte();
		}
	}

	return at;
}
