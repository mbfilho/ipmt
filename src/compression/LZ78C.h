#ifndef LZ78C_H
#define LZ78C_H
#include "Compressor.h"
#include "HashTable.h"

class LZ78C : public Compressor {
public:
	LZ78C(FILE* output);

	void writeByte(int arg);

	void flushAndClose();
private:
	HashTable* hashTable;
	int dictionarySize;
	int currentNode;
};


#endif
