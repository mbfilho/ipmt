#ifndef SUFFIX_TREE
#define SUFFIX_TREE

#include "Global.h"
#include "SuffixTreeNode.h"
#include "Index.h"
#include "Printer.h"

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
	
	void findMatchings(const char* pat, size_t m, bool countOnly); 

	void serialize(Serializer* serializer); 
	void decompress(Decompressor* decompressor); 
private:
	//Os suffix links não ficam no nó para que não ocupem memória quando a árvore for carregada de um arquivo. Nessa ocasião eles não são necessários
	vector<int> suffixLinks;

	vector<SuffixTreeNode> nodes; //Os nós das árvores

	const char* text; //O texto
	size_t n; //O tamanho do texto

	int insertNodeIntoTree(int lblStart, int lblEnd);

	ImplicitPointer followSuffixLink(ImplicitPointer prt);
	void canonise(ImplicitPointer& prt);
	int split(ImplicitPointer prt, char ch, bool* isTerm);

	void fixTree(SuffixTreeNode& node);

	FILE* dotFile;//Arquivo para imprimir a árvore graficamente

	void printTree(int step);
	void _printTreeRec(int cur, int step); 

	void getMatchings(const char* pat, size_t m, SuffixTreeNode& node, int nodeHeight, Printer& printer);
};

#endif
