#include "LZWC.h"

LZWC::LZWC(FILE* output, int compressionLevel):Compressor(output) {
	currentNode = 0;
	size = 1;
	hashTable = new HashTable();
	for(int i = 0; i < 256; ++i) {
		hashTable->put(make_pair(0,i), size);
		size++;
	}

	if(compressionLevel == 0) 
		dictionaryMaxSize = 1 << 16;
	else if(compressionLevel == 1)
		dictionaryMaxSize = 1 << 19;
	else
		dictionaryMaxSize = 1 << 30; //infinito
}

void LZWC::feedRawByte(Byte arg) {
	int nextNode = hashTable->get(make_pair(currentNode, arg));
	if(nextNode == -1) {
		int encodedNodeSize;
		ull token = encodeInt(currentNode, SIZE_IN_BITS(currentNode), &encodedNodeSize);
	
		int sizeOfUncompressedSeq = 1 + 9*seq.size(); //(1 + seq.size()) + 8 * seq.size();
		if(1 + encodedNodeSize <= sizeOfUncompressedSeq){ //verifica se vale a pena codificar os tokens ou enviar a sequência descomprimida
			writeTokenToFile(1, 1);
			writeTokenToFile(token, encodedNodeSize);
		}else {
			writeTokenToFile(1ULL << (seq.size()), 1+seq.size());
			for(int i = 0; i < seq.size(); ++i)
				writeTokenToFile(seq[i], 8);
		}
	
		//Insere o termo no dicionário	
		hashTable->put(make_pair(currentNode, arg), size);
		size++;
		if(size >= dictionaryMaxSize) {
			hashTable->clear();
			size = 1;
			for(int i = 0; i < 256; ++i) {
				hashTable->put(make_pair(0,i), size);
				size++;
			}
		}
	
		currentNode = hashTable->get(make_pair(0,arg));
		seq.clear();
		seq.push_back(arg);
	} else {
		currentNode = nextNode;
		seq.push_back(arg);
	}
}

void LZWC::encodeAndWrite(int node, int sizeInBits) {
	int encodedSize;	
	ull token = encodeInt(node, sizeInBits, &encodedSize);
	if(encodedSize <= 0 || encodedSize > 64) {
		printf("EncodedSize %d Node %d SizeInBits %d\n", encodedSize, node, sizeInBits);
	}
	assert(encodedSize > 0 && encodedSize <= 64);
	writeTokenToFile(token, encodedSize);
}

void LZWC::onClosing() {
	if(currentNode != 0)  {
		writeTokenToFile(1, 1);
		encodeAndWrite(currentNode, SIZE_IN_BITS(currentNode));
	}
}


