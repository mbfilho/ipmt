#ifndef LZ77C_H
#define LZ77C_H
#include "Compressor.h"
#include "SlidingWindow.h"
#include "Global.h"

#define MAX_OUTPUTBUFFER_SIZE 1024

class LZ77C : public Compressor {
public:
	LZ77C(const char* fileName, int bufferSize, int lookAheadSize);
	
	void writeInt(int arg);
	void writeArrayOfInts(const int* arg, int size);
	void writeByte(int arg);	
	void writeText(const char* text, int size);

	void flushAndClose();

private:
	SlidingWindow window;
	int WB, WL; //os tamanhos das janelas de buffer (WB) e da janela 'lookahead' (WL)
	int* fail; //a função de falha do KMP para WL
	FILE* file;

	ull* outputBuffer;
	int outSize;
	
	void emmitToken();
	void insertIntoBuffer(ull token);
	void buildFailFunction();
};

#endif

