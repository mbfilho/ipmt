#include "IpmtConfiguration.h"

IpmtConfiguration::IpmtConfiguration() {
	indexType = "suffixtree";
	compression="lzw";
	countFlag = 0;
	wb = 1024;
	wl = 16;
	interrupt = -1;
	compressionLevel = 1;	
	isCompressionLevelSet = false;
}

CompressionAlgorithm IpmtConfiguration::getCompressionAlgorithm() {
	if(compression == "none")
		return NONE;
	else if(compression == "lz77")
		return LZ77;
	else if(compression == "lz78")
		return LZ78;
	else
		return LZW;
}

IndexDataStructure IpmtConfiguration::getIndexDataStructure() {
	if(indexType == "suffixtree")
		return SUFFIX_TREE;
	return SUFFIX_ARRAY;
}

bool IpmtConfiguration::validateConfig(){
	bool valid = true;

	if(mode != "search" && mode != "index") {
		printf("Modo de operação \'%s\' não suportado. Você quis dizer \'search\' ou \'index\'?\n", mode.c_str());
		return false;
	}

	if(mode == "search"){
		if(indexFileName == "") {
			printf("No modo \'search\' é preciso informar um arquivo que contenha um índice previamente criado.");
			valid = false;
		}

		if(patternFileName == "" && patterns.size() == 0) {
			printf("No modo \'search\' é preciso fornecer um padrão ou um arquivo contendo padrões\n");
			valid = false;
		}
		if(patternFileName != ""){
			ifstream patsFile(patternFileName.c_str());
			if(!patsFile.good()) {
				printf("Não foi possível abrir o arquivo de padrões \'%s\' para leitura\n", patternFileName.c_str());
				valid = false;
			} else {
				while(patsFile.good()){
					string line;
					getline(patsFile, line);
					if(line.size() > 0)
						patterns.push_back(line);	
				}

				patsFile.close();
				if(patterns.size() == 0){
					printf("Aparentemente o arquivo de padrões \'%s\' está vazio.\n", patternFileName.c_str());
					valid = false;
				}
			}
		}

	} else {
		if(indexType != "suffixtree" && indexType != "suffixarray") {
			printf("Indice não suportado: \'%s\'. Você quis dizer \'suffixtree\' ou \'suffixarray\'?\n", indexType.c_str());
			valid = false;
		}
	
		if(compression != "lz77" && compression != "lz78" && compression != "lzw" && compression != "none") {
			printf("Algoritmo de compressão não suportado: \'%s\'. Você quis dizer \'lz77\', \'lz78\', \'lzw\' ou \'none\'?\n", compression.c_str());
			valid = false;
		}

		if(isCompressionLevelSet) {
			if(compression != "lz78" && compression != "lzw" ) {
				printf("Atenção! A opção \'level\' só está disponível para os algoritmos de compressão lz78 e lzw. Ela será, portanto, ignorada.\n");
			} else if(compressionLevel < 0 || compressionLevel > 2) {
				printf("Para configurar a eficácia e rapidez da compressão utilize a opção \'level\' com um dos valores:\n* 0 (compressão mais rápida e menos eficaz)\n* 1 (nível moderado de compressão e rapidez)\n* 2 (compressão mais lenta e eficaz)\n");
				valid = false;
			}
		}

		if(textFileName == "") {
			printf("No modo \'index\' é preciso informar um arquivo de texto a partir do qual o índice será construído.\n");
			valid = false;
		}
	}

	return valid;
}
