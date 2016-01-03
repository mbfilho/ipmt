#ifndef GLOBAL_H
#define GLOBAL_H

#include <getopt.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <cstring>
using std::queue;
using std::pair;
using std::map;
using std::make_pair;
using std::set;
using std::vector;
using std::list;
using std::make_heap;
using std::sort_heap;
using std::string;
using std::ifstream;
using std::getline;

typedef unsigned long long ull;
typedef unsigned int ui;
typedef unsigned char uchar;
typedef uchar Byte;

enum CompressionAlgorithm {
	LZ77, LZ78, LZW, NONE
};

enum IndexDataStructure {
	SUFFIX_ARRAY, SUFFIX_TREE
};

#define MIN(a,b) ((a)<(b))?(a):(b)
#define MAX(a,b) (((a) < (b))?(b):(a))
#define SIZE_IN_BITS(arg) ((arg) == 0 ? 1 : 32 - __builtin_clz(arg))

#endif
