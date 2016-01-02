#include "SuffixArray.h"
#include "SuffixTree.h"
#include "IpmtConfiguration.h"
#include "OptionsParsing.cpp"
#include <cstring>
#include "DummyCompressor.h"
#include "DummyDecompressor.h"
#include "LZ78C.h"
#include "LZ78D.h"
#include "LZWD.h"
#include "LZWC.h"
#include "LZ77C.h"
#include "LZ77D.h"
#define SIZE (1<<27)

char buffer[SIZE];
char line[10000];

Index* loadIndex(IpmtConfiguration& config) {
	Index* loaded = NULL;
	Decompressor* decompressor = NULL;
	FILE* indexFile = fopen(config.indexFileName.c_str(), "rb"); 	

	if(!indexFile) {
		printf("Não foi possível abrir o arquivo \'%s\' para leitura.\n", config.indexFileName.c_str());
		assert(indexFile);
	}

	CompressionAlgorithm alg;
	IndexDataStructure index;
	assert(1 == fread(&index, sizeof(index), 1, indexFile));
	assert(1 == fread(&alg, sizeof(alg), 1, indexFile));

	if(alg == LZ77) {
		int wb, wl;
		assert(1 == fread(&wb, sizeof(int), 1, indexFile));	
		assert(1 == fread(&wl, sizeof(int), 1, indexFile));	
		decompressor = new LZ77D(indexFile, wb, wl);	
	} else if(alg == LZ78) {
		decompressor = new LZ78D(indexFile);
	} else if(alg == LZW) {
		decompressor = new LZWD(indexFile);
	} else if(alg == NONE) {
		decompressor = new DummyDecompressor(indexFile);
	}
	
	if(index == SUFFIX_ARRAY) {
		loaded = new SuffixArray();
	} else if(index == SUFFIX_TREE) {
		loaded = new SuffixTree(NULL);
	}
	Deserializer* deserializer = new Deserializer(decompressor);
	loaded->deserialize(deserializer);
	
	decompressor->close();
	
	delete deserializer;
	delete decompressor; 

	return loaded;
}

int main(int argc, char* argv[]){
	IpmtConfiguration& config = parseOptions(argc, argv);
	if(config.helpFlag) {
		printHelpMessage();
		exit(0);
	} else if(!config.validateConfig()) {
		printHelpMessage();
		exit(1);
	}
	
	printf("Mode %s\n", config.mode.c_str());

	Index* index = NULL;
	if(config.mode == "index") {
		if(config.indexType == "suffixtree") {
			printf("Usando suffixtree\n");
			index = new SuffixTree(NULL);
		} else if(config.indexType == "suffixarray") {
			index = new SuffixArray();
			printf("Usando suffixarray\n");
		}
		if(FILE* in = fopen(config.textFileName.c_str(), "r")){
			size_t read = fread(buffer, sizeof(char), SIZE-2, in);
			if(config.indexType == "suffixtree")
				buffer[read++] = '$';
			buffer[read] = 0;

			index->build(buffer, read);

			Serializer* serializer = new Serializer(config);
			index->serialize(serializer);
			serializer->flushAndClose();
			delete serializer;
		}else printf("O arquivo de texto \'%s\' não pôde ser aberto para leitura\n", config.textFileName.c_str());
	} else {
		index = loadIndex(config);
		for(int i = 0; i < config.patterns.size(); ++i){
			printf("Buscando padrão \'%s\' ...\n", config.patterns[i].c_str());
			index->findMatchings(config.patterns[i].c_str(), config.patterns[i].size(), config.countFlag);	
		}
	}
}




