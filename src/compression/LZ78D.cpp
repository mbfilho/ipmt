#include "LZ78D.h"


LZ78D::LZ78D(const char* fileName): input(fileName) {
	trie.push_back(ReversedTrieNode(-1, 0));//a raiz
}

void LZ78D::close() {
	input.close();
}

int LZ78D::readInt() {
	int ret = 0;
	for(int i = 0; i < 4; ++i) 
		ret |= readByte() << (i * 8);
	return ret;	
}

int LZ78D::readByte() {
	if(buffer.size() == 0)
		readToken();
	int byte = buffer.front();
	buffer.pop_front();

	return byte;
}

void LZ78D::readToken() {
	int trieNode = decodeInt(), mismatchingByte = decodeInt();
	list<int>::iterator it = buffer.end();
	it = buffer.insert(it, mismatchingByte);
	for(int i = trieNode; i != 0; i = trie[i].parent) 
		it = buffer.insert(it, trie[i].label);

	trie.push_back(ReversedTrieNode(trieNode, mismatchingByte));
}

int LZ78D::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
