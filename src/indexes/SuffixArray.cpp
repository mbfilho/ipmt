#include "SuffixArray.h"

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


void SuffixArray::serialize(Compressor* compressor) {
	//Escreve o tamanho do texto e o texto
	compressor->feedRawBits(n, 32);
	int tot = 4 * n, saved = 0, percentageToPrint = 0;
	for(int i = 0; i < n; ++i) {
		compressor->feedRawBits(text[i], 8);
		if(percentageToPrint  <= saved) {
			printf("\r%.2lf%% concluído", double(saved)/tot * 100);
			fflush(stdout);
			percentageToPrint += tot/10;
		}
	}
	saved = n;
		
	int sizeOfIntegers = SIZE_IN_BITS(int(n-1)); 
	int *arrays[] = {suffixArray, lLcp, rLcp};
	for(int j = 0; j < 3; ++j) {
		for(int i = 0; i < n; ++i) {
			compressor->feedRawBits(arrays[j][i], sizeOfIntegers);
			++saved;
			if(percentageToPrint  <= saved) {
				printf("\r%.2lf%% concluído", double(saved)/tot * 100);
				fflush(stdout);
				percentageToPrint += tot/10;
			}
		}
	}
	printf("\n");

}

void SuffixArray::deserialize(Decompressor* decompressor) {
	n = decompressor->readInt(32);
	char* tmp = new char[n+1];
		
	int tot = n, loaded = 0, percentageToPrint = 0;
	for(int i = 0; i < n; ++i) {
		tmp[i] = decompressor->readInt(8);
		if(percentageToPrint <= i) {
			printf("\rDescomprimindo string ... %.2lf%% concluído", double(i)/tot * 100);
			fflush(stdout);
			percentageToPrint += tot/10;
		}
	}
	tmp[n] = 0;
	text = tmp;

	printf("\n");
	tot = 3 * n, loaded = 0, percentageToPrint = 0;
	suffixArray = new int[n];
	lLcp = new int[n];
	rLcp = new int[n];
	int sizeOfIntegers = SIZE_IN_BITS(int(n-1)); 
	
	int* arrays[] = {suffixArray, lLcp, rLcp};
	for(int j = 0; j < 3; ++j) {
		for(int i = 0; i < n; ++i) {
			arrays[j][i] = decompressor->readInt(sizeOfIntegers);
			++loaded;
			if(percentageToPrint <= loaded) {
				printf("\rDescomprimindo restante do índice ... %.2lf%% concluído", double(loaded)/tot * 100);
				fflush(stdout);
				percentageToPrint += tot/10;
			}
		}
	}
	printf("\n\n\n");
	
}


void SuffixArray::build(const char* text, size_t size) {
	this->n = size;
	this->text = text;	

	piecesRank = new int[size];
	suffixArray = new int[size];

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
	
	buildLcpArrays(0, n-1);
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
	int tot = 0;
	while((1<<tot)<=n) ++tot;
	k = 0;
	for(int i = 0; i < n; ++i)
		suffixArray[i] = i;
	while((1<<k) <= n){
		printf("\rFase %d/%d", k,tot), fflush(stdout);
	
		sortPieces();
		
		int rank = 1;
		tmp[suffixArray[0]] = 1;
		
		int pi[2], pj[2], *plast = pi, *pcur = pj;
		getPair(suffixArray[0], plast);
		for(int i = 1; i < n; ++i) {
			getPair(suffixArray[i], pcur);
			tmp[suffixArray[i]] = pcur[1] == plast[1] && pcur[0] == plast[0] ? tmp[suffixArray[i-1]] : ++rank;
			std::swap(plast, pcur);
		}

		std::swap(piecesRank, tmp);

		if(rank == n) {
			printf("\rArray totalmente construído na etapa %d/%d", k, tot);
			break;
		}
		++k;
	}
	printf("\n");
}

void SuffixArray::sortPieces() {
	int pi[2];
	for(int d = 1; d >= 0; --d){
		for(int i =0 ; i <= n; ++i)
			count[i]=0; 

		for(int i = 0; i < n; ++i){
			getPair(suffixArray[i], pi);
			++count[pi[d]];
		}

		for(int i = 1; i <= n; ++i)
			count[i] += count[i-1];

		for(int i = n - 1; i >= 0; --i){
			getPair(suffixArray[i], pi);
			tmp[--count[pi[d]]] = suffixArray[i];
		}
		std::swap(tmp, suffixArray);
	}
}

void SuffixArray::findMatchings(const char* pat, size_t m, bool countOnly) { 
	int suc = findSuccessor(pat, m), pred = findPredecessor(pat, m);
	printf("%d ocorrências encontradas\n", suc <= pred ? pred - suc + 1 : 0);

	if(!countOnly) {
		Printer printer(text, n, pat, m);
		for(int i = suc; i <= pred; ++i)
			printer.addMatching(suffixArray[i]);
		printer.print();
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







