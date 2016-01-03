#include "LZWC.h"

LZWC::LZWC(FILE* output):Compressor(output) {
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
		if(size >= (1<<19)) {
			hashTable->clear();
			size = 1;
			for(int i = 0; i < 256; ++i) {
				hashTable->put(make_pair(0,i), size);
				size++;
			}
		}
	
		currentNode = hashTable->get(make_pair(0,arg));
	} else 
		currentNode = nextNode;
}

void LZWC::flushAndClose() {
	flushInput();

	if(currentNode != 0) {
		encodeAndWriteInt(currentNode);
	}

	flushOutput();
	close();
}


