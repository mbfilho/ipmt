#include "Global.h"

ull encodeInt(int arg, int argSize, int* encodedSize) {
	int sizeOfSize = SIZE_IN_BITS(argSize);

	//token eh o monte de bits gerados da codificação de 'arg'	
	ull token = 
		(1ULL<<sizeOfSize)-1 //representação unária de argSize
		| (ull(argSize) << (sizeOfSize + 1))//separador '0' + representação (binária) de size.
		| (ull(arg) << (2*sizeOfSize+1)); //acrescenta o 'arg' propriamente dito
	
	*encodedSize = 2*sizeOfSize + 1 + argSize;
	return token;
	
}
