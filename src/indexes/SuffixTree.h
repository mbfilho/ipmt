#ifndef SUFFIX_TREE
#define SUFFIX_TREE

#include <vector>
#include <cstdio>
#include <set>
#include <map>
#include <algorithm>
#include <cstring>
using std::pair;
using std::map;
using std::make_pair;
using std::set;
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

	void findMatchings(const char* pat, size_t m, bool countOnly); 
private:
	void fixTree(SuffixTreeNode& node);

	void printTree(int step);
	void _printTreeRec(int cur, int step); 


	void getAllLines(const char* pat, size_t m, SuffixTreeNode& node, int nodeHeight, map<pair<int,int>, set<int> >& linesAndPositions);
	void getLine(int matchStart, map<pair<int,int>, set<int> >& linesAndPositions);
};

#endif
