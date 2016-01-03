#include "LZ78C.h"

LZ78C::LZ78C(FILE* output): Compressor(output) {
	currentNode = 0;
	dictionarySize = 1;
	hashTable = new HashTable();
}

void LZ78C::writeByte(int arg) {
	int nextNode = hashTable->get(make_pair(currentNode, arg));
	if(nextNode == -1) {
		encodeAndWriteInt(currentNode); //O indice do termo no dicionário
		encodeAndWriteInt(arg); //O índice do elemento 'mismatching'	
	
		//Insere o termo no dicionário	
		hashTable->put(make_pair(currentNode, arg), dictionarySize);
		++dictionarySize;

		if(dictionarySize >= (1<<19) ) {
			currentNode = 0;
			dictionarySize = 1;
			hashTable->clear();
		}
		
		currentNode = 0;
	} else 
		currentNode = nextNode;
}


void LZ78C::flushAndClose() {
	flushInput();

	if(currentNode != 0) {
		encodeAndWriteInt(currentNode);
		encodeAndWriteInt(0); //Don't care
	}

	flushOutput();
	close();
}


