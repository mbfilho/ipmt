#include "SuffixArray.h"
#include "SuffixArray2.h"
#include "SuffixTree.h"
#include "OptionsParsing.cpp"
#include <cstring>
#include "DummyCompressor.h"
#include "DummyDecompressor.h"

#define SIZE (1<<27)

char buffer[SIZE];
char line[10000];

int main(int argc, char* argv[]){
	IpmtConfiguration& config = parseOptions(argc, argv);
	
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
			char indexFileName[100];
			strcpy(indexFileName, config.textFileName.c_str());
			strcat(indexFileName, ".idx");
			Compressor* compressor = new DummyCompressor(indexFileName);
			index->compress(compressor);
		}else printf("O arquivo de texto \'%s\' não pôde ser aberto para leitura\n", config.textFileName.c_str());
	} else {
		char indexFileName[100];
		strcpy(indexFileName, config.textFileName.c_str());
		strcat(indexFileName, ".idx");
		Decompressor* decompressor = new DummyDecompressor(indexFileName);	
		index = new SuffixTree(NULL);
		index->decompress(decompressor);
		for(int i = 0; i < config.patterns.size(); ++i){
			index->findMatchings(config.patterns[i].c_str(), config.patterns[i].size(), config.countFlag);	
		}
	}
}

