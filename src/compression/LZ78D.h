#ifndef LZ78D_H
#define LZ78D_H
#include "InputBuffer.h"
#include "Decompressor.h"

struct ReversedTrieNode {
	int parent, label;
	ReversedTrieNode(){}
	ReversedTrieNode(int p, int l): parent(p), label(l){}
};

class LZ78D : public Decompressor {
public:
	LZ78D(const char* fileName);
	
	int readInt();
	int readByte();
	void close();

private:
	InputBuffer input;	

	list<int> buffer;

	vector<ReversedTrieNode> trie;

	void readToken();
	int decodeInt();
};
#endif
