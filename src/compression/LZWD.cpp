#include "LZWD.h"


LZWD::LZWD(FILE* inputFile): input(inputFile) {
	trie = new HashTable();
	printf("Criei\n");
	revTrie.push_back(ReversedTrieNode(-1, 0));//a raiz
	for(int i = 0; i < 256; ++i) {
		insertIntoTrie(0, i);
	}
	lastNode = -1;
}

void LZWD::close() {
	input.close();
}

void LZWD::insertIntoTrie(int parent, Byte arg) {
	trie->put(make_pair(parent,arg), revTrie.size());
	revTrie.push_back(ReversedTrieNode(parent, arg));
}

int LZWD::readByte() {
	if(buffer.size() == 0)
		readToken();
	int byte = buffer.front();
	buffer.pop_front();

	return byte;
}

void LZWD::readUncompressedSequence(int size) {
	Byte value =  input.getBunchOfBits(8);
	if(lastNode != -1)
		insertIntoTrie(lastNode, value);
	buffer.push_back(value);

	int curNode = trie->get(make_pair(0,value));
	for(int i = 1; i < size; ++i) {
		value = input.getBunchOfBits(8);
		buffer.push_back(value);
		curNode = trie->get(make_pair(curNode, value));
	}
	
	//Como atualizar lastNode?
	lastNode = curNode;
}

void LZWD::readToken() {
	int uncompressedSeqSize = 0;
	while(!input.getNextBit()) {
		++uncompressedSeqSize;
	}

	if(uncompressedSeqSize != 0) {
		readUncompressedSequence(uncompressedSeqSize);
	} else {
		int trieNode = decodeInt();
		if(trieNode == revTrie.size()) {
			list<int>::iterator it = buffer.end();
			for(int i = lastNode; i != 0; i = revTrie[i].parent) 
				it = buffer.insert(it, revTrie[i].label);
			
			insertIntoTrie(lastNode, *it);

			buffer.push_back(*it);
		} else {
			list<int>::iterator it = buffer.end();
			for(int i = trieNode; i != 0; i = revTrie[i].parent) 
				it = buffer.insert(it, revTrie[i].label);

			if(lastNode != -1) 
				insertIntoTrie(lastNode, *it);

		}

		lastNode = trieNode;
	}
	
	if(revTrie.size() + 1 >= (1<<19)) {
		lastNode = -1;
		revTrie.clear();
		trie->clear();

		revTrie.push_back(ReversedTrieNode(-1, 0));//a raiz
		for(int i = 0; i < 256; ++i) 
			insertIntoTrie(0, i);
		
	}
}

int LZWD::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
