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
		int nodeSize = SIZE_IN_BITS(currentNode);
		int curNodeRpr = 1 + nodeSize + 2*SIZE_IN_BITS(nodeSize);

		if(curNodeRpr <= 9*seq.size()){
			writeTokenToFile(1, 1);
			encodeAndWrite(currentNode, nodeSize); //O indice do termo no dicionário
		}else {
			writeTokenToFile(1ULL << (seq.size()), 1+seq.size());
			if(seq.size() > 63) {
				throw 11;
			}
			int i = 0;
			while(seq.size() - i >= 8) {
				ull token =  0;
				for(int j = 7; j >=0; --j)
					token = (token << 8) | seq[i+j];
				i += 8;
				writeTokenToFile(token, 64);
			}
			
			ull token = 0;
			int tSize = 0;
			for(int j = seq.size() - 1; j >= i; --j) {
				token = (token << 8) | seq[j];
				tSize += 8;
			}
			if(tSize)
				writeTokenToFile(token, tSize);
		}
	
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


