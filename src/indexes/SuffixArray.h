#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include "Index.h"
#include "Global.h"
#include "Printer.h"
typedef pair<int,int> pii;

struct Piece {
	int i, p[2];

	Piece& operator=(const Piece& ot) { 
		i = ot.i;
		p[0] = ot.p[0];
		p[1] = ot.p[1];
	}
};

class SuffixArray : public Index {
public:
	SuffixArray();
	~SuffixArray();
	void build(const char* text, size_t n);
	void findMatchings(const char* pat, size_t m, bool countOnly); 

	void serialize(Compressor* compressor); 
	void deserialize(Decompressor* decompressor); 

	//Existem apenas para rodar os testes
	int* getArray();
	int* getLlcp();
	int* getRlcp(); 
private:
	
	int getLcp(const char* a, const char* b);
	int findSuccessor(const char* pat, size_t m);
	int findPredecessor(const char* pat, size_t m);
	
	void sortPieces();
	pair<int,int> getPair(int pos);
	int buildLcpArrays(int l, int r);
	void buildSuffixArray();
	
	Piece* pieces; //O array de triplas que será repetidamente ordenado
	Piece *auxPieces; //array auxiliar para a ordenação

	int *saDual; //Dado um índice qual a ordem dele no array de sufixo
	int k; //O tamanho dos pedaços na construção do array
	int* suffixArray;
	int* lcp;
	int* count, *tmp; //Para o algoritmo de ordenação
	int *lLcp, *rLcp;
	size_t n;
	const char* text;
};

#endif

