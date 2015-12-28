#include <getopt.h>
#include <cstdlib>
#include <cstdio>

#include <string>
#include <vector>
using std::string;
using std::vector;

struct IpmtConfiguration {
	string mode;
	
	string indexType;
	string compression;
	int countFlag;

	vector<string> patterns;
	string patternFileName;
	
	string indexFileName;
		
	string textFileName;

	IpmtConfiguration() {
		indexType = "suffixtree";
		compression="dummy";
		mode="index";
		countFlag = 0;
	}

	bool validateConfig() {
	}

} config;

/*
* Configuração para a função getopt_long.
* Adaptado de 
* http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/
#define COMPRESSION 1
#define INDEXTYPE 2
#define PATTERN 3
struct option options[] =
{
	{"count", no_argument, &config.countFlag, 'c'},
	{"compression", required_argument, 0, COMPRESSION},
	{"indextype", required_argument, 0, INDEXTYPE},
	{"pattern", required_argument, 0, PATTERN},

//	{"", no_argument, &config.showAlignmentFlag, 1},
//	{"count-only",   no_argument, &config.countOnlyFlag, 1},
//	{"help",   no_argument, &config.showHelpMessageFlag, 1},
//	{"algorithm", required_argument, 0, 'a'},
//	{"edit", required_argument, 0, 'e'},
//	{"pattern", required_argument, 0, 'p'},
	{0, 0, 0, 0}
};

/*
* Análise dos parâmetros do programa (preenchimento dos campos de 'config')
* Adaptado de 
* * http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/
IpmtConfiguration& parseOptions(int argc, char* argv[]) {
	
	while(true){
	    int c = getopt_long (argc, argv, "c:", options, NULL);

	    if(c == -1) 
	    	break;
		if(c == COMPRESSION) {
			config.compression = optarg;	
		} else if(c == INDEXTYPE) {
			config.indexType = optarg;
		} else if(c == PATTERN) {
			config.patternFileName = optarg;
		} else if(c == 'c') {
			config.countFlag = 1;
		} else {
			exit(1);
		}
	}

	//parâmetros sem opções. Podem ser o padrão e os arquivos de texto
	if (optind < argc) {
		//Modo de operação
		config.mode = argv[optind++];
		
		if(config.mode == "index") {
			//o proximo valor deve ser o nome do arquivo de texto
			config.textFileName = argv[optind++];
		} else if(config.mode == "search") {
			//Usuário não informou uma fonte de padrões.
			//Então a próxima string é um padrão.
			if(config.patternFileName == ""){ 
				config.patterns.push_back(argv[optind++]);
			}
			config.indexFileName  = argv[optind++];
		} else {
			printf("Modo de operação não suportado: |%s|\n", config.mode.c_str());
			exit(1);	
		}
	}

	return config;
}

void printHelpMessage() {
}
