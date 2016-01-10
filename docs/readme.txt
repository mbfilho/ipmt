
*******************************************************************************
			Breve introdução ao uso da ferramenta ipmt
por:
	Jamerson Lima
	Márcio Barbosa
*******************************************************************************

###############################################################################
							INSTALAÇÃO

Acesso o diretório ipmt por um terminal e execute o comando:
	make
um diretório ipmt/bin será criado e conterá a ferramenta ipmt


###############################################################################
							FUNCIONALIDADES BÁSICAS
							
Construção de um índice:
		iptm index arquivo_de_texto.txt
	essa opção gera o arquivo de índice: arquivo_de_texto.txt.idx

Busca de padrões em um índice:
	ipmt search padrao arquivo_de_indice
		ou
	iptm search -p arquivo_de_padroes arquivo_de_indice



###############################################################################
							OPÇÕES DISPONÍVEIS

Para a construção de índice:
    --indextype						   Define o tipo de índice a ser criado. Os 
									   valores possíveis são: 
									      suffixtree (padrão), suffixtree2 e suffixarray

	--compression					   Define o algoritmo de compressão utilizado.
									   Os valores possíveis são:
									  	  lz77, lz78, lzw (padrão) e none
	
	--wl							   Define o tamanho da janela de lookahead 
									   do algoritmo lz77. Valor padrão: 1024

	--wb							   Define o tamanho da janela de buffer
									   do algoritmo lz77. Valor padrão: 16
										
	--level							   Define a qualidade da compressão dos 
									   algoritmos lz78 e lzw. Os valores possíveis
									   são:
									   	  	0 -> compressão mais rápida
											1 (padrão)
											2 -> melhor compressão 
									   
	--dotfile						   Arquivo onde serão salvos as etapas
									   intermediárias da construção da árvore de
									   sufixos em formato dot.

Para a busca de padrões:

	-c, --count						   A ferramenta apenas reporta a quantidade de 
									   ocorrências encontradas. Essa flag é desligada
									   por padrão, ou seja, a ferramenta imprime
									   as linhas das ocorrências por padrão.

	-p, --pattern					   Define um arquivo fonte de padrões, um em cada
									   linha. 

Geral:
	-h, --help						  Exibe uma mensagem de ajuda									


