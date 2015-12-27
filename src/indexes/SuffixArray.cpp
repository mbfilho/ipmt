#include "SuffixArray.h"
#define MAX(a,b) (((a) < (b))?(b):(a))
#define MIN(a,b) (((a) < (b))?(a):(b))
#define getPair(pos,out) {out[0]=piecesRank[pos],out[1]=(pos+(1<<k)) >= n ? 0 : piecesRank[pos+(1<<k)];}
SuffixArray::SuffixArray(){
	piecesRank = NULL;
	suffixArray = NULL;
	lcp = NULL;
	count = NULL;
	tmp = NULL;
	lLcp = NULL;
	rLcp = NULL;
}

SuffixArray::~SuffixArray(){
	if(piecesRank) delete [] piecesRank;
	if(suffixArray) delete [] suffixArray;	
	if(lcp) delete [] lcp;
	if(count) delete [] count;
	if(tmp) delete [] tmp;
	if(lLcp) delete [] lLcp;
	if(rLcp) delete [] rLcp;
}
struct O{
	const char* text;
	O(const char* text):text(text){}

	bool operator()(const int a, const int b){
		const char* A = text + a, *B = text + b;
		while(*A && *B && *A == *B) ++A, ++B;
		if(*A == 0) return 1;
		if(*B == 0) return 0;
//		printf("NO Sort %d %d %d\n", a, b, *A < *B);
		return *A < *B;
	}
};

void SuffixArray::build(const char* text, size_t size) {
	this->n = size;
	this->text = text;	

	sa = new int[size];
	for(int i = 0; i < n; ++i)sa[i] = i, printf("SA[%d] = %d\n", i, sa[i]) ;
	std::sort(sa, sa + n, O(text));

	piecesRank = new int[MAX(256, size)+1];
	suffixArray = new int[MAX(256, size)+1];
	
	count = new int[MAX(256, size)+1];
	tmp = new int[MAX(256, size)+1];
	
	buildSuffixArray();
	lcp = count; //para evitar fazer nova alocação
	count = NULL; //Não vamos mais utilizar count

	//Lembrando que os valores em piecesRank são 1-based	
	int lastLcp = 0;
	for(int i = 0; i < n; ++i){
		if(piecesRank[i] > 1){
			int j = suffixArray[piecesRank[i]-2];//predecessor do sufixo i
			while(i+lastLcp < n && j + lastLcp < n && text[i+lastLcp] == text[j+lastLcp])
				++lastLcp;
			lcp[piecesRank[i]-1] = lastLcp;
			if(lastLcp>0)
				--lastLcp;
		}
	}
	
	lLcp = piecesRank;
	piecesRank = NULL;
	rLcp = tmp;
	tmp = NULL;
	//Só coloquei isso para testar. Pode ser removido quando tiver certeza que a implementação está correta.
	for(int i = 0; i < n; ++i)
		lLcp[i] = rLcp[i] = -1;
	buildLcpArrays(0, n-1);

	for(int i = 0; i < n; ++i)
		printf("S[%d] = %d\n", i, int(text[i]));	
	for(int i = 0; i < n; ++i){
		printf("[%d] %d x  %d\n", i, sa[i], suffixArray[i]);
		assert(sa[i] == suffixArray[i]);
	}
}

int* SuffixArray::getArray() {
	return suffixArray;
}

int* SuffixArray::getLlcp() {
	return lLcp;
}

int* SuffixArray::getRlcp() {
	return rLcp;
}

//Preenche os valores lLcp[h] e rRcp[h] para h = (l+r)/2.
//Retorna o menor valor do array lcp no intervalo (l, r]
int SuffixArray::buildLcpArrays(int l, int r) {
	
	//não atualiza (l|r)Lcp. A busca binária não entra em intervalos deste tamanho
	if(r-l == 1) {
		return lcp[r];
	}

	int h = (l+r)>>1;

	lLcp[h] = buildLcpArrays(l, h);
   	rLcp[h] = buildLcpArrays(h, r);
	
	return MIN(lLcp[h], rLcp[h]);
}

