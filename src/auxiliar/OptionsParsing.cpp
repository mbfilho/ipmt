#include <getopt.h>
#include <cstdlib>
#include <cstdio>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::vector;
using std::ifstream;
using std::getline;

struct IpmtConfiguration {
	string mode;
	
	string indexType;
	string compression;
	int countFlag;
	int helpFlag;

	vector<string> patterns;
	string patternFileName;
	
	string indexFileName;
		
	string textFileName;

	IpmtConfiguration() {
		indexType = "suffixtree";
		compression="dummy";
		countFlag = 0;
	}

	bool validateConfig() {
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

} config;

/*
* Configuração para a função getopt_long.
* Adaptado de 
* http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/
#define COMPRESSION 1
#define INDEXTYPE 2
struct option options[] =
{
	{"count", no_argument, &config.countFlag, 'c'},
	{"help", no_argument, &config.helpFlag, 'h'},
	{"compression", required_argument, 0, COMPRESSION},
	{"indextype", required_argument, 0, INDEXTYPE},
	{"pattern", required_argument, 0, 'p'},
	{0, 0, 0, 0}
};

/*
* Análise dos parâmetros do programa (preenchimento dos campos de 'config')
* Adaptado de 
* * http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/
IpmtConfiguration& parseOptions(int argc, char* argv[]) {
	
	while(true){
	    int c = getopt_long (argc, argv, "cp:", options, NULL);
	
	    if(c == -1) 
	    	break;
		if(c == COMPRESSION) {
			config.compression = optarg;	
		} else if(c == INDEXTYPE) {
			config.indexType = optarg;
		} else if(c == 'p') {
			config.patternFileName = optarg;
		} else if(c == 'c') {
			config.countFlag = 1;
		} else if(c == 'h') {
			config.helpFlag = 1;
			return config;//O usuário quer ajuda!  
		} 
	}

	//parâmetros sem opções. São esperados aqui o modo de operação, o arquivo de padrões, o arquivo de texto e o arquivo de índice.
	if (optind < argc) {
		//Modo de operação
		if(optind < argc)
			config.mode = argv[optind++];
		
		if(config.mode == "index") {
			//o proximo valor deve ser o nome do arquivo de texto
			if(optind < argc)
				config.textFileName = argv[optind++];
		} else if(config.mode == "search") {
			//Usuário não informou uma fonte de padrões.
			//Então a próxima string é um padrão.
			if(config.patternFileName == "" && optind < argc){ 
				config.patterns.push_back(argv[optind++]);
			}
			if(optind < argc)
				config.indexFileName  = argv[optind++];
		}
	}
	

	return config;
}

void printHelpMessage() {
	printf("TODO: Mensagem de ajuda\n");
}
