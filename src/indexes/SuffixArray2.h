#ifndef SUFFIX_ARRAY_2
#define SUFFIX_ARRAY_2
#include <cstdio>
#include <algorithm>
#include "Index.h"

#define getPair(pos,out) {out[0]=piecesRank[pos],out[1]=(pos+(1<<k)) >= n ? 0 : piecesRank[pos+(1<<k)];}
using std::pair;
using std::make_pair;

class SuffixArray2 : public Index {
public:
	SuffixArray2();
	~SuffixArray2();
	void build(const char* text, size_t n);
	
	//Existem apenas para rodar os testes
	int* getArray();
	int* getLlcp();
	int* getRlcp(); 
private:
	void sortPieces();
	int buildLcpArrays(int l, int r);
	void buildSuffixArray();
	//O array P^k
	int *piecesRank; //Dado um índice qual a ordem dele (no passo anterior). No fim das iterações, ele é o dual do array de sufixo.
	int k; //O tamanho dos pedaços na construção do array
	int* suffixArray;
	int* lcp;
	int  *tmp; //Para o algoritmo de ordenação
	int *lLcp, *rLcp;
	size_t n;
	const char* text;

	struct Cmp{
		int* piecesRank, k;
		size_t n;
		
		Cmp(int* pp, int kk, size_t nn): piecesRank(pp), k(kk), n(nn){}
		bool operator()(int a, int b) {
			int pa[2], pb[2];
			getPair(a, pa); getPair(b, pb);
			return pa[0] < pb[0] || (pa[0] == pb[0] && pa[1] < pb[1]);
		}
	};
};

#endif

