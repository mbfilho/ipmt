#include "DummyCompressor.h"

DummyCompressor::DummyCompressor(const char* filename) {
	file = fopen(filename, "wb");
	
	if(!file) {
		printf("Não foi possível criar o arquivo \'%s\'\n", filename);
		assert(file);
	}
}

void DummyCompressor::writeInt(int arg) {
	assert(1 == fwrite(&arg, sizeof(int), 1, file));
}

void DummyCompressor::writeArrayOfInts(const int* arg, int size) {
	assert(size == fwrite(arg, sizeof(int), size, file));
}

void DummyCompressor::writeByte(char arg) {
	assert(1 == fwrite(&arg, sizeof(char), 1, file));
}

void DummyCompressor::writeText(const char* text, int size) {
	assert(size == fwrite(text, sizeof(char), size, file));
}

void DummyCompressor::flushAndClose() {
	fclose(file);
}

