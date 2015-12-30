#include "LZ77C.h"

LZ77C::LZ77C(const char* fileName, int bufferSize, int lookAheadSize) : Compressor(fileName) {
	WB = bufferSize;
	WL = lookAheadSize;
	
	int tmp[] = {WB, WL};
	fwrite(tmp, sizeof(int), 2, outputFile);

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
	int size = windowSize;
	uchar* curWindow = window;
	while(size >= WB + WL){
		int jump = emmitToken(curWindow, WB+WL);
		size -= jump;
		curWindow += jump;
	}

	assert(size >= WB);

	if(last) {
		while(size > WB) {
			int jump = emmitToken(curWindow, size);
			size -= jump;
			curWindow += jump;
		}
	} else {
		for(int i = 0; i < size; ++i)
			window[i] = curWindow[i];
		windowSize = size;
	}
}

int LZ77C::emmitToken(uchar* curWindow, int wSize) {
	int wl = wSize - WB, k = -1;
	buildFailFunction(curWindow + WB, wl);	

	ui bestMatchingSize = 0, bestMatchingPos, mismatchingChar = 0;

	/*Investigar melhor maneira de fazer isso.
	* Garante que sempre vai haver um caractere de mismatching.
	* Quando wl != WL, eh o final da stream, e o último caracter não está no alfabeto.
	*/
	if(wl == WL) 
		--wl;

	for(int i = 0; i < wSize; ++i){
		while(k != -1 && curWindow[WB + k+1] != curWindow[i])
			k = fail[k];
		if(curWindow[WB+k+1] == curWindow[i])
			++k;

		int matchingPos = i - k;
		//O matching começou depois da janela de buffer
		if(matchingPos >= WB)
			break;
		if((k+1) >= bestMatchingSize) {
			bestMatchingSize = k + 1;
			bestMatchingPos = i - k;
			mismatchingChar = curWindow[WB+bestMatchingSize];
		}
		
		//não dá pra fazer melhor que isso
		if(k == wl - 1) 
			break;
	}

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

