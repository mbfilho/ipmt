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
	ImplicitPointer current(0, 1, 0);

	for(int i = 0; i < n; ++i){
		int wprime = -1;
		
		int w;
		bool isTerm;
		for(w = split(current, text[i], &isTerm); !isTerm; w = split(current, text[i], &isTerm)) {			
			//Cria uma folha representando o sufixo [i..] e 'pendura' em w 			
			nodes[w].children[text[i]] = nodes.size();
			nodes.push_back(SuffixTreeNode(i, -1));
	
			if(wprime != -1)
				nodes[wprime].sl = w.v;

			wprime = w;	
			if(current.v == 0 && current.st == 1 && current.end == 0) break;
			current = followSuffixLink(current);
			canonise(current);
		}
		if(wprime != -1)
			nodes[wprime].sl = w.v;

		//Desce do terminador utilizando a aresta certa		
		if(current.end >= current.st)
			++current.end;
		else
			current.st = current.end = i;
			
		canonise(current);
	}
}

int SuffixTree::split(ImplicitPointer prt, char ch, bool* isTerm){
	if(prt.end >= prt.st) {//meio de aresta
		if(text[prt.end+1] == ch){
			*isTerm = true;
			return prt.v;
		} else {
			
		}
	}

}

ImplicitPointer SuffixTree::followSuffixLink(ImplicitPointer prt){
	if(prt.v != 0)
		prt.v = nodes[prt.v].sl;
	else
		++prt.st;
		
	return prt;
}

void SuffixTree::canonise(ImplicitPointer& prt){
	while(true) {
		int from = prt.v, to = nodes[from].children[text[prt].st];
		int size = nodes[to].end - nodes[to].start;
		if(size >= prt.end-prt.st+1){
			prt.st += size;
			prt.v = to;
		}else
			break;
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