void SuffixArray::buildSuffixArray() {
	//primeira 'ordenação'
	for(int i = 0; i < 256; ++i) count[i] = 0;
	for(int i = 0; i < n; ++i) ++count[128+text[i]];
	for(int i = 1; i < 256; ++i) count[i] = count[i-1] + (count[i] != 0);
	for(int i = 0; i < n; ++i){
		piecesRank[i] = count[128+text[i]];
	}

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

struct P{
	int* piecesRank;
	int k, n;
	P(int* p, int k, int n):piecesRank(p), k(k), n(n){}
	bool operator()(const int a, const int b){
		int pi[2], pj[2];
		getPair(a, pi);
		getPair(b, pj);
		return pi[0] < pj[0] || (pi[0] == pj[0] && pi[1] < pj[1]);
	}
};

void SuffixArray::sortPieces() {
	int pi[2];
	for(int d = 1; d >= 0; --d){
		for(int i =0 ; i <= MAX(256, n); ++i)
			count[i]=0; 

		for(int i = 0; i < n; ++i){
			getPair(suffixArray[i], pi);
			++count[pi[d]];
		}
		for(int i = 1; i <= MAX(256, n); ++i)
			count[i] += count[i-1];
		
		for(int i = n - 1; i >= 0; --i){
			getPair(suffixArray[i], pi);
			tmp[--count[pi[d]]] = suffixArray[i];
		}
		int* aux = tmp;
		tmp = suffixArray;
		suffixArray = aux;
	}
}

void SuffixArray::findMatchings(const char* pat, size_t m, bool countOnly) { 
	int suc = findSuccessor(pat, m), pred = findPredecessor(pat, m);
	printf("%d ocorrências encontradas\n", suc <= pred ? pred - suc + 1 : 0);
	printf("(%d,%d)\n", suc, pred);
	for(int i = n-1; i >= 0; --i){
		int lcp = getLcp(pat, text + suffixArray[i]);
		
		if(lcp < m && (suffixArray[i]+lcp >= n || pat[lcp] > text[suffixArray[i]+lcp])) {//se o pat > 
			printf(">> %d |%.30s|\n", i, text+suffixArray[i+1]);
			assert(i+1 == suc);
			break;
		}
	}
	for(int i = 0; i < n; ++i){
		int lcp = getLcp(pat, text + suffixArray[i]);

		if((lcp == m && suffixArray[i] + lcp >= n )|| (lcp < m && suffixArray[i] + lcp < n && pat[lcp] < text[suffixArray[i] + lcp]))//se pat <
		{
			printf("Predecessor >> %d |%.30s|\n", i-1, text+suffixArray[i-1]);
			assert(i-1 == pred);
			break;
		}
	}
}

/*
* Retorna o lcp de duas cadeias terminadas com \0
*/
int SuffixArray::getLcp(const char* a, const char* b){
	int lcp = 0;
	while(*a && *b && *a == *b) ++lcp, ++a, ++b;
	return lcp;
}

/*
* Retorna a primeira posição do array de sufixo que possui um sufixo lexicograficamente >=_m 'pat'
*/
int SuffixArray::findSuccessor(const char* pat, size_t m) {
	int L = getLcp(text + suffixArray[0], pat), R = getLcp(pat, text + suffixArray[n-1]);

	if(L == m || (suffixArray[0] + L < n && pat[L] <= text[suffixArray[0] + L])) //verifica se o sufixo em sa[0] >=_m pat
		return 0;
	if(R < m && (suffixArray[n-1] + R >= n || pat[R] > text[suffixArray[n-1] + R])) //o caso em que pat não tem sucessor: pat é maior que todos os sufixos
		return n;
	
	int l = 0, r = n - 1;
	//(l,r]

	while(r-l > 1) {
		int h = (l+r)>>1, H;

		if(L >= R) {
			if(lLcp[h] < L) H = lLcp[h];
			else H = L + getLcp(pat + L, text + suffixArray[h] + L);	
		} else {
			if(rLcp[h] < R) H = rLcp[h];
			else H = R + getLcp(pat + R, text + suffixArray[h] + R);
		}

		if(H < m && (suffixArray[h]+H >= n || pat[H] > text[suffixArray[h]+H]))//Testa se T[h..] < pat. Nesse caso h tem que cair fora da busca
			l = h, L = H;
		else
			r = h, R = H;
	}

	return r;
}

/*
* Retorna a última posição do array de sufixo que possui um sufixo lexicograficamente <=_m 'pat'
*/
int SuffixArray::findPredecessor(const char* pat, size_t m) {
	int L = getLcp(pat, text + suffixArray[0]), R = getLcp(pat, text + suffixArray[n-1]);

	if(L < m && (suffixArray[0]+L < n) && pat[L] < text[suffixArray[0]+L])//Trata o caso em que o predecessor não existe: todos os sufixos são maiores que 'pat'
		return -1;
	if(R == m || suffixArray[n-1] + R >= n ||pat[R] >= text[suffixArray[n-1] + R])//Verifica se o último sufixo <=_m pat e, portanto, é a resposta
		return n-1;

	int l = 0, r = n-1;
	//[l,r)	

	while(r-l>1) {
		int h = (r+l)>>1, H;	

		if(L >= R) {
			if(lLcp[h] < L) H = lLcp[h];
			else H = L + getLcp(pat + L, text + suffixArray[h] + L);	
		} else {
			if(rLcp[h] < R) H = rLcp[h];
			else H = R + getLcp(pat + R, text + suffixArray[h] + R);
		}

		if(H == m || suffixArray[h]+H >= n || text[suffixArray[h]+H] <= pat[H]) //o sufixo em sa[h] eh <=_m pat. Ele pode ser a resposta.
			l = h, L = H;
		else
			r = h, R = H;	
	}

	return l;
}







