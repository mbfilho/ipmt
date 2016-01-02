#include "LZ77C.h"

LZ77C::LZ77C(FILE* output, int bufferSize, int lookAheadSize) : Compressor(output) {
	WB = bufferSize;
	WL = lookAheadSize;
	
	bitsWB  = (WB == 0 ? 1 : 32 - __builtin_clz(WB));
	bitsWL  = (WL == 0 ? 1 : 32 - __builtin_clz(WL));
	assert(bitsWB + bitsWL + 8 <= 64);

	fail = new int[WL];

	/*
	* 'window' contém tanto o buffer quanto o lookahead. As 
	* primeiras 'WB' posições representam o buffer. Por isso,
	* precisamos preencher com alguma coisa.
	*/
	for(int i = 0; i < WB; ++i)
		window[i] = 0;
	windowSize = WB;
}

void LZ77C::writeByte(int arg) {
	window[windowSize++] = arg;
	if(windowSize == MAX_WINDOW_SIZE-1) 
		emmitTokens(false);
}

void LZ77C::buildFailFunction(uchar* ahead, int aSize) {
	int k = fail[0] = -1;
	for(int i = 1; i < aSize; ++i){
		while(k != -1 && ahead[i] != ahead[k+1])
			k = fail[k];
		if(ahead[i] == ahead[k+1])
			++k;
		fail[i] = k;
	}
}

void LZ77C::emmitTokens(bool last) {
	Treap<CmpSet> tree(CmpSet(window, windowSize, WL)); 

	if(windowSize > WB) {
		for(int i = 0; i < WB; ++i)
			tree.insert(i);
	}

	int offset = 0;
	while(windowSize - offset >= WB + WL){
		int jump = emmitToken(offset, tree);
		for(int i = 0; i < jump; ++i) {
			tree.remove(offset + i);
			if(offset + WB + i < windowSize)
				tree.insert(offset + WB + i);
		}

		offset += jump;
	}

	assert((windowSize - offset) >= WB);

	if(last) {
		while(windowSize - offset > WB) {
			int jump = emmitToken(offset, tree);
			for(int i = 0; i < jump; ++i) {
				tree.remove(offset + i);
				if(offset + WB + i < windowSize)
					tree.insert(offset + WB + i);
			}
			offset += jump;
		}
	} else {
		for(int i = 0; i < (windowSize-offset); ++i)
			window[i] = window[i+offset];
		windowSize -= offset;
	}
}


int LZ77C::emmitToken(int offset, Treap<CmpSet>& tree) {
	int wSize = MIN(windowSize - offset, WB+WL);
	uchar* curWindow = window + offset;
	int wl = wSize - WB, k = -1;
	buildFailFunction(curWindow + WB, wl);	

	ui bestMatchingSize = 0, bestMatchingPos = 0, mismatchingChar = 0;

	/*Investigar melhor maneira de fazer isso.
	* Garante que sempre vai haver um caractere de mismatching.
	* Quando wl != WL, eh o final da stream, e o último caracter não está no alfabeto.
	*/
	if(wl == WL) 
		--wl;
	
	TreapNode* root = tree.root;
	while(root != NULL) {
		int lcp = MIN(wl, tree.comparator.getLcp(WB+offset, root->key));
		if(lcp >= bestMatchingSize) {
			bestMatchingSize = lcp;
			bestMatchingPos = root->key-offset;
			mismatchingChar = window[offset+WB+lcp];
		}
		if(lcp == wl) break; //não dá para encontrar melhor

		if(root->key + lcp >= windowSize || window[root->key+lcp] < window[WB+lcp+offset]) {
			root = root -> right;
		} else
			root = root -> left;
	}
	if(!bestMatchingSize)
		bestMatchingPos = 0;

	ull token = 
		bestMatchingPos
		| (ull(bestMatchingSize) << (bitsWB))
		| (ull(ui(mismatchingChar)) << (bitsWB+bitsWL));
	insertIntoBuffer(token, 8 + bitsWB + bitsWL);
	
	return bestMatchingSize+1;
}

void LZ77C::flushAndClose() {
	emmitTokens(true);

	flush(true);
	close();
}

