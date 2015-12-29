#include "LZWD.h"


LZWD::LZWD(const char* fileName): input(fileName) {
	trie.push_back(ReversedTrieNode(-1, 0));//a raiz
	for(int i = 0; i < 256; ++i) {
		trie.push_back(ReversedTrieNode(0, i));
	}
	lastNode = -1;
}

void LZWD::close() {
	input.close();
}

int LZWD::readInt() {
	int ret = 0;
	for(int i = 0; i < 4; ++i) 
		ret |= (readByte() + 128) << (i * 8);
	return ret;	
}

int LZWD::readByte() {
	if(buffer.size() == 0)
		readToken();
	int byte = buffer.front() - 128;
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
}

int LZWD::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
