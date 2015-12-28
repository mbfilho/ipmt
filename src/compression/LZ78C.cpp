#include "LZ78C.h"


LZ78C::LZ78C(const char* fileName) {
	currentNode = 0;
	trie.push_back(TrieNode());//Root
	trie.back().idx = 0;
	dicionarySize = 1;

	bufferSize = 0;
	lastTokenSize = 0;

	file = fopen(fileName, "wb");
	if(!file){
		printf("[LZ78 Compressor] Não foi possível abrir o arquivo \'%s\' para escrita.\n", fileName);
		assert(file);
	}
}

void LZ78C::writeInt(int arg) {
	ui u = ui(arg);
	for(int i = 0; i < 4; ++i) {
		writeByte(u & 0xff);	
		u >>= 8;
	}
}

void LZ78C::writeArrayOfInts(const int* arg, int size) {
	for(int i = 0; i < size; ++i)
		writeInt(arg[i]);
}

void LZ78C::writeText(const char* text, int size) {
	for(int i = 0; i < size; ++i)
		writeByte(text[i]);
}

void LZ78C::writeByte(int arg) {
	int nextNode = trie[currentNode].getChild(arg, trie);
	if(nextNode == -1) {
		encodeInt(trie[currentNode].idx); //O indice do termo no dicionário
		encodeInt(arg); //O índice do elemento 'mismatching'	
	
		//Insere o termo no dicionário	
		trie.push_back(TrieNode());
		trie.back().idx = dicionarySize++;
		trie[currentNode].addChild(arg, trie.size()-1, trie.back());
		
		currentNode = 0;
	} else 
		currentNode = nextNode;
}

int LZ78C::getSizeInBits(int arg) {
	ui u = ui(arg);
	if(!u) return 1;

	int size = 0;
	while(u) {
		++size;
		u >>= 1;
	}

	return size;
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
void LZ78C::encodeInt(int arg) {
	int size = (arg == 0 ? 1 : 32 - __builtin_clz(arg));
	int sizeOfSize = (size == 0 ? 1 : 32 - __builtin_clz(size));
	ull token = 0; //token eh o monte de bits gerados da codificação de 'arg'
	
	//representação unária de size
	for(int i = 0; i < sizeOfSize; ++i)
		token = (token << 1) | 1;

	//separador '0' + representação (binária) de size. 
	token = token | (ull(size) << (sizeOfSize + 1));

	int tokenSize = 2*sizeOfSize+1;//o tamanho atual do token
	//acrescenta o 'arg' propriamente dito
	token = token | (ull(ui(arg)) << tokenSize);
	tokenSize += size;
	
	insertIntoBuffer(token, tokenSize);
}

void printBinaryFormat(ull a, int size) {
	for(int i = 0; i < size; ++i){
		printf("%llu", a & 1);
		a >>= 1;
	}
	printf("\n");
}

void LZ78C::insertIntoBuffer(ull token, int tokenSize){
	if(bufferSize) {//verifica se pode 'encher' o último elemento do buffer
		ull& lastToken = buffer[bufferSize-1];

		lastToken |= token << lastTokenSize;
		int filledbits = MIN(64 - lastTokenSize, tokenSize); //a quantidade de bits acrescentados ao último elemento
		lastTokenSize += filledbits;
		
		//discarta os bits que foram colocados no último elemento
		token <<= filledbits;
		tokenSize -= filledbits;
	}

	if(tokenSize) {
		buffer[bufferSize++] = token;
		lastTokenSize = tokenSize;
	}
	
	if(bufferSize >= BUFFER_MAX_SIZE)
		flush();
}

/*
* Escreve todos os elementos do buffer no arquivo EXCETO o último, que ainda pode não ter todos seus bits preenchidos
*/
void LZ78C::flush() {
	if(bufferSize < 1) return;

	assert(bufferSize - 1 == fwrite(buffer, sizeof(ull), bufferSize - 1, file));
	buffer[0] = buffer[bufferSize-1];
	bufferSize = 1;
}

void LZ78C::flushAndClose() {
	if(currentNode != 0) {
		encodeInt(trie[currentNode].idx);
		encodeInt(256);//Um caracter imaginário fora do alfabeto
	}
	flush();
	if(bufferSize == 1) {
		assert(1 == fwrite(&buffer[0], sizeof(ull), 1, file));
	}

	fclose(file);
}


