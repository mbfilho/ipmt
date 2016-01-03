#include "DummyCompressor.h"

DummyCompressor::DummyCompressor(FILE* output): Compressor(output) {
}

void DummyCompressor::writeByte(int arg) {
	insertIntoBuffer(arg, 8);
}

void DummyCompressor::flushAndClose() {
	flushInput();
	flushOutput();
	close();
}

