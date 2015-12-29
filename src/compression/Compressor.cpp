#include "Compressor.h"


Compressor::Compressor(const char* fileName) {
	outputFile = fopen(fileName, "wb");
	if(!outputFile) {
		printf("Não foi possível abrir o arquivo \'%s\' para escrita\n", fileName);
		assert(outputFile);
	}

	outputBuffer = new ull[BUFFER_MAX_SIZE+1];
	outputBufferSize = 0;
	lastTokenSize = 0;

}

void Compressor::encodeAndWriteInt(int arg) {
	int size = (arg == 0 ? 1 : 32 - __builtin_clz(arg));
	int sizeOfSize = (size == 0 ? 1 : 32 - __builtin_clz(size));
	ull token = 0; //token eh o monte de bits gerados da codificação de 'arg'
	
	//representação unária de size
	token = (1ULL<<sizeOfSize)-1;

	//separador '0' + representação (binária) de size. 
	token = token | (ull(size) << (sizeOfSize + 1));

	int tokenSize = 2*sizeOfSize+1;//o tamanho atual do token
	//acrescenta o 'arg' propriamente dito
	token = token | (ull(ui(arg)) << tokenSize);
	tokenSize += size;
	
	insertIntoBuffer(token, tokenSize);
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
	for(int i = 0; i < size; ++i)
		writeByte(text[i]);
}

void Compressor::close() {
	fclose(outputFile);
}

/*
* Escreve todos os elementos do buffer no arquivo EXCETO o último, que ainda pode não ter todos seus bits preenchidos
*/
void Compressor::flush(bool force) {
	if(!outputBufferSize) return;
	if(outputBufferSize == 1 && !force) return;

	int toWrite = force ? outputBufferSize : outputBufferSize-1;
	assert(toWrite == fwrite(outputBuffer, sizeof(ull), toWrite, outputFile));

	if(!force) {
		outputBuffer[0] = outputBuffer[outputBufferSize-1];
		outputBufferSize = 1;
	}
}

void Compressor::insertIntoBuffer(ull token, int tokenSize){
	int filledbits = 0;
	if(outputBufferSize) {//verifica se pode 'encher' o último elemento do outputBuffer
		ull& lastToken = outputBuffer[outputBufferSize-1];
		
		lastToken |= token << lastTokenSize;//O comportamento é indeterminado quando lastTokenSize=64 (a.k.a não funciona e para achar o erro é difícil).
		
		filledbits = MIN(64 - lastTokenSize, tokenSize); //a quantidade de bits acrescentados ao último elemento
		lastTokenSize += filledbits;
		
		//descarta os bits que foram colocados no último elemento
		token >>= filledbits;
		tokenSize -= filledbits;
	}

	if(tokenSize) {
		outputBuffer[outputBufferSize++] = token;
		lastTokenSize = tokenSize;
	}

	if(lastTokenSize == 64) {
		outputBuffer[outputBufferSize++] = 0;
		lastTokenSize = 0;
	}
	
	if(outputBufferSize >= BUFFER_MAX_SIZE)
		flush(false);
}


