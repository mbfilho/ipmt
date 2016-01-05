#include "LZ78C.h"

LZ78C::LZ78C(FILE* output, int compressionLevel): Compressor(output) {
	currentNode = 0;
	dictionarySize = 1;
	hashTable = new HashTable();

	if(compressionLevel == 0) 
		dictionaryMaxSize = 1 << 16;
	else if(compressionLevel == 1)
		dictionaryMaxSize = 1 << 19;
	else
		dictionaryMaxSize = 1 << 30; //infinito
}

void LZ78C::feedRawByte(Byte arg) {
	int nextNode = hashTable->get(make_pair(currentNode, arg));
	if(nextNode == -1) {
		int nodeEncodeSize;
		ull tokenNode = encodeInt(currentNode, SIZE_IN_BITS(currentNode), &nodeEncodeSize);
		
		int sizeOfUncompressedSeq = 1 + 10 * sequence.size(); //(2 + sequence.size()) + 8 * sequence.size() + 8;
	
		//Verifica se vale a pena mandar a tupla codificada ao invés da sequência descodificada
		if(1 + nodeEncodeSize + 8 < sizeOfUncompressedSeq) {
			writeTokenToFile(1, 1);
			writeTokenToFile(tokenNode, nodeEncodeSize);//O indice do termo no dicionário
			writeTokenToFile(arg, 8);//O índice do elemento 'mismatching', não codificado	
		} else {
			writeTokenToFile(1ULL << (1+sequence.size()), sequence.size()+2);

			for(int i = 0; i < sequence.size(); ++i) 
				writeTokenToFile(sequence[i], 8);
			writeTokenToFile(arg, 8); 
		}

		//Insere o termo no dicionário	
		hashTable->put(make_pair(currentNode, arg), dictionarySize);
		++dictionarySize;

		if(dictionarySize >= dictionaryMaxSize) {
			currentNode = 0;
			dictionarySize = 1;
			hashTable->clear();
		}

		sequence.clear();	
		currentNode = 0;
	} else {
		currentNode = nextNode;
		sequence.push_back(arg);
	}
}

void LZ78C::encodeAndWrite(int arg) {
	int argSize = SIZE_IN_BITS(arg), encodedSize;
	ull token = encodeInt(arg, argSize, &encodedSize);
	writeTokenToFile(token, encodedSize);
}

void LZ78C::onClosing() {
	if(currentNode != 0) {
		writeTokenToFile(1,1);
		encodeAndWrite(currentNode);
		writeTokenToFile(0,8);//Don't care
	}
}


