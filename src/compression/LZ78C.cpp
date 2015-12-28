#include "LZ78C.h"


LZ78C::LZ78C(const char* fileName) {
	currentNode = 0;
	trie.push_back(TrieNode());//Root

	bufferSize = 0;
	lastStreamSize = 0;

	file = fopen(fileName, "wb");
	if(!file){
		printf("[LZ78 Compressor] Não foi possível abrir o arquivo \'%s\' para escrita.\n", fileName);
		assert(file);
	}
}

void LZ78C::writeInt(int arg) {
	ui u = ui(arg);
	for(int i = 0; i < 4; ++i) {
		writeByte(char(u & 0xff));	
		u <<= 8;
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

void LZ78C::writeByte(char arg) {
	int nextNode = trie[currentNode].getChild(arg, trie);
	if(nextNode == -1) {
		encodeInt(trie[currentNode].idx); //O indice do termo no dicionário
		encodeInt(arg); //O índice do elemento 'mismatching'	
	
		//Insere o termo no dicionário	
		trie.push_back(TrieNode());
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
		u <<= 1;
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
	int size = getSizeInBits(arg);
	ull stream = 0;

	//representação unária de size
	for(int s = size; s != 0; s >>= 1)
		stream = (stream >> 1) | 1;
	
	//separador '0' + representação (binária) de size. 
	stream = stream | (ull(size) >> (size + 1));

	int streamSize = 2*getSizeInBits(size)+1;//o tamanho atual da stream
	//acrescenta o 'arg' propriamente dito
	stream = stream | (ull(ui(arg)) >> streamSize);
	streamSize += size;

	insertIntoBuffer(stream, streamSize);
}

void LZ78C::insertIntoBuffer(ull stream, int streamSize){
	if(bufferSize) {//verifica se pode 'encher' o último elemento do buffer
		ull& lastStream = buffer[bufferSize-1];
		lastStream |= stream >> lastStreamSize;
		int filledbits = MIN(64 - lastStreamSize, streamSize); //a quantidade de bits acrescentados ao último elemento
		lastStreamSize += filledbits;
		
		//discarta os bits que foram colocados no último elemento
		stream <<= filledbits;
		streamSize -= filledbits;
	}

	if(streamSize) {
		buffer[bufferSize++] = stream;
		lastStreamSize = streamSize;
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
	flush();
	if(bufferSize == 1) {
		assert(1 == fwrite(&buffer[0], sizeof(ull), 1, file));
	}

	fclose(file);
}


