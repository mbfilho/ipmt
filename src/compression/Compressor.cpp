#include "Compressor.h"


Compressor::Compressor(const char* fileName) {
	outputFile = fopen(fileName, "wb");
	if(!outputFile) {
		printf("Não foi possível abrir o arquivo \'%s\' para escrita\n", fileName);
		assert(outputFile);
	}

	lastTokenSize = 0;
	lastToken = 0;
}

void Compressor::encodeAndWriteInt(int arg) {
	int size = (arg == 0 ? 1 : 32 - __builtin_clz(arg));
	int sizeOfSize =  32 - __builtin_clz(size);

	//token eh o monte de bits gerados da codificação de 'arg'	
	ull token = 
		(1ULL<<sizeOfSize)-1 //representação unária de size
		| (ull(size) << (sizeOfSize + 1))//separador '0' + representação (binária) de size.
		| (ull(ui(arg)) << (2*sizeOfSize+1)); //acrescenta o 'arg' propriamente dito
	
	insertIntoBuffer(token, 2*sizeOfSize+1+size);
}

void Compressor::writeInt(int arg) {
	ui u = ui(arg);
	for(int i = 0; i < 4; ++i) {
		writeByte(u & 0xff);	
		u >>= 8;
	}
}

void Compressor::writeArrayOfInts(const int* arg, int size) {
	for(int i = 0; i < size; ++i)
		writeInt(arg[i]);
}

void Compressor::writeText(const char* text, int size) {
	for(int i = 0; i < size; ++i){
		writeByte(uchar(text[i]));
	}
}

void Compressor::close() {
	fclose(outputFile);
}

void Compressor::flush(bool force) {
	fwrite(&lastToken, sizeof(ull), 1, outputFile);
}

void Compressor::insertIntoBuffer(ull token, int tokenSize){
	int filledbits = MIN(64 - lastTokenSize, tokenSize); //a quantidade de bits acrescentados ao último elemento
	lastToken |= token << lastTokenSize;//O comportamento é indeterminado quando lastTokenSize=64 (a.k.a não funciona e para achar o erro é difícil).

	lastTokenSize += filledbits;

	if(lastTokenSize == 64) {
		fwrite(&lastToken, sizeof(ull), 1, outputFile);

		//descarta os bits que foram colocados no último elemento
		token >>= filledbits;
		tokenSize -= filledbits;
		
	 	if(tokenSize) {
			lastToken = token;
			lastTokenSize = tokenSize;
		} else {
			lastToken = 0;
			lastTokenSize = 0;
		}
	}
	
}


