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
		if(!current.isImplicit() && wprime != -1){
			nodes[wprime].sl = current.v;
		}

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
	fixTree(0);
	if(dotFile)
		fclose(dotFile);
}

void SuffixTree::fixTree(int node){
	if(nodes[node].isLeaf()){
		nodes[node].end = n-1;
	}else{
		for(map<char,int>::iterator it = (nodes[node].children)->begin(); it != (nodes[node].children)->end(); ++it){
			fixTree(it->second);
		}
	}
}

void SuffixTree::printMatchingLines(const char* pat, size_t m) {
	ImplicitPointer matchPoint(0,1,0);//eh o locus do prefixo de pat com i caracteres
	
	int i = 0, to, j, height = 0;
	//Invariante: no início de cada iteração matchPoint é explícito
	while(i < m){
		if(!nodes[matchPoint.v].hasChild(pat[i]))
			break;
		to = nodes[matchPoint.v].getChild(pat[i]);
		j = nodes[to].start;
		
		//Tenta consumir os caracteres da aresta
		while(i < m && j <= nodes[to].end && pat[i] == text[j]) 
			++i, ++j;

		//Houve um mismatch
		if(i < m && j <= nodes[to].end) 
			break;

		//A aresta foi toda consumida: 'to' agora é o vértice explícito de matchPoint	
		if(j > nodes[to].end){ 
			matchPoint.v = to;
		}else // O padrão foi todo consumido: matchPoint se torna implícito, mas a iteração acaba agora.
			matchPoint.st = nodes[to].start, matchPoint.end = j - 1;
		height += nodes[to].end - nodes[to].start + 1;
	}

	if(i < m) printf("Nenhuma ocorrência encontrada\n");
	else {
		printf("Size %d\n", height);
		map<pair<int,int>, set<int> > linesAndPositions;
		getAllLines(pat, m, to, height, linesAndPositions);
		int cont = 0;
		for(map<pair<int,int>, set<int> >::iterator it = linesAndPositions.begin(); it  != linesAndPositions.end(); ++it){
			int lastColoredChar = -1;
			for(int i = (it->first).first; i <= (it->first).second; ++i){
				if(it->second.count(i)){
					++cont;
					if(lastColoredChar == -1)
						printf("\033[31m");	
					lastColoredChar = i + m - 1;
				}
				printf("%c", text[i]);
				if(i == lastColoredChar) {
					lastColoredChar = -1;
					printf("\033[0m");	
				}
			}
			printf("\n");
		}
		printf("Occs: %d\n", cont);

	}
}

/*
* Recupera todas as linhas que contém o padrão 'pat'. Para isso, 
* faz-se uma busca em profundidade para se encontrar todas as folhas
* que estão na subárvore de 'node'. As linhas são salvas no mapa.
*/
void SuffixTree::getAllLines(const char* pat, size_t m, int node, int nodeHeight, map<pair<int,int>, set<int> > &linesAndPositions ) {
	if(nodes[node].isLeaf()) //É folha
		getLine(n - nodeHeight, linesAndPositions);	
	else {
		for(map<char,int>::iterator it = (nodes[node].children)->begin(); it != (nodes[node].children)->end(); ++it){
			int next = it->second, edgeSize = nodes[next].end - nodes[next].start + 1;
			getAllLines(pat, m, next, nodeHeight + edgeSize, linesAndPositions); 
		}
	}
}

/*
* Recupera a linha que contém o casamento iniciado em 'matchStart'. 
* Preenche o mapa {linha -> posição do matching}.
*/
void SuffixTree::getLine(int matchStart, map<pair<int,int>, set<int> > &linesAndPositions ) {
	int lineSt = matchStart, lineEnd = matchStart;

	while(lineSt > 0 && text[lineSt] != '\n') --lineSt;
	if(text[lineSt] == '\n') ++lineSt;
	
	while(lineEnd < n-1 && text[lineEnd] != '\n') ++lineEnd;
	if(text[lineEnd] == '\n') --lineEnd;

	pair<int,int> line(lineSt,lineEnd);

	linesAndPositions[line].insert(matchStart);
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
	int antes = prt.v;
	if(prt.v != 0)
		prt.v = nodes[prt.v].sl;
	else
		++prt.st;
	if(prt.v == -1) {
		printf("SL de %d he -1!!!\n", antes);
		throw 2;
	}

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
		char buffer[10000];
		sprintf(buffer, "%.*s", labelSize, text + next.start);
		if(strlen(buffer) != labelSize){
			 throw false;
		}
		fprintf(dotFile, "%d -> %d [label=\"%.*s\"]\n", cur, it->second, labelSize, text + next.start);

		_printTreeRec(it->second, step);
	}

}

