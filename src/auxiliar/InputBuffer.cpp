#include "InputBuffer.h"

InputBuffer::InputBuffer(const char* fileName) : fileName(fileName) {
	file = fopen(fileName, "rb");
	if(!file) {
		printf("[InputBuffer] Não foi possível abrir o arquivo \'%s\' para leitura.\n", fileName);
		assert(file);
	}

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
	++bitPos;

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
		bitPos = 0;
	}
}

void InputBuffer::fillBuffer() {
	int read = fread(buffer, sizeof(ull), maxSize, file);
	if(!read){
		printf("Erro de I/O: não foi possível ler mais bytes do arquivo \'%s\'\n", fileName);
		assert(read); 
	}
	size = read;
}
