#ifndef LZWD_H
#define LZWD_H
#include "InputBuffer.h"
#include "Decompressor.h"

class LZWD : public Decompressor {
public:
	LZWD(FILE* inputFile);
	
	int readInt();
	int readByte();
	void close();

private:
	struct ReversedTrieNode {
		int parent, label;
		ReversedTrieNode(){}
		ReversedTrieNode(int p, int l): parent(p), label(l){}
	};

	InputBuffer input;	

	list<int> buffer;

	vector<ReversedTrieNode> trie;
	int lastNode; //O último nó decodificado. Ele representa um casamento e precisa ser estendido com a letra que ocasionou o mismatch. Mas essa letra só é conhecida na leitura seguinte

	void readToken();
	int decodeInt();
};
#endif
