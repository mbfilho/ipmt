#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#define BUFFER_MAX_SIZE 1024

#include "Global.h"

/*
 * Um compressor recebe uma stream de bits e utiliza um algoritmo de compressão para 
 * gerar uma stream comprimida.
*/
class Compressor {
public:
	Compressor(const char* fileName);
	/*
	* Os algoritmos a seguir fornecem maneiras diferentes do usuário fazer a mesma coisa:
	*	definir quais serão os próximos bits da stream de entrada. Note que, os métodos
	* 	que recebem arrays NÃO salvam o tamanho desses arrays. Se isso for necessário
	*	o usuário dessa classe deve cuidar disso separadamente.
	*/
	virtual void writeInt(int arg);
	virtual void writeArrayOfInts(const int* arg, int size);
	virtual void writeText(const char* text, int size);
	
	
	virtual void writeByte(int arg) = 0;
	virtual void flushAndClose() = 0;

	FILE* outputFile;

	ull* outputBuffer;
	int lastTokenSize; //a quantidade de bits utilizados no último elemento de 'buffer'
	int outputBufferSize;

protected:
	/*
	* Método utilizado pelos algoritmos LZW e LZ78
	* Estratégia:
	* 	Seja a2 a representação binária de arg. A codificação de arg será:
	* 		|a2|_1 + 0 + |a2|_2 + a2
	* Tamanho dessa codificação (em bits):
	* 	|a2| + log(|a2|) + 1 + log(|a2|) = O(|a2| + log(|a2|))
	* ou, de maneira equivalente,
	*	O(log(a2) + log log(a2))
	*/
	void encodeAndWriteInt(int arg);

	virtual void flush(bool force);
	void insertIntoBuffer(ull token, int tokenSize);
	void close();
};

#endif
