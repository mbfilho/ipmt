#include "DummyDecompressor.h"

DummyDecompressor::DummyDecompressor(FILE* inputFile) {
	file = inputFile;
}

int DummyDecompressor::decompressNextByte() {
 	Byte tmp;
	assert(1 == fread(&tmp, sizeof(Byte), 1, file));
	return tmp;
 }

 void DummyDecompressor::close() {
 	fclose(file);
 }

