#ifndef LZ77C_H
#define LZ77C_H
#include "Compressor.h"
#include "Global.h"
#include "Treap.h"
#define MAX_WINDOW_SIZE (1<<20)

struct CmpSet {
	int totalSize, WL;
	uchar* window;
	
	CmpSet(uchar* win, int size, int W) {
		totalSize = size;
		WL = W;
		window = win;
	}
	
	int getLcp(const int a, const int b) {
		int s = MIN(MIN(WL,totalSize-a), totalSize-b), lcp = 0;
		while(lcp < s && window[lcp+a] == window[lcp+b]) ++lcp;
		return lcp;
	}

	bool operator()(const int a, const int b) {
		int lcp = getLcp(a, b);
		if(b+lcp >= totalSize)//se 'b' acabou então 'a' não pode ser menor.
			return false;
		if(a+lcp >= totalSize) //'a' acabou, mas 'b' não.
			return true;

		//nenhum dos dois acabou
		if(window[a+lcp] == window[b+lcp]) 
			return a < b;
		return window[a+lcp] < window[b+lcp];
	}
};
class LZ77C : public Compressor {
public:
	LZ77C(const char* fileName, int bufferSize, int lookAheadSize);
	
	void writeByte(int arg);	

	void flushAndClose();

private:
	int WB, WL; //os tamanhos das janelas de buffer (WB) e da janela 'lookahead' (WL)
	int bitsWB, bitsWL; //A quantidade de bits para se representar WB e WL, respectivamente.

	int* fail; //a função de falha do KMP para WL
	FILE* file;

	int emmitToken(int offset, Treap<CmpSet>& tree);
	void emmitTokens(bool last);

	void buildFailFunction(uchar* ahead, int aSize);

	uchar window[MAX_WINDOW_SIZE];
	int windowSize;
};

#endif

