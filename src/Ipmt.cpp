#include "SuffixArray.h"
#include "SuffixTree.h"
#include "SuffixTree2.h"
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
		perror("Erro:");
		assert(indexFile);
	}

	CompressionAlgorithm alg;
	IndexDataStructure index;

	assert(1 == fread(&index, sizeof(IndexDataStructure), 1, indexFile));
	assert(1 == fread(&alg, sizeof(CompressionAlgorithm), 1, indexFile));

	if(alg == LZ77) {
		int wb, wl;
		assert(1 == fread(&wb, sizeof(int), 1, indexFile));	
		assert(1 == fread(&wl, sizeof(int), 1, indexFile));	
		decompressor = new LZ77D(indexFile, wb, wl);	
		printf("Algoritmo de compressão: LZ77, com buffer %d e lookahead %d.\n", wb, wl);
	} else if(alg == LZ78) {
		int level;
		assert(1 == fread(&level, sizeof(int), 1, indexFile));
		decompressor = new LZ78D(indexFile, level);
		printf("Algoritmo de compressão: LZ78 com nível de compressão %d.\n", level);
	} else if(alg == LZW) {
		int level;
		assert(1 == fread(&level, sizeof(int), 1, indexFile));
		decompressor = new LZWD(indexFile, level);
		printf("Algoritmo de compressão: LZW com nível de compressão %d.\n", level);
	} else if(alg == NONE) {
		decompressor = new DummyDecompressor(indexFile);
		printf("Algoritmo de compressão: nenhum.\n");
	}
	
	if(index == SUFFIX_ARRAY) {
		loaded = new SuffixArray();
		printf("Tipo do índice armazenado: Array de Sufixos.\n");
	} else if(index == SUFFIX_TREE) {
		loaded = new SuffixTree(NULL);
		printf("Tipo do índice armazenado: Árvore de Sufixos.\n");
	} else if(index == SUFFIX_TREE2) {
		loaded = new SuffixTree2(NULL);
		printf("Tipo do índice armazenado: Árvore de Sufixos [2].\n");
	}

	assert(loaded != NULL); assert(decompressor != NULL);
	loaded->deserialize(decompressor);
	
	decompressor->close();
	
	delete decompressor; 

	return loaded;
}

void storeIndex(IpmtConfiguration& config, Index* index) {
	CompressionAlgorithm alg = config.getCompressionAlgorithm();
	IndexDataStructure indexType = config.getIndexDataStructure();
	
	string indexFileName = config.textFileName + ".idx";
	FILE* output = fopen(indexFileName.c_str(), "wb");
	if(!output) {
		printf("Não foi possível abrir o arquivo \'%s\' para escrita\n", indexFileName.c_str());
		assert(output);
	}
	fwrite(&indexType, sizeof(IndexDataStructure), 1, output);
	fwrite(&alg, sizeof(CompressionAlgorithm), 1, output);
	
	if(alg == LZ77) {
		int args[] = {config.wb, config.wl};
		fwrite(args, sizeof(int), 2, output);
	}
	if(alg == LZ78 || alg == LZW) {
		fwrite(&(config.compressionLevel), sizeof(int), 1, output);
	}

	Compressor* compressor = NULL;	
	switch(alg) {
		case LZ77:
			compressor = new LZ77C(output, config.wb, config.wl);
			printf("Comprimindo índice com o algoritmo LZ77, com buffer = %d e lookahead %d.\n", config.wb, config.wl);
			break;
		case LZ78:
			compressor = new LZ78C(output, config.compressionLevel);
			printf("Comprimindo índice com o algoritmo LZ78 com nível de compressão %d.\n", config.compressionLevel);
			break;
		case LZW:
			compressor = new LZWC(output, config.compressionLevel);
			printf("Comprimindo índice com o algoritmo LZW com nível de compressão %d.\n", config.compressionLevel);
			break;
		case NONE:
			compressor = new DummyCompressor(output);
			printf("Salvando o índice descomprimido.\n");
			break;
	}
	assert(compressor != NULL);

	index->serialize(compressor);
	compressor->flushAndClose();
		
	delete compressor;
}

int main(int argc, char* argv[]){
	IpmtConfiguration& config = parseOptions(argc, argv);
	if(config.helpFlag) {
		printHelpMessage();
		exit(0);
	} else if(!config.validateConfig()) {
		printf("Execute 'ipmt --help' para mais informações de uso.\n");
		exit(1);
	}
	
	printf("Mode %s\n", config.mode.c_str());

	Index* index = NULL;
	if(config.mode == "index") {
		IndexDataStructure indexType = config.getIndexDataStructure();
		if(indexType == SUFFIX_TREE2) {
			printf("Construindo Árvore de Sufixos [2] ...\n");
			const char* dotFile = NULL;
			if(config.dotFile != "")
				dotFile = config.dotFile.c_str();
			index = new SuffixTree2(dotFile);
		} else if(indexType == SUFFIX_TREE) {
			printf("Construindo Árvore de Sufixos ...\n");
			const char* dotFile = NULL;
			if(config.dotFile != "")
				dotFile = config.dotFile.c_str();
			index = new SuffixTree(dotFile);
		} else if(indexType == SUFFIX_ARRAY) {
			index = new SuffixArray();
			printf("Construindo Array de Sufixos ...\n");
		}

		if(FILE* in = fopen(config.textFileName.c_str(), "r")){
			size_t read = fread(buffer, sizeof(char), SIZE-2, in);
			if(buffer[read-1] == '\n') --read;
			if(indexType == SUFFIX_TREE || indexType == SUFFIX_TREE2)
				buffer[read++] = 0; //adiciona um caractere que não aprece no texto. Vai que o '$' aparece ...
			buffer[read] = 0;
			
			index->build(buffer, read);
			printf("Índice construído. Serializando ...\n");
			if(config.interrupt == 0) {
				printf("Interrompendo após construção\n");
				return 0;
			}
			storeIndex(config, index);
			if(config.interrupt == 1) {
				printf("Interrompendo após compressão\n");
				return 0;
			}

		}else printf("O arquivo de texto \'%s\' não pôde ser aberto para leitura\n", config.textFileName.c_str());
		
	} else {
		index = loadIndex(config);
		if(config.interrupt == 2) {
			printf("Interrompendo após descompressão\n");
			return 0;
		}
		for(int i = 0; i < config.patterns.size(); ++i){
			printf("Buscando padrão \'%s\' ...\n", config.patterns[i].c_str());
			index->findMatchings(config.patterns[i].c_str(), config.patterns[i].size(), config.countFlag);	
		}
	}
}




