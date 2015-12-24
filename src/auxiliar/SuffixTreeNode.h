#ifndef SUFFIX_TREE_NODE
#define SUFFIX_TREE_NODE

#include <map>
using std::map;

class SuffixTreeNode {
public:
	SuffixTreeNode();
	SuffixTreeNode(int start, int end);

	void setChild(char ch, int node);
	int getChild(char ch);
	bool hasChild(char ch); 
	bool isLeaf(); 
			
	//O rótulo da aresta (pai, este nó). [start, end]
	int start, end;
	
	//O suffixLink
	int sl;
	
	//Quantidade de folhas na subárvore enraizada por este nó
	int leaves;

	// Lista de adjacência. Por enquanto é um map.
	map<char, int>* children;	
};


#endif
