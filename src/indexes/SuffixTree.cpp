#include "SuffixTree.h"

SuffixTree::SuffixTree(const char* dotFileName) {
	dotFile = NULL;

	if(dotFileName) 
		dotFile = fopen(dotFileName, "w");
}

void SuffixTree::compress(Compressor* compressor) {
	//Escreve o tamanho do texto e o texto
	printf(">>>> %lu\n", nodes.size());
	compressor->writeInt(n);
	compressor->writeText(text, n);

	//Escreve a quantidade de nós
	compressor->writeInt(nodes.size());

	//Escreve os nós em si
	for(int i = 0; i < nodes.size(); ++i){
		if(i%10000 == 0) printf("\r%lf%% completo", 100*double(i)/nodes.size());
		SuffixTreeNode& cur = nodes[i];
		int fieldsToSave[] = {
			cur.start,
			cur.end,
			cur.leaves,
			cur.firstChild,
			cur.sibling
		};
		compressor->writeArrayOfInts(fieldsToSave, 5);
	}
	printf("\n");
}

void SuffixTree::decompress(Decompressor* decompressor) {
	n = decompressor->readInt();
	char* tmp = new char[n];
	for(int i = 0; i < n; ++i)
		tmp[i] = decompressor->readByte();
	tmp[n-1] = 0;
	text = tmp;

	//Atenção para a leitura dos nós!
	//A ordem tem que ser a mesma da escrita!
	nodes.resize(decompressor->readInt());
	for(int i = 0; i < nodes.size(); ++i){
		SuffixTreeNode& cur = nodes[i];
		cur.start = decompressor->readInt();
		cur.end = decompressor->readInt();
		cur.leaves = decompressor->readInt();
		cur.firstChild = decompressor->readInt();
		cur.sibling = decompressor->readInt();
	}
}
 
void SuffixTree::build(const char* text, size_t n) {
	this->text = text;
	this->n = n;

	insertNodeIntoTree(0, 0); //A raiz
	ImplicitPointer current(0, 1, 0);
	for(int i = 0; i < n; ++i){
		int wprime = -1;
		int w;
		bool isTerm;
		
		for(w = split(current, text[i], &isTerm); !isTerm; w = split(current, text[i], &isTerm)) {			
			//Cria uma folha representando o sufixo [i..] e 'pendura' em w 			
			int leaf = insertNodeIntoTree(i, -1);
			assert(nodes.at(w).getChild(text[i], text, nodes) == -1 && "O no atual jah tem o caracter que esta sendo acrescentado!"); 
			
			nodes.at(w).addChild(leaf, nodes.at(leaf));

			if(wprime != -1)
				suffixLinks.at(wprime) = w;

			wprime = w;	
			if(current.v == 0 && !current.isImplicit()) {
				isTerm = false; //Sinaliza que o terminador não existe
				break;
			}

			current = followSuffixLink(current);
			canonise(current);
		}
	
		assert(wprime == -1 || !current.isImplicit());

		if(!current.isImplicit() && wprime != -1)
			suffixLinks[wprime] = current.v;

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
		int toIdx = nodes.at(prt.v).getChild(text[prt.st], text, nodes);
		*isTerm = text[nodes.at(toIdx).start + prt.strSize()] == ch;

		if(*isTerm) 
			return prt.v;
		else {//Aqui 'prt' se torna explícito

			int w = insertNodeIntoTree(nodes.at(toIdx).start, nodes.at(toIdx).start + prt.strSize() - 1); //O novo locus explícito de 'prt'. A aresta (prt.v, w) é um prefixo de (prt.v, to)

			nodes.at(toIdx).start += prt.strSize(); //a futura aresta (w, to) é um sufixo da antiga aresta (prt.v, w)
			nodes.at(w).firstChild = toIdx; //pendura 'to' em 'w'

			//É preciso substituir 'to' por 'w' na lista de adjacência de 'prt.v'
			nodes.at(w).sibling = nodes.at(toIdx).sibling;
			if(nodes.at(prt.v).firstChild == toIdx) 
				nodes.at(prt.v).firstChild = w;
			else {
				int prev = -1, cur = nodes.at(prt.v).firstChild;
				while(cur != toIdx) {
					prev = cur;
					cur = nodes.at(cur).sibling;
				}
				nodes.at(prev).sibling = w;
			}
			nodes.at(toIdx).sibling = -1;
			
			return w;	
		}
	} else { //Não é implícito.
		*isTerm = (nodes.at(prt.v).getChild(ch, text, nodes) != -1);
		return prt.v;
	}
}

ImplicitPointer SuffixTree::followSuffixLink(ImplicitPointer prt){
	if(prt.v != 0)
		prt.v = suffixLinks[prt.v];
	else
		++prt.st;

	assert(prt.v != -1);

	return prt;
}

void SuffixTree::canonise(ImplicitPointer& prt){
	while(prt.isImplicit()) {
		int from = prt.v, to = nodes.at(from).getChild(text[prt.st], text, nodes);
		int size = (nodes.at(to).end == -1? n-1 : nodes.at(to).end) - nodes.at(to).start + 1;
		if(size <= prt.strSize()){
			prt.st += size;
			prt.v = to;
		}else
			break;
	}
}

int SuffixTree::insertNodeIntoTree(int lblStart, int lblEnd) {
	int newNodeIdx = nodes.size();
	nodes.push_back(SuffixTreeNode(lblStart, lblEnd));
	suffixLinks.push_back(-1);

	return newNodeIdx;
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
		Printer printer(text, n, pat, m);
		getMatchings(pat, m, nodes.at(cur), height, printer); //Registra todas as ocorrências do padrão. Para isso é preciso achar todas as folhas abaixo de 'cur'
		printer.print();//imprime as ocorrências

	}
}

/*
* Registra todas as ocorrências do padrão no texto
*/
void SuffixTree::getMatchings(const char* pat, size_t m, SuffixTreeNode& node, int nodeHeight, Printer& printer) {
	if(node.isLeaf()) //É folha
		printer.addMatching(n-nodeHeight);
	else {
		for(int nt = node.firstChild; nt != -1; nt = nodes.at(nt).sibling){
			SuffixTreeNode& next = nodes.at(nt);
			int edgeSize = next.end - next.start + 1;
			getMatchings(pat, m, next, nodeHeight + edgeSize, printer); 
		}
	}
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

	if(suffixLinks[cur] != -1)
		fprintf(dotFile, "%d -> %d [style=dotted,color=red]\n", cur, suffixLinks[cur]);

	for(int nt = node.firstChild; nt != -1; nt = nodes.at(nt).sibling){
		SuffixTreeNode& next = nodes.at(nt);
		int labelSize = (next.end == -1 ? step : next.end) - next.start + 1;
		char buffer[10000];
		sprintf(buffer, "%.*s", labelSize, text + next.start);
		if(strlen(buffer) != labelSize){
		}
//		fprintf(dotFile, "%d -> %d\n", cur, nt);
		fprintf(dotFile, "%d -> %d [label=\"%.*s\"]\n", cur, nt, labelSize, text + next.start);

		_printTreeRec(nt, step);
	}

}

