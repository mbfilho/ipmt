#include "LZWC.h"

LZWC::LZWC(const char* fileName):Compressor(fileName) {
	currentNode = 0;
	size = 1;
	hashTable = new HashTable();
	for(int i = 0; i < 256; ++i) {
		hashTable->put(make_pair(0,i), size);
		size++;
	}
}

void LZWC::writeByte(int arg) {
	int nextNode = hashTable->get(make_pair(currentNode, arg));
	if(nextNode == -1) {
		encodeAndWriteInt(currentNode); //O indice do termo no dicionário
	
		//Insere o termo no dicionário	
		hashTable->put(make_pair(currentNode, arg), size);
		size++;
	
		currentNode = hashTable->get(make_pair(0,arg));
	} else 
		currentNode = nextNode;
}

void LZWC::flushAndClose() {
	if(currentNode != 0) {
		encodeAndWriteInt(currentNode);
	}

	flush(true);
	close();
}


