#ifndef COMPRESSOR_H
#define COMPRESSOR_H
/*
 * Um compressor recebe uma stream de bits e utiliza um algoritmo de compressão para 
 * gerar uma stream comprimida.
*/
class Compressor {
public:
	/*
	* Os algoritmos a seguir fornecem maneiras diferentes do usuário fazer a mesma coisa:
	*	definir quais serão os próximos bits da stream de entrada. Note que, os métodos
	* 	que recebem arrays NÃO salvam o tamanho desses arrays. Se isso for necessário
	*	o usuário dessa classe deve cuidar disso separadamente.
	*/
	virtual void writeInt(int arg) = 0;
	virtual void writeArrayOfInts(const int* arg, int size) = 0;
	virtual void writeByte(int arg) = 0;
	virtual void writeText(const char* text, int size) = 0;

	virtual void flushAndClose() = 0;
};

#endif
