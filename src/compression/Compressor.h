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
	* Esse método recebe uma sequência de bits (não comprimida)
	* e repassa bytes à estratégia de compressão. Note que a
	* quantidade de bits recebida pode não ser múltipla de 8.
	* Por isso existe um 'buffer' de 1 byte para evitar passar
	* bytes 'quebrados' para a compressão.
	*/	
	void feedRawBits(ull bits, int howMany);
	
	void flushAndClose();

protected:
	/*
	* Alimenta a estratégia de compressão com 1 byte. Ele 
	* pode estar 'incompleto' caso seja o final da stream
	* de bits que será comprimida. Esse é o único caso.
	*/
	virtual void feedRawByte(Byte arg) = 0;

	/*
	* Última chance da estratégia de compressão escrever na
	* stream de saída (arquivo).
	*/
	virtual void onClosing() = 0;
	
	/*
	* Um 'token' é um bloco de informações gerado pela estratégia
	* de compressão. Por exemplo, um inteiro codificado pelo LZ78. 
	*/
	void writeTokenToFile(ull token, int tokenSize);
private:
	FILE* outputFile;
	
	int lastTokenSize; //a quantidade de bits utilizados em token 
	ull lastToken; //lastToken contem os bits da última tupla codificada pelo algoritmo

	Byte unfinishedByte;
	int unfinishedByteSize;
};

#endif
