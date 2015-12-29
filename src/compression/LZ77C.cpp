#include "LZ77C.h"

LZ77C::LZ77C(const char* fileName, int bufferSize, int lookAheadSize) : window(bufferSize + lookAheadSize), Compressor(fileName) {
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
		window.append(0);

}

void LZ77C::writeByte(int arg) {
	window.append(arg);
	if(window.isFull())
		emmitToken();
}

void LZ77C::buildFailFunction() {
	int k = fail[0] = -1;
	int wl = window.getSize() - WB;
	for(int i = 1; i < wl; ++i){
		int cur = window.get(i+WB);
		while(k != -1 && cur != window.get(WB + k + 1))
			k = fail[k];
		if(cur == window.get(WB + k + 1))
			++k;
		fail[i] = k;
	}
}

void LZ77C::emmitToken() {
	assert(window.getSize() > WB);

	buildFailFunction();	
	int k = -1;
	ui bestMatchingSize = 0, bestMatchingPos;
	int wl = window.getSize() - WB ;

	/*Investigar melhor maneira de fazer isso.
	* Garante que sempre vai haver um caractere de mismatching.
	* Quando wl != WL, eh o final da stream, e o último caracter não está no alfabeto.
	*/
	if(wl == WL) 
		--wl;

	for(int i = 0; i < window.getSize(); ++i){
		int cur = window.get(i);
		while(k != -1 && window.get(WB + k+1) != cur)
			k = fail[k];
		if(window.get(WB + k+1) == cur)
			++k;

		int matchingPos = i - k;
		//O matching começou depois da janela de buffer
		if(matchingPos >= WB)
			break;
		if((k+1) >= bestMatchingSize) {
			bestMatchingSize = k + 1;
			bestMatchingPos = i - k;
		}
		
		//não dá pra fazer melhor que isso
		if(k == wl - 1) 
			break;
	}
	int mismatchingChar = 0;
	if(WB + bestMatchingSize < window.getSize())
		mismatchingChar = window.get(WB + bestMatchingSize);

	ull token = 
		bestMatchingPos
		| (ull(bestMatchingSize) << (bitsWB))
		| (ull(ui(mismatchingChar)) << (bitsWB+bitsWL));
	insertIntoBuffer(token, 8 + bitsWB + bitsWL);
	printf("(%u, %u, %u) Size %d\n", bestMatchingPos, bestMatchingSize, mismatchingChar, 8+bitsWB+bitsWL);
	window.slide(bestMatchingSize+1);
}

void LZ77C::flushAndClose() {
	while(window.getSize() > WB) 
		emmitToken();
	
	flush(true);
	close();
}

