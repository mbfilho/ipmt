#include "LZ78C.h"

LZ78C::LZ78C(FILE* output): Compressor(output) {
	currentNode = 0;
	dictionarySize = 1;
	hashTable = new HashTable();
}

void LZ78C::feedRawByte(Byte arg) {
	int nextNode = hashTable->get(make_pair(currentNode, arg));
	if(nextNode == -1) {

		encodeAndWrite(currentNode); //O indice do termo no dicionário
		encodeAndWrite(arg); //O índice do elemento 'mismatching'	
	
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

void LZ78C::encodeAndWrite(int arg) {
	int argSize = SIZE_IN_BITS(arg), encodedSize;
	ull token = encodeInt(arg, argSize, &encodedSize);
	writeTokenToFile(token, encodedSize);
}

void LZ78C::onClosing() {
	if(currentNode != 0) {
		encodeAndWrite(currentNode);
		encodeAndWrite(0); //Don't care
	}
}


