#ifndef GLOBAL_H
#define GLOBAL_H

#include <getopt.h>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>

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
	SUFFIX_ARRAY, SUFFIX_TREE, SUFFIX_TREE2
};

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
extern ull encodeInt(int arg, int argSize, int* encodedSize);

#define MIN(a,b) ((a)<(b))?(a):(b)
#define MAX(a,b) (((a) < (b))?(b):(a))
#define SIZE_IN_BITS(arg) ((arg) == 0 ? 1 : 32 - __builtin_clz(arg))

#endif
