#ifndef SUFFIX_TREE_NODE
#define SUFFIX_TREE_NODE

#include "Global.h"

class SuffixTreeNode {
public:
	SuffixTreeNode();
	SuffixTreeNode(int start, int end);

	void addChild(int nodeIdx, SuffixTreeNode& node);
	int getChild(char ch, const char* text, vector<SuffixTreeNode>& nodes);
	bool isLeaf(); 
			
	//O rótulo da aresta (pai, este nó). [start, end]
	int start, end;

	/*
	* A razão de utilizarmos 'union' é porque 'leaves' só é 
	* necessária durante a fase de busca, enquanto que 'sl' só é 
	* necessário durante a fase de construção. Dessa forma 
	* economizamos memória e sem nenhum esforço.
	*/
	union {	
		//Quantidade de folhas na subárvore enraizada por este nó
		int leaves;

		//SuffixLink
		int sl;
	};

	int firstChild;

	int sibling;
};


#endif
