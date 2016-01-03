#include "LZWD.h"


LZWD::LZWD(FILE* inputFile): input(inputFile) {
	trie.push_back(ReversedTrieNode(-1, 0));//a raiz
	for(int i = 0; i < 256; ++i) {
		trie.push_back(ReversedTrieNode(0, i));
	}
	lastNode = -1;
}

void LZWD::close() {
	input.close();
}

int LZWD::readByte() {
	if(buffer.size() == 0)
		readToken();
	int byte = buffer.front();
	buffer.pop_front();

	return byte;
}

void LZWD::readToken() {
	int trieNode = decodeInt();
	if(trieNode == trie.size()) {
		list<int>::iterator it = buffer.end();
		for(int i = lastNode; i != 0; i = trie[i].parent) 
			it = buffer.insert(it, trie[i].label);
		trie.push_back(ReversedTrieNode(lastNode, *it));
		buffer.push_back(*it);
	} else {
		list<int>::iterator it = buffer.end();
		for(int i = trieNode; i != 0; i = trie[i].parent) 
			it = buffer.insert(it, trie[i].label);

		if(lastNode != -1)
			trie.push_back(ReversedTrieNode(lastNode, *it));
	}

	lastNode = trieNode;

	if(trie.size() + 1 >= (1<<19)) {
		lastNode = -1;
		trie.clear();

		trie.push_back(ReversedTrieNode(-1, 0));//a raiz
		for(int i = 0; i < 256; ++i) {
			trie.push_back(ReversedTrieNode(0, i));
		}
	}
}

int LZWD::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
