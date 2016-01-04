#ifndef LZWD_H
#define LZWD_H
#include "InputBuffer.h"
#include "Decompressor.h"

class LZWD : public Decompressor {
public:
	LZWD(FILE* inputFile);
	
	void close();

private:
	InputBuffer input;	
	vector<Byte> buffer;
	vector<pair<int,int> > table;

	int nextAvailableByte;

	int lastPos; //O último nó decodificado. Ele representa um casamento e precisa ser estendido com a letra que ocasionou o mismatch. Mas essa letra só é conhecida na leitura seguinte
	
	void readUncompressedSequence(int size);

	void readToken();
	int decodeInt();
	int readByte();
};
#endif
