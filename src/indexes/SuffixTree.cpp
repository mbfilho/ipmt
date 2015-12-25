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
		printf("Ok# ? %d\n", current.strSize() == 0 || nodes[current.v].getChild(text[current.st], text, nodes) != -1);
		for(w = split(current, text[i], &isTerm); !isTerm; w = split(current, text[i], &isTerm)) {			
			//Cria uma folha representando o sufixo [i..] e 'pendura' em w 			
			nodes.push_back(SuffixTreeNode(i, -1));
			if(w >= nodes.size()) throw "xau";
			if(nodes.at(w).getChild(text[i], text, nodes) != -1) throw '*';
			nodes.at(w).addChild(nodes.size()-1, nodes.back());

			if(wprime != -1)
				nodes.at(wprime).sl = w;

	
			wprime = w;	
			if(current.v == 0 && !current.isImplicit()) {
				isTerm = false; //Sinaliza que o terminador não existe
				break;
			}

			current = followSuffixLink(current);
			printf("Ok* ? %d\n", current.strSize() == 0 || nodes[current.v].getChild(text[current.st], text, nodes) != -1);
			canonise(current);
		}

		if(wprime != -1 && current.isImplicit()) throw "wth!";

		if(!current.isImplicit() && wprime != -1){
			nodes.at(wprime).sl = current.v;
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
	fixTree(nodes.at(0));
	if(dotFile)
		fclose(dotFile);
}

void SuffixTree::fixTree(SuffixTreeNode& node){
	if(node.isLeaf()){
		node.end = n-1;
		node.leaves = 1;
	}else{
		for(int nt = node.firstChild; nt != -1; nt = nodes.at(nt).sibling) {
			fixTree(nodes.at(nt));
			node.leaves += nodes.at(nt).leaves;
		}
	}
}

int SuffixTree::split(ImplicitPointer prt, char ch, bool* isTerm){
	if(prt.isImplicit()) {
		int to = nodes.at(prt.v).getChild(text[prt.st], text, nodes);
		*isTerm = text[nodes.at(to).start + prt.strSize()] == ch;

		if(*isTerm) 
			return prt.v;
		else {//Aqui 'prt' se torna explícito

			//'w' vai passar a ser o locus do label de 'to'
			int w = nodes.size(); //O índice do novo nó (folha)
			nodes.push_back(SuffixTreeNode(nodes.at(to).start + prt.strSize(), nodes.at(to).end));
			nodes.at(w).sl = nodes.at(to).sl;
			nodes.at(w).firstChild = nodes.at(to).firstChild;

			nodes.at(to).end = nodes.at(w).start - 1; //nodes.at(to).start + prt.strSize() - 1; //A aresta de 'to' vai passar a ser um prefixo do que era
			nodes.at(to).firstChild = w;
			nodes.at(to).sl = -1;

			return to;	
		}
	} else { //Não é implícito.
		*isTerm = (nodes.at(prt.v).getChild(ch, text, nodes) != -1);
		return prt.v;
	}
}

ImplicitPointer SuffixTree::followSuffixLink(ImplicitPointer prt){
	if(prt.v != 0)
		prt.v = nodes.at(prt.v).sl;
	else
		++prt.st;

	if(prt.v == -1) throw 2;

	return prt;
}

void SuffixTree::canonise(ImplicitPointer& prt){
	while(prt.isImplicit()) {
		int from = prt.v, to = nodes.at(from).getChild(text[prt.st], text, nodes);
		printf("> From %d To %dSize %lu\n", prt.v, to, nodes.size());
		if(to == -1) {
			printf("Ideal %c\n", text[prt.st]);
			printf("O que tem pra hj:\n");
			for(int nt = nodes[from].firstChild; nt != -1; nt = nodes[nt].sibling)
				printf("|%c|\n", text[nodes[nt].start]);
		}
		int size = (nodes.at(to).end == -1? n-1 : nodes.at(to).end) - nodes.at(to).start + 1;
		if(size <= prt.strSize()){
			prt.st += size;
			prt.v = to;
		}else
			break;
	}
}

