#ifndef LZ77C_H
#define LZ77C_H
#include "Compressor.h"
#include "Global.h"
#define MAX_WINDOW_SIZE (1<<20)

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

	int emmitToken(uchar* curWindow, int wSize);
	void emmitTokens(bool last);

	void buildFailFunction(uchar* ahead, int aSize);

	uchar window[MAX_WINDOW_SIZE];
	int windowSize;
};

#endif

