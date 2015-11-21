#ifndef INDEX
#define INDEX
#include <cstdlib>

/*
 * A classe mãe de todos os indexes
 */
class Index {
	public:
	virtual void build(const char* text, size_t n) = 0;
};
#endif
