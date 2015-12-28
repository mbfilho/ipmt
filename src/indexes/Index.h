#ifndef INDEX
#define INDEX
#include <cstdlib>
#include "Compressor.h"
#include "Decompressor.h"

/*
 * A classe mãe de todos os indexes
 */
class Index {
	public:
	virtual void build(const char* text, size_t n) = 0;
	virtual void findMatchings(const char* pat, size_t m, bool countOnly) = 0; 
	virtual void compress(Compressor* compressor) = 0; 
	virtual void decompress(Decompressor* decompressor) = 0; 
};
#endif
