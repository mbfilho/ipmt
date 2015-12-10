#ifndef SUFFIX_TREE
#define SUFFIX_TREE

#include <vector>
#include <cstdio>
using std::vector;

#include "SuffixTreeNode.h"
#include "Index.h"

struct ImplicitPointer {
	int v, st, end;

	ImplicitPointer(int vv, int ss, int ee): v(vv), st(ss), end(ee){}

	bool isImplicit() {	return st <= end; }
	int strSize(){ return end - st + 1; }
};

class SuffixTree : public Index {
public:
	SuffixTree(const char* dotFileName);

	void build(const char* text, size_t n);
	
	ImplicitPointer followSuffixLink(ImplicitPointer prt);
	void canonise(ImplicitPointer& prt);
	int split(ImplicitPointer prt, char ch, bool* isTerm);

	vector<SuffixTreeNode> nodes;
	
	//Arquivo para imprimir a árvore graficamente
	FILE* dotFile;

	const char* text;
	size_t n;

private:
	void printTree(int step);
	void _printTreeRec(int cur, int step); 
};

#endif
