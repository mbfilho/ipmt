#include "SuffixArray2.h"

SuffixArray2::SuffixArray2(){
	piecesRank = NULL;
	suffixArray = NULL;
	lcp = NULL;
	tmp = NULL;
	lLcp = NULL;
	rLcp = NULL;
}

SuffixArray2::~SuffixArray2(){
	if(piecesRank) delete [] piecesRank;
	if(suffixArray) delete [] suffixArray;	
	if(lcp) delete [] lcp;
	if(tmp) delete [] tmp;
	if(lLcp) delete [] lLcp;
	if(rLcp) delete [] rLcp;
}

void SuffixArray2::build(const char* text, size_t size) {
	this->n = size;
	this->text = text;	
	piecesRank = new int[MAX(256, size)+1];//importante que seja deste tamanho
	suffixArray = new int[MAX(256, size)+1];
	
	tmp = new int[MAX(256, size)+1];
	
	buildSuffixArray();
	lcp = new int[size];

	//Lembrando que os valores em piecesRank são 1-based	
	int lastLcp = 0;
	for(int i = 0; i < n; ++i){
		//printf("I %d Rank %d\n", i, piecesRank[i]-1);
		if(piecesRank[i] > 1){
			int j = suffixArray[piecesRank[i]-2];//predecessor do sufixo i
			while(i+lastLcp < n && j + lastLcp < n && text[i+lastLcp] == text[j+lastLcp])
				++lastLcp;
			lcp[piecesRank[i]-1] = lastLcp;
			if(lastLcp>0)
				--lastLcp;
		}
	}
//	printf("SuffixArray:\n");
//	for(int i = 0; i < n; ++i)
//		printf("%2d - |%s|\n", suffixArray[i], text + suffixArray[i]);

//	printf("Lcp (conse.): oo");
//	for(int i = 1; i < n; ++i)
//		printf(" %d", lcp[i]);
//	printf("\n");

	lLcp = piecesRank;
	piecesRank = NULL;
	rLcp = tmp;
	tmp = NULL;
	//Só coloquei isso para testar. Pode ser removido quando tiver certeza que a implementação está correta.
	for(int i = 0; i < n; ++i)
		lLcp[i] = rLcp[i] = -1;
	buildLcpArrays(0, n-1);
}

int* SuffixArray2::getArray() {
	return suffixArray;
}

int* SuffixArray2::getLlcp() {
	return lLcp;
}

int* SuffixArray2::getRlcp() {
	return rLcp;
}

//Preenche os valores lLcp[h] e rRcp[h] para h = (l+r)/2.
//Retorna o menor valor do array lcp no intervalo (l, r]
int SuffixArray2::buildLcpArrays(int l, int r) {
	
	//não atualiza (l|r)Lcp. A busca binária não entra em intervalos deste tamanho
	if(r-l == 1) {
		return lcp[r];
	}

	int h = (l+r)>>1;

	lLcp[h] = buildLcpArrays(l, h);
   	rLcp[h] = buildLcpArrays(h, r);
	
	return MIN(lLcp[h], rLcp[h]);
}

void SuffixArray2::buildSuffixArray() {
	//primeira 'ordenação'
	for(int i = 0; i < n; ++i)
		piecesRank[i] = text[i];

	k = 0;
	while((1<<k) <= n){
		for(int i = 0; i < n; ++i)
			suffixArray[i] = i;
		
		sortPieces();
		int rank = 1;
		int i = 0;
		while(i < n){
			int j = i;
			int pi[2], pj[2];
			getPair(suffixArray[i], pi);

			while(j < n){
				getPair(suffixArray[j], pj);
				
				if(pi[0] == pj[0] && pi[1] == pj[1]){
				   	tmp[suffixArray[j]] = rank;
					++j;
				} else {
					++rank;
					break;
				}
			}
			i = j;	
		}
		int* aux = piecesRank;
		piecesRank = tmp;
		tmp = aux;
		++k;
	}
}


void SuffixArray2::sortPieces() {
	Cmp c(piecesRank, k, n);
	make_heap(suffixArray, suffixArray + n, c);
	sort_heap(suffixArray, suffixArray + n, c);
}

