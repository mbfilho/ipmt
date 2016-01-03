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
		int nodeSIze = SIZE_IN_BITS(currentNode);
		int curNodeRpr = 1 + nodeSIze + 2*SIZE_IN_BITS(nodeSIze);

		if(1+curNodeRpr <= 9*seq.size()){
			insertIntoBuffer(1, 1);
			encodeAndWriteInt(currentNode); //O indice do termo no dicionário
		}else {
			insertIntoBuffer(0, seq.size());
			int i = 0;
			while(seq.size() - i >= 8) {
				ull token =  0;
				for(int j = 7; j >=0; --j)
					token = (token << 8) | seq[i+j];
				i += 8;
				insertIntoBuffer(token, 64);
			}
			
			ull token = 0;
			int tSize = 0;
			for(int j = seq.size() - 1; j >= i; --j) {
				token = (token << 8) | seq[j];
				tSize += 8;
			}
			if(tSize)
				insertIntoBuffer(token, tSize);
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

void LZWC::flushAndClose() {
	flushInput();

	if(currentNode != 0) {
		encodeAndWriteInt(currentNode);
	}

	flushOutput();
	close();
}


