#include "LZ77C.h"

LZ77C::LZ77C(const char* fileName, int bufferSize, int lookAheadSize) : window(bufferSize + lookAheadSize){
	assert(bufferSize + lookAheadSize <= (1<<23));
	file = fopen(fileName, "wb");
	
	if(!file) {
		printf("[LZ77 compressor] Não foi possível abrir o arquivo \'%s\' para escrita.\n", fileName);
		assert(file);
	}

	WB = bufferSize;
	WL = lookAheadSize;
	fail = new int[WL];

	/*
	* 'window' contém tanto o buffer quanto o lookahead. As 
	* primeiras 'WB' posições representam o buffer. Por isso,
	* precisamos preencher com alguma coisa.
	*/
	for(int i = 0; i < WB; ++i)
		window.append(0);

	outputBuffer = new ull[MAX_OUTPUTBUFFER_SIZE + 1];
	outSize = 0;
}

void LZ77C::writeInt(int arg) {
	ui u = ui(arg);
	for(int i = 0; i < 4; ++i) {
		writeByte(u & 0xff);	
		u >>= 8;
	}
}

void LZ77C::writeArrayOfInts(const int* arg, int size) {
	for(int i = 0; i < size; ++i)
		writeInt(arg[i]);
}

void LZ77C::writeText(const char* text, int size) {
	for(int i = 0; i < size; ++i)
		writeByte(text[i]);
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
		| (ull(bestMatchingSize) << 23)
		| (ull(ui(mismatchingChar)) << 56);
	insertIntoBuffer(token);
	
	window.slide(bestMatchingSize+1);
}

void LZ77C::flushAndClose() {
	while(window.getSize() > WB) 
		emmitToken();

	if(outSize)  {
		assert(outSize == fwrite(outputBuffer, sizeof(ull), outSize, file));
		outSize = 0;
	}

	fclose(file);
}

void LZ77C::insertIntoBuffer(ull token) {
	outputBuffer[outSize++] = token;
	if(outSize >= MAX_OUTPUTBUFFER_SIZE)  {
		assert(outSize == fwrite(outputBuffer, sizeof(ull), outSize, file));
		outSize = 0;
	}
}


