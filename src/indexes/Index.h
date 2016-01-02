#ifndef INDEX_H
#define INDEX_H
#include <cstdlib>
#include "Serializer.h"
#include "Deserializer.h"

/*
 * A classe mãe de todos os indexes
 */
class Index {
	public:
	virtual void build(const char* text, size_t n) = 0;
	virtual void findMatchings(const char* pat, size_t m, bool countOnly) = 0; 
	virtual void serialize(Serializer* serializer) = 0; 
	virtual void deserialize(Deserializer* deserializer) = 0; 
};
#endif
