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

void LZWC::feedRawByte(Byte arg) {
	int nextNode = hashTable->get(make_pair(currentNode, arg));
	if(nextNode == -1) {
		encodeAndWrite(currentNode); //O indice do termo no dicionário
	
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

void LZWC::encodeAndWrite(int node) {
	int size = SIZE_IN_BITS(node), encodedSize;	
	ull token = encodeInt(node, size, &encodedSize);
	writeTokenToFile(token, encodedSize);
}

void LZWC::onClosing() {
	if(currentNode != 0) 
		encodeAndWrite(currentNode);
}


