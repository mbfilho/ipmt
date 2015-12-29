#ifndef LZ77C_H
#define LZ77C_H
#include "Compressor.h"
#include "SlidingWindow.h"
#include "Global.h"

class LZ77C : public Compressor {
public:
	LZ77C(const char* fileName, int bufferSize, int lookAheadSize);
	
	void writeByte(int arg);	

	void flushAndClose();

private:
	SlidingWindow window;
	int WB, WL; //os tamanhos das janelas de buffer (WB) e da janela 'lookahead' (WL)
	int bitsWB, bitsWL; //A quantidade de bits para se representar WB e WL, respectivamente.

	int* fail; //a função de falha do KMP para WL
	FILE* file;

	void emmitToken();
	void buildFailFunction();
};

#endif

