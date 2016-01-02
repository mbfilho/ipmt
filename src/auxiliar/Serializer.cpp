#include "Serializer.h"

Serializer::Serializer(IpmtConfiguration& config) {
	CompressionAlgorithm alg = config.getCompressionAlgorithm();
	IndexDataStructure index = config.getIndexDataStructure();

	string indexFileName = config.textFileName + ".idx";
	FILE* output = fopen(indexFileName.c_str(), "wb");
	if(!output) {
		printf("Não foi possível abrir o arquivo \'%s\' para escrita\n", indexFileName.c_str());
		assert(output);
	}
	
	fwrite(&index, sizeof(index), 1, output);
	fwrite(&alg, sizeof(alg), 1, output);
	
	if(alg == LZ77) {
		int args[] = {config.wb, config.wl};
		fwrite(args, sizeof(int), 2, output);
	}

	compressor = NULL;	
	switch(alg) {
		case LZ77:
			compressor = new LZ77C(output, config.wb, config.wl);
			break;
		case LZ78:
			compressor = new LZ78C(output);
			break;
		case LZW:
			compressor = new LZWC(output);
			break;
		case NONE:
			compressor = new DummyCompressor(output);
			break;
	}

	assert(compressor != NULL);
}

void Serializer::serializeInt(int value) {
	int encodedSize;
	ull encodedInt = encodeInt(value, &encodedSize);
	writeToCompressor(encodedInt, encodedSize);
}

void Serializer::serializeChar(char ch) {
	writeToCompressor(ch, 8);	
}

ull Serializer::encodeInt(int arg, int *encodedSize) {
	int size = SIZE_IN_BITS(arg);
	int sizeOfSize = SIZE_IN_BITS(size);

	//token eh o monte de bits gerados da codificação de 'arg'	
	ull token = 
		(1ULL<<sizeOfSize)-1 //representação unária de size
		| (ull(size) << (sizeOfSize + 1))//separador '0' + representação (binária) de size.
		| (ull(ui(arg)) << (2*sizeOfSize+1)); //acrescenta o 'arg' propriamente dito

	*encodedSize = 2*sizeOfSize+1+size;
	return token;
}

void Serializer::writeToCompressor(ull token, int tokenSize) {
	lastToken |= (token << lastTokenSize);
	int filledBits = MIN(64 - lastTokenSize, tokenSize);
	token >>= filledBits;
	tokenSize -= filledBits;
	if(lastTokenSize == 64) {
		compressor->writeInt(int(lastToken));
		lastToken >>= 32;
		compressor->writeInt(int(lastToken));
		lastToken = 0;
		lastTokenSize = 0;
	}

	if(tokenSize) {
		lastToken = token;
		lastTokenSize = tokenSize;
	}
}

void Serializer::flushAndClose() {
	compressor->flushAndClose();
}