void SuffixTree::findMatchings(const char* pat, size_t m, bool countOnly) {
	int i = 0; //tamanho do prefixo de pat já casado
	int cur = 0; //locus do vértice logo abaixo do locus de 'pat'
	int height = 0; //altura do vértice 'cur'
	
	while(i < m){//enquanto o padrão não foi todo consumido
		cur = nodes.at(cur).getChild(pat[i], text, nodes);
		if(cur == -1) //não dá para estender o padrão
			break;
		height += nodes.at(cur).end - nodes.at(cur).start + 1;

		//Tenta consumir os caracteres da aresta
		int j = nodes.at(cur).start;
		while(i < m && j <= nodes.at(cur).end && pat[i] == text[j]) 
			++i, ++j;

		//Houve um mismatch
		if(i < m && j <= nodes.at(cur).end) 
			break;
	}
	int occs = 0;
	if(i == m)
		occs = nodes.at(cur).leaves;
	printf("%d ocorrências encontradas\n", occs);

	//imprime as ocorrências
	if(!countOnly && i == m) {
		map<pair<int,int>, set<int> > linesAndPositions; //Para cada linha (stPos, endPos) guardamos o início de cada casamento
		getAllLines(pat, m, nodes.at(cur), height, linesAndPositions); //preenche o mapa com todas as linhas. Para isso é preciso achar todas as folhas abaixo de 'cur'

		int cont = 0;
		for(map<pair<int,int>, set<int> >::iterator it = linesAndPositions.begin(); it  != linesAndPositions.end(); ++it){
			int pos = (it->first).first, lastPos = (it->first).second; //os limites da linha atual
			
			for(set<int>::iterator matching = (it->second).begin(); matching != (it->second).end(); ++matching){
				++cont;
				if(pos < *matching) { 
					printf("%.*s", *matching - pos, text + pos);
					printf("\033[31m");
					printf("%s", pat);
					printf("\033[0m");	
					pos = *matching + m;
				} else if(*matching + m > pos){
					printf("\033[31m");
					printf("%.*s", int(*matching + m - pos), pat + (pos-*matching));
					printf("\033[0m");	
					pos = *matching + m;
				}
			}

			if(pos <= lastPos)
				printf("%.*s", lastPos - pos + 1, text + pos);

			printf("\n");
		}
		if(occs != cont) throw "save-us!";
	}
}

/*
* Recupera todas as linhas que contém o padrão 'pat'. Para isso, 
* faz-se uma busca em profundidade para se encontrar todas as folhas
* que estão na subárvore de 'node'. As linhas são salvas no mapa.
*/
void SuffixTree::getAllLines(const char* pat, size_t m, SuffixTreeNode& node, int nodeHeight, map<pair<int,int>, set<int> > &linesAndPositions ) {
	if(node.isLeaf()) //É folha
		getLine(n - nodeHeight, linesAndPositions);	
	else {
		for(int nt = node.firstChild; nt != -1; nt = nodes.at(nt).sibling){
			SuffixTreeNode& next = nodes.at(nt);
			int edgeSize = next.end - next.start + 1;
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

void SuffixTree::printTree(int step) {
	if(!dotFile)
		return;
	static int cnt = 0;

	fprintf(dotFile, "digraph Tree%d_%d{\n", step, cnt++);
	
	for(int i = 0; i < nodes.size(); ++i)
		fprintf(dotFile, "%d [label=\"%d\"]\n", i, i);
	_printTreeRec(0, step);	

	fprintf(dotFile, "}\n");
}

void SuffixTree::_printTreeRec(int cur, int step){
	SuffixTreeNode& node = nodes.at(cur);

	if(node.sl != -1)
		fprintf(dotFile, "%d -> %d [style=dotted,color=red]\n", cur, node.sl);

	for(int nt = node.firstChild; nt != -1; nt = nodes.at(nt).sibling){
		SuffixTreeNode& next = nodes.at(nt);
		int labelSize = (next.end == -1 ? step : next.end) - next.start + 1;
		char buffer[10000];
		sprintf(buffer, "%.*s", labelSize, text + next.start);
		if(strlen(buffer) != labelSize){
		}
		fprintf(dotFile, "%d -> %d\n", cur, nt);
		//fprintf(dotFile, "%d -> %d [label=\"%.*s\"]\n", cur, nt, labelSize, text + next.start);

		_printTreeRec(nt, step);
	}

}

