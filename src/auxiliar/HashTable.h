#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "Global.h"

typedef pair<int,int> pii;

//"Arguably the single most important data structure known to mankind"
class HashTable {
public:
	
	void put(pii key, int value);
	int get(pii key);

private:
	int getIndex(pii key);

	//A base utilizada no cálculo dos hashes
	static const ull B = (1ULL << 32) - 1;

	static const int TABLE_SIZE = 1 << 20;

	vector<pair<pii,int> > table[TABLE_SIZE];
};
#endif
