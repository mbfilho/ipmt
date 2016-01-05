#ifndef LZWC_H
#define LZWC_H
#include "Compressor.h"
#include "HashTable.h"

class LZWC : public Compressor {
public:
	LZWC(FILE* output, int compressionLevel);

protected:
	void feedRawByte(Byte arg);
	void onClosing();

private:
	int currentNode;
	int size;
	vector<Byte> seq;	
	/*
	* Usaremos uma HashTable para implementar uma trie.
	* Ou seja, indexaremos uma cadeia pelo par (no, char)
	* representando o filho de 'no' com aresta 'char'
	* na trie. Isso possibilita que o cálculo do hash
	* se dê de maneira constante e ajuda muito
	* no tratamento de colisões dentro da HashTable.
	*/
	HashTable* hashTable;
	
	int dictionaryMaxSize;
	void encodeAndWrite(int node, int sizeInBits);
};


#endif
