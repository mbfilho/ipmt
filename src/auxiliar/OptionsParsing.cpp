#include "Global.h"
#include "IpmtConfiguration.h"

/*
* Configuração para a função getopt_long.
* Adaptado de 
* http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/

IpmtConfiguration config;

#define COMPRESSION 1
#define INDEXTYPE 2
#define LZ77_BUFFER 3
#define LZ77_LOOKA 4
struct option options[] =
{
	{"count", no_argument, &config.countFlag, 'c'},
	{"help", no_argument, &config.helpFlag, 'h'},
	{"compression", required_argument, 0, COMPRESSION},
	{"indextype", required_argument, 0, INDEXTYPE},
	{"wb", required_argument, 0, LZ77_BUFFER},
	{"wl", required_argument, 0, LZ77_LOOKA},
	{"pattern", required_argument, 0, 'p'},
	{"interrupt", required_argument, 0, 'i'},
	{0, 0, 0, 0}
};

/*
* Análise dos parâmetros do programa (preenchimento dos campos de 'config')
* Adaptado de 
* * http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/
IpmtConfiguration& parseOptions(int argc, char* argv[]) {
	while(true){
	    int c = getopt_long (argc, argv, "cp:i:", options, NULL);
	
	    if(c == -1) 
	    	break;
		if(c == 'i') {
			config.interrupt = atoi(optarg);
		}else if(c == LZ77_BUFFER) {
			config.wb = atoi(optarg);
		} else if(c == LZ77_LOOKA) {
			config.wl = atoi(optarg);
		} else if(c == COMPRESSION) {
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
