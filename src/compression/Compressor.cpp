#include "Compressor.h"


Compressor::Compressor(FILE* output) {
	outputFile = output;
	
	lastTokenSize = 0;
	lastToken = 0;

	unfinishedByte = 0;
	unfinishedByteSize = 0;
}

void Compressor::feedRawBits(ull bits, int howMany) {
	unfinishedByte |= Byte(bits << unfinishedByteSize);
	int filledBits = MIN(8 - unfinishedByteSize, howMany);
	howMany -= filledBits;
	bits >>= filledBits;
	unfinishedByteSize += filledBits;
	
	if(unfinishedByteSize == 8) {
		feedRawByte(unfinishedByte);
		unfinishedByte = 0;
		unfinishedByteSize = 0;
	}

	while(howMany >= 8) {
		feedRawByte(Byte(bits));
		bits >>= 8;
		howMany -= 8;
	}

	if(howMany) {
		unfinishedByte = bits;
		unfinishedByteSize = howMany;
	}
}

void Compressor::flushAndClose() {
	//Flush input: envia um byte 'quebrado' para ser comprimido
	if(unfinishedByteSize)
		feedRawByte(unfinishedByte);
	
	onClosing();
	
	if(lastTokenSize)
		fwrite(&lastToken, sizeof(ull), 1, outputFile);
	
	fclose(outputFile);
}

void Compressor::writeTokenToFile(ull token, int tokenSize) {
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


