#ifndef SUFFIX_TREE_NODE
#define SUFFIX_TREE_NODE

#include "Global.h"

class SuffixTreeNode {
public:
	SuffixTreeNode();
	SuffixTreeNode(int start, int end);

	void addChild(int nodeIdx, SuffixTreeNode& node,  vector<SuffixTreeNode>& nodes);
	int getChild(char ch, const char* text, vector<SuffixTreeNode>& nodes);
	bool isLeaf(); 
			
	//O rótulo da aresta (pai, este nó). [start, end]
	int start, end;
	
	//Quantidade de folhas na subárvore enraizada por este nó
	int leaves;

	int firstChild;

	int sibling;
	int previous;
};


#endif
