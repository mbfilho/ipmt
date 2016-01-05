#ifndef LZ78C_H
#define LZ78C_H
#include "Compressor.h"
#include "HashTable.h"

class LZ78C : public Compressor {
public:
	LZ78C(FILE* output, int compressionLevel);

protected:
	void feedRawByte(Byte arg);
	void onClosing();

private:
	HashTable* hashTable;
	int dictionarySize;
	int currentNode;
	int dictionaryMaxSize;
		
	vector<Byte> sequence;

	void encodeAndWrite(int arg);
};


#endif
