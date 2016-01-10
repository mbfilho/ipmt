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
#define DOT_FILE 5
#define COMPRESSION_LEVEL 6
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
	{"dotfile", required_argument, 0, DOT_FILE},
	{"level", required_argument, 0, COMPRESSION_LEVEL},
	{0, 0, 0, 0}
};

/*
* Análise dos parâmetros do programa (preenchimento dos campos de 'config')
* Adaptado de 
* * http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/
IpmtConfiguration& parseOptions(int argc, char* argv[]) {
	while(true){
	    int c = getopt_long (argc, argv, "cp:i:h", options, NULL);
	
	    if(c == -1) 
	    	break;
		if(c == COMPRESSION_LEVEL) {
			config.compressionLevel = atoi(optarg);
			config.isCompressionLevelSet = true;
		} else if(c == 'i') {
			config.interrupt = atoi(optarg);
		} else if(c == DOT_FILE) {
			config.dotFile = optarg;
		} else if(c == LZ77_BUFFER) {
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
	printf("###############################################################################\n");          
	printf("                          FUNCIONALIDADES BÁSICAS\n");
	printf("              \n");
	printf("Construção de um índice:\n");
	printf("    iptm index arquivo_de_texto.txt\n");
	printf("essa opção gera o arquivo de índice: arquivo_de_texto.txt.idx\n");
	printf("\n");
	printf("Busca de padrões em um índice:\n");
	printf("  ipmt search padrao arquivo_de_indice\n");
	printf("    ou\n");
	printf("  iptm search -p arquivo_de_padroes arquivo_de_indice\n");
	printf("###############################################################################\n");
	printf("                          OPÇÕES DISPONÍVEIS\n");
	printf("\n");
	printf("Para a construção de índice:\n");
	printf(" --indextype               Define o tipo de índice a ser criado. Os \n");
	printf("                           valores possíveis são: \n");
	printf("                           suffixtree (padrão), suffixtree2 e suffixarray\n");
	printf("\n");
	printf(" --compression             Define o algoritmo de compressão utilizado.\n");
	printf("                           Os valores possíveis são:\n");
	printf("                           lz77, lz78, lzw (padrão) e none\n");
	printf("\n");  
	printf(" --wl                      Define o tamanho da janela de lookahead\n"); 
	printf("                           do algoritmo lz77. Valor padrão: 1024\n");
	printf("\n");
	printf(" --wb                      Define o tamanho da janela de buffer\n");
	printf("                           do algoritmo lz77. Valor padrão: 16\n");
	printf("\n");
	printf(" --level                   Define a qualidade da compressão dos \n");
	printf("                           algoritmos lz78 e lzw. Os valores possíveis\n");
	printf("                           são:\n");
	printf("                               0 -> compressão mais rápida\n");
	printf("                               1 (padrão)\n");
	printf("                               2 -> melhor compressão \n");
	printf("\n");
	printf(" --dotfile                 Arquivo onde serão salvos as etapas\n");
	printf("                           intermediárias da construção da árvore de\n");
	printf("                           sufixos em formato dot.\n");
	printf("\n");
	printf("Para a busca de padrões:\n");
	printf("\n");
	printf(" -c, --count               A ferramenta apenas reporta a quantidade de \n");
	printf("                           ocorrências encontradas. Essa flag é desligada\n");
	printf("                           por padrão, ou seja, a ferramenta imprime\n");
	printf("                           as linhas das ocorrências por padrão.\n");
	printf("\n");
	printf(" -p, --pattern             Define um arquivo fonte de padrões, um em cada\n");
	printf("                           linha.\n");
	printf("Geral:\n");
	printf(" -h, --help                Exibe uma mensagem de ajuda\n");    					
}
