#include "LZWC.h"


LZWC::LZWC(const char* fileName) {
	currentNode = 0;
	trie.push_back(TrieNode());//Root
	for(int i = 0; i < 256; ++i) {
		trie.push_back(TrieNode());
		trie[0].addChild(i, trie.size()-1, trie.back());
	}

	bufferSize = 0;
	lastTokenSize = 0;

	file = fopen(fileName, "wb");
	if(!file){
		printf("[LZW Compressor] Não foi possível abrir o arquivo \'%s\' para escrita.\n", fileName);
		assert(file);
	}
}

void LZWC::writeInt(int arg) {
	ui u = ui(arg);
	for(int i = 0; i < 4; ++i) {
		writeByte(u & 0xff);	
		u >>= 8;
	}
}

void LZWC::writeArrayOfInts(const int* arg, int size) {
	for(int i = 0; i < size; ++i)
		writeInt(arg[i]);
}

void LZWC::writeText(const char* text, int size) {
	for(int i = 0; i < size; ++i)
		writeByte(text[i]+128);
}

void LZWC::writeByte(int arg) {
	int nextNode = trie[currentNode].getChild(arg, trie);
	if(nextNode == -1) {
		encodeInt(currentNode); //O indice do termo no dicionário
	
		//Insere o termo no dicionário	
		trie.push_back(TrieNode());
		trie[currentNode].addChild(arg, trie.size()-1, trie.back());
		
		currentNode = trie[0].getChild(arg, trie);
		if(currentNode == -1) printf("> %d\n", arg);
		assert(currentNode != -1);
	} else 
		currentNode = nextNode;
}


/*
* Estratégia:
* 	Seja a2 a representação binária de arg. A codificação de arg será:
* 		|a2|_1 + 0 + |a2|_2 + a2
* Tamanho dessa codificação (em bits):
* 	|a2| + log(|a2|) + 1 + log(|a2|) = O(|a2| + log(|a2|))
* ou, de maneira equivalente,
*	O(log(a2) + log log(a2))
*/
void LZWC::encodeInt(int arg) {
	int size = (arg == 0 ? 1 : 32 - __builtin_clz(arg));
	int sizeOfSize = (size == 0 ? 1 : 32 - __builtin_clz(size));
	ull token = 0; //token eh o monte de bits gerados da codificação de 'arg'
	
	//representação unária de size
	token = (1ULL<<sizeOfSize)-1;

	//separador '0' + representação (binária) de size. 
	token = token | (ull(size) << (sizeOfSize + 1));

	int tokenSize = 2*sizeOfSize+1;//o tamanho atual do token
	//acrescenta o 'arg' propriamente dito
	token = token | (ull(ui(arg)) << tokenSize);
	tokenSize += size;
	
	insertIntoBuffer(token, tokenSize);
}


void LZWC::insertIntoBuffer(ull token, int tokenSize){
	int filledbits = 0;
	if(bufferSize) {//verifica se pode 'encher' o último elemento do buffer
		ull& lastToken = buffer[bufferSize-1];
		
		lastToken |= token << lastTokenSize;//O comportamento é indeterminado quando lastTokenSize=64 (a.k.a não funciona e para achar o erro é difícil).
		
		filledbits = MIN(64 - lastTokenSize, tokenSize); //a quantidade de bits acrescentados ao último elemento
		lastTokenSize += filledbits;
		
		//descarta os bits que foram colocados no último elemento
		token >>= filledbits;
		tokenSize -= filledbits;
	}

	if(tokenSize) {
		buffer[bufferSize++] = token;
		lastTokenSize = tokenSize;
	}

	if(lastTokenSize == 64) {
		buffer[bufferSize++] = 0;
		lastTokenSize = 0;
	}
	
	if(bufferSize >= BUFFER_MAX_SIZE)
		flush();
}

/*
* Escreve todos os elementos do buffer no arquivo EXCETO o último, que ainda pode não ter todos seus bits preenchidos
*/
void LZWC::flush() {
	if(bufferSize < 1) return;
	assert(bufferSize - 1 == fwrite(buffer, sizeof(ull), bufferSize - 1, file));
	buffer[0] = buffer[bufferSize-1];
	bufferSize = 1;
}

void LZWC::flushAndClose() {
	if(currentNode != 0) {
		encodeInt(currentNode);
	}

	flush();
	if(bufferSize == 1) {
		assert(1 == fwrite(&buffer[0], sizeof(ull), 1, file));
	}

	fclose(file);
}


