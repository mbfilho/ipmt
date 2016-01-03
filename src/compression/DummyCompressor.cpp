#include "DummyCompressor.h"

DummyCompressor::DummyCompressor(FILE* output): Compressor(output) {
}

void DummyCompressor::feedRawByte(Byte arg) {
	writeTokenToFile(arg, 8);
}

void DummyCompressor::onClosing() {
}

