#include "IpmtConfiguration.h"

IpmtConfiguration::IpmtConfiguration() {
	indexType = "suffixtree";
	compression="none";
	countFlag = 0;
	wb = 1024;
	wl = 16;
}

CompressionAlgorithm IpmtConfiguration::getCompressionAlgorithm() {
	if(compression == "none")
		return CompressionAlgorithm::NONE;
	else if(compression == "lz77")
		return CompressionAlgorithm::LZ77;
	else if(compression == "lz78")
		return CompressionAlgorithm::LZ78;
	else
		return CompressionAlgorithm::LZW;
}

IndexDataStructure IpmtConfiguration::getIndexDataStructure() {
	if(indexType == "suffixtree")
		return IndexDataStructure::SUFFIX_TREE;
	return IndexDataStructure::SUFFIX_ARRAY;
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
		printf("TODO: validar compressão\n");

		if(textFileName == "") {
			printf("No modo \'index\' é preciso informar um arquivo de texto a partir do qual o índice será construído.\n");
			valid = false;
		}
	}

	return valid;
}
