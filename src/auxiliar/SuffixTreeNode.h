#ifndef SUFFIX_TREE_NODE
#define SUFFIX_TREE_NODE

#include <vector>
using std::vector;

class SuffixTreeNode {
public:
	SuffixTreeNode();
	SuffixTreeNode(int start, int end);
	SuffixTreeNode(const SuffixTreeNode& ot);

	void addChild(int nodeIdx, SuffixTreeNode& node);
	int getChild(char ch, const char* text, vector<SuffixTreeNode>& nodes);
	bool isLeaf(); 
			
	//O rótulo da aresta (pai, este nó). [start, end]
	int start, end;
	
	//O suffixLink
	int sl;
	
	//Quantidade de folhas na subárvore enraizada por este nó
	int leaves;

	int firstChild;

	int sibling;
};


#endif
