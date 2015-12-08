#ifndef SUFFIX_TREE
#define SUFFIX_TREE

#include <vector>
#include <cstdio>
using std::vector;

#include "SuffixTreeNode.h"
#include "Index.h"

class SuffixTree : public Index {
public:
	SuffixTree(const char* dotFileName);

	void build(const char* text, size_t n);
	
	vector<SuffixTreeNode> nodes;
	
	//Arquivo para imprimir a árvore graficamente
	FILE* dotFile;

	const char* text;
	size_t n;

private:
	void printTree(int step);
	void _printTreeRec(int cur); 
};

#endif
