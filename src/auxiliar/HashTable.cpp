#include "HashTable.h"

int HashTable::getIndex(pii key) {
	return (key.first * B + key.second) % TABLE_SIZE;
}

void HashTable::put(pii key, int value) {
	table[getIndex(key)].push_back(make_pair(key, value));
}

int HashTable::get(pii key) {
	int ret = -1, idx = getIndex(key);
	for(int i = 0; i < table[idx].size(); ++i) {
		if(table[idx][i].first == key)
			return table[idx][i].second;
	}
	return -1;
}
