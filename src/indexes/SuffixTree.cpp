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
			nodes[w].setChild(text[i], nodes.size());
			nodes.push_back(SuffixTreeNode(i, -1));
	
			if(wprime != -1)
				nodes[wprime].sl = w;

			wprime = w;	
			if(current.v == 0 && !current.isImplicit()) {
				isTerm = false; //Sinaliza que o terminador não existe
				break;
			}
			current = followSuffixLink(current);
			canonise(current);
		}

//		if(isTerm && wprime != -1)
//			nodes[wprime].sl = w;

		//Desce do terminador, se ele existir, utilizando a aresta certa		
		if(isTerm) {
			if(current.isImplicit()) //Deve ser sempre verdade que current.end = i - 1
				++current.end;
			else
				current.st = current.end = i;
		}
		
		canonise(current);
		printTree(i);
	}
	fclose(dotFile);
}

int SuffixTree::split(ImplicitPointer prt, char ch, bool* isTerm){
	if(prt.isImplicit()) {
		int to = nodes[prt.v].getChild(text[prt.st]);
		*isTerm = text[nodes[to].start + prt.strSize()] == ch;

		if(*isTerm) 
			return prt.v;
		else { //Aqui ocorre a criação de um novo nó interno
			int w = nodes.size(); //O índice do novo nó

			nodes.push_back(SuffixTreeNode(nodes[to].start, nodes[to].start + prt.strSize()-1));//O novo nó em si, com sua aresta incidente

			nodes[to].start += prt.strSize(); //Atualiza a aresta do no que agora será filho de w
			nodes[w].setChild(text[nodes[to].start], to);//Pendura 'to' em w
			nodes[prt.v].setChild(text[nodes[w].start], w);//Pendura w em v, substituindo 'to'
			
			return w;	
		}
	} else { //Não é implícito.
		*isTerm = nodes[prt.v].hasChild(ch);
		return prt.v;
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
	while(prt.isImplicit()) {
		int from = prt.v, to = nodes[from].getChild(text[prt.st]);
		int size = (nodes[to].end == -1? n-1 : nodes[to].end) - nodes[to].start + 1;
		if(size <= prt.strSize()){
			prt.st += size;
			prt.v = to;
		}else
			break;
	}
}

void SuffixTree::printTree(int step) {
	if(!dotFile)
		return;

	fprintf(dotFile, "digraph Tree%d{\n", step);
	
	for(int i = 0; i < nodes.size(); ++i)
		fprintf(dotFile, "%d [label=\"%d\"]\n", i, i);
	_printTreeRec(0, step);	

	fprintf(dotFile, "}\n");
}

void SuffixTree::_printTreeRec(int cur, int step){
	SuffixTreeNode& node = nodes[cur];

	if(node.sl != -1)
		fprintf(dotFile, "%d -> %d [style=dotted,color=red]\n", cur, node.sl);

	for(map<char,int>::iterator it = node.children->begin(); it != node.children->end(); ++it) {
		SuffixTreeNode& next = nodes[it->second];
		int labelSize = (next.end == -1 ? step : next.end) - next.start + 1;

		fprintf(dotFile, "%d -> %d [label=%.*s]\n", cur, it->second, labelSize, text + next.start);

		_printTreeRec(it->second, step);
	}

}

