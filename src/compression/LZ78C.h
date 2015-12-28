#ifndef LZ78C_H
#define LZ78C_H
#include "Compressor.h"
#include "TrieNode.h"
#include "Global.h"

#define BUFFER_MAX_SIZE 1024
#define MIN(a,b) ((a)<(b))?(a):(b)
class LZ78C : public Compressor {
public:
	LZ78C(const char* fileName);

	void writeInt(int arg);
	void writeArrayOfInts(const int* arg, int size);
	void writeByte(int arg);
	void writeText(const char* text, int size);

	void flushAndClose();
private:
	vector<TrieNode> trie;
	int currentNode;
	int dicionarySize;

	ull buffer[BUFFER_MAX_SIZE+1];
	int lastTokenSize; //a quantidade de bits utilizados no último elemento de 'buffer'
	int bufferSize;

	FILE* file;
	
	int getSizeInBits(int arg);
	void encodeInt(int arg);
	void insertIntoBuffer(ull token, int tokenSize);
	void flush();
};


#endif
