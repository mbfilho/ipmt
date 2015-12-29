#ifndef LZWC_H
#define LZWC_H
#include "Compressor.h"
#include "TrieNode.h"

class LZWC : public Compressor {
public:
	LZWC(const char* fileName);

	void writeByte(int arg);
	void writeText(const char* text, int size);

	void flushAndClose();
private:
	vector<TrieNode> trie;
	int currentNode;
};


#endif
