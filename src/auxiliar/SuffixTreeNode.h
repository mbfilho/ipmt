#ifndef SUFFIX_TREE_NODE
#define SUFFIX_TREE_NODE

#include <map>
using std::map;

class SuffixTreeNode {
public:
	SuffixTreeNode();
	SuffixTreeNode(int start, int end);
		
	//O rótulo da aresta (pai, este nó). [start, end)
	int start, end;

	// Lista de adjacência. Por enquanto é um map.
	map<char, int>* children;	
};


#endif
