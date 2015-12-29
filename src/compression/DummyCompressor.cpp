#include "DummyCompressor.h"

DummyCompressor::DummyCompressor(const char* filename): Compressor(filename) {
}

void DummyCompressor::writeByte(int arg) {
	insertIntoBuffer(arg, 8);
}

void DummyCompressor::flushAndClose() {
	flush(true);
	close();
}

