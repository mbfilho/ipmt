#include "DummyDecompressor.h"

DummyDecompressor::DummyDecompressor(FILE* inputFile) {
	file = inputFile;
}

 int DummyDecompressor::readInt() {
 	int tmp;
	assert(1 == fread(&tmp, sizeof(int), 1, file));

	return tmp;
 }

int DummyDecompressor::readByte() {
 	int tmp;
	assert(1 == fread(&tmp, sizeof(char), 1, file));

	return tmp;
 }

 void DummyDecompressor::close() {
 	fclose(file);
 }

