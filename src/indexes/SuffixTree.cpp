#include "SuffixTree.h"

SuffixTree::SuffixTree(const char* dotFileName) {
	dotFile = NULL;

	if(dotFileName) 
		dotFile = fopen(dotFileName, "w");
}

void SuffixTree::build(const char* text, size_t n) {
	this->text = text;
	this->n = n;
	
	nodes.push_back(SuffixTreeNode(0,0)); //A raiz

	//dummy nodes, só para testar
	for(int i = 0; i < n; ++i ){
		map<char,int>& children = *(nodes.back().children);
		children[text[i]] = int(nodes.size());
		nodes.push_back(SuffixTreeNode(i, i+1));
		printTree(i);
	}
}

void SuffixTree::printTree(int step) {
	if(!dotFile)
		return;

	fprintf(dotFile, "graph Tree%d{\n", step);
	
	for(int i = 0; i < nodes.size(); ++i)
		fprintf(dotFile, "%d [label=\"%d\"]\n", i, i);
	_printTreeRec(0);	

	fprintf(dotFile, "}\n");
}

void SuffixTree::_printTreeRec(int cur){
	SuffixTreeNode& node = nodes[cur];

	for(map<char,int>::iterator it = node.children->begin(); it != node.children->end(); ++it) {
		SuffixTreeNode& next = nodes[it->second];
		int labelSize = (next.end == -1 ? n : next.end) - next.start;

		fprintf(dotFile, "%d -- %d [label=%.*s]\n", cur, it->second, labelSize, text + next.start);

		_printTreeRec(it->second);
	}

}

