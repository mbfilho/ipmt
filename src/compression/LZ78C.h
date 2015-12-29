#ifndef LZ78C_H
#define LZ78C_H
#include "Compressor.h"
#include "TrieNode.h"

class LZ78C : public Compressor {
public:
	LZ78C(const char* fileName);

	void writeByte(int arg);

	void flushAndClose();
private:
	vector<TrieNode> trie;
	int currentNode;
};


#endif
