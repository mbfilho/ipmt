#ifndef SUFFIX_ARRAY
#define SUFFIX_ARRAY

#include "Index.h"
#include "Global.h"

class SuffixArray : public Index {
public:
	SuffixArray();
	~SuffixArray();
	void build(const char* text, size_t n);
	void findMatchings(const char* pat, size_t m, bool countOnly); 

	//Existem apenas para rodar os testes
	int* getArray();
	int* getLlcp();
	int* getRlcp(); 
private:
	
	void sortPieces();
	pair<int,int> getPair(int pos);
	int buildLcpArrays(int l, int r);
	void buildSuffixArray();
	//O array P^k
	int *piecesRank; //Dado um índice qual a ordem dele (no passo anterior). No fim das iterações, ele é o dual do array de sufixo.
	int k; //O tamanho dos pedaços na construção do array
	int* suffixArray;
	int* lcp;
	int* count, *tmp; //Para o algoritmo de ordenação
	int *lLcp, *rLcp;
	size_t n;
	const char* text;
};

#endif

