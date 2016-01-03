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
	Compressor(FILE* output);
	/*
	* Os algoritmos a seguir fornecem maneiras diferentes do usuário fazer a mesma coisa:
	*	definir quais serão os próximos bits da stream de entrada. 
	*/
	virtual void writeInt(int arg, int sizeInBits);
	
	virtual void flushAndClose() = 0;

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

	virtual void writeByte(int arg) = 0;

	virtual void flushOutput();
	virtual void flushInput();

	void insertIntoBuffer(ull token, int tokenSize);
	void close();

private:
	FILE* outputFile;
	
	int lastTokenSize; //a quantidade de bits utilizados em token 
	ull lastToken; //lastToken contem os bits da última tupla codificada pelo algoritmo

	Byte unfinishedByte;
	int unfinishedByteSize;
};

#endif
