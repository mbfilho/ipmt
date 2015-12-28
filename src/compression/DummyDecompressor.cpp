#include "DummyDecompressor.h"

DummyDecompressor::DummyDecompressor(const char* filename) {
	file = fopen(filename, "rb");
	if(!file) {
		printf("Nao foi possível abrir o arquivo \'%s\' para leitura\n", filename);
		assert(file);
	}
}

 int DummyDecompressor::readInt() {
 	int tmp;
	assert(1 == fread(&tmp, sizeof(int), 1, file));

	return tmp;
 }

 char DummyDecompressor::readByte() {
 	char tmp;
	assert(1 == fread(&tmp, sizeof(char), 1, file));

	return tmp;
 }

 void DummyDecompressor::close() {
 	fclose(file);
 }

