#include "SuffixTree2.h"

SuffixTree2::SuffixTree2(const char* dotFileName) {
	dotFile = NULL;

	if(dotFileName) 
		dotFile = fopen(dotFileName, "w");
}

void SuffixTree2::serialize(Compressor* compressor) {
	//Escreve o tamanho do texto e o texto
	compressor->feedRawBits(n, 32);
	int tot = n + tree.treeSize, saved = 0, percentageToPrint = 0;
	for(int i = 0; i < n; ++i) {
		compressor->feedRawBits(text[i], 8);
		++saved;
		if(percentageToPrint  <= saved) {
			printf("\r%.2lf%% concluído", double(saved)/tot * 100);
			fflush(stdout);
			percentageToPrint += tot/10;
		}
	}
		
	int sizeOfIntegers = SIZE_IN_BITS(int(n-1)); 

	//Escreve a quantidade de nós
	compressor->feedRawBits(tree.treeSize, 32);

	//Serializa a raiz 	
	compressor->feedRawBits(tree.getFieldValue(0, START), sizeOfIntegers);
	compressor->feedRawBits(tree.getFieldValue(0, END), sizeOfIntegers);
	queue<int> queue;
	queue.push(0);
	++saved;

	while(!queue.empty()) { //fila não vazia
		int cur = queue.front();
		queue.pop();
		
		for(int nt = tree.getFieldValue(cur, FIRST_CHILD); nt != -1; nt = tree.getFieldValue(nt, SIBLING)) {
			compressor->feedRawBits(0,1);
			compressor->feedRawBits(tree.getFieldValue(nt, START), sizeOfIntegers);
			int end = tree.getFieldValue(nt, END);
			compressor->feedRawBits(end == -1 ? n-1: end, sizeOfIntegers);
			
			queue.push(nt);
			++saved;
		
			if(percentageToPrint <= saved) {
				printf("\r%.2lf%% concluído", double(saved)/tot * 100);
				fflush(stdout);
				percentageToPrint += tot/10;
			}
		}
		compressor->feedRawBits(1,1);
	}
	printf("\n");
}

void SuffixTree2::deserialize(Decompressor* decompressor) {
	n = decompressor->readBits(32);
	int tot = n, percentageToPrint = 0;
	 
	char* tmp = new char[n];
	for(int i = 0; i < n; ++i) {
		tmp[i] = decompressor->readBits(8);
		if(i >= percentageToPrint) {
			printf("\rDescomprimindo string ... %.2lf%% concluído", 100 * double(i) / tot);
			fflush(stdout);
			percentageToPrint += n / 10;
		}
	}
	printf("\n");
	tmp[n-1] = 0;
	text = tmp;
	

	int totalOfNodes = decompressor->readBits(32), sizeOfIntegers = SIZE_IN_BITS(int(n-1));
	tree.setup(totalOfNodes);
	tree.resize(totalOfNodes);
	tot = totalOfNodes, percentageToPrint = 0;

	queue<int> queue;
	int start = decompressor->readBits(sizeOfIntegers), end = decompressor->readBits(sizeOfIntegers), curSize = 1;
	tree.insertNode(start, end);
	queue.push(0);

	while(!queue.empty()) {
		bool endOfChildren = decompressor->readBits(1);
		if(endOfChildren)
			queue.pop();
		else {
			start = decompressor->readBits(sizeOfIntegers);
			end = decompressor->readBits(sizeOfIntegers);
			tree.insertNode(start, end);
			tree.addChild(queue.front(), curSize);
			queue.push(curSize++);
			
			if(curSize >= percentageToPrint) {
				printf("\rDescomprimindo restante do índice ... %.2lf%% concluído", 100 * double(curSize) / tot);
				fflush(stdout);
				percentageToPrint += tot / 10;
			}
		}
	}
	printf("\n");
	percentageToPrint = 0;
	for(int i = tree.treeSize - 1; i >= 0; --i) {
		if((tot-i) >= percentageToPrint) {
			printf("\rFinalizando estrutura de dados ... %.2lf%% concluído", 100 * double(i) / tot);
			fflush(stdout);
			percentageToPrint += tot / 10;
		}
		
		int leaves = 0, firstChild = tree.getFieldValue(i, FIRST_CHILD);
		if(firstChild == -1)
			leaves = 1;

		for(int nt = firstChild; nt != -1; nt = tree.getFieldValue(nt, SIBLING)) 
				leaves += tree.getFieldValue(nt, LEAVES);
		tree.setFieldValue(i, LEAVES, leaves);
	}
	printf("\n\n\n");
}
 
void SuffixTree2::build(const char* text, size_t n) {
	this->text = text;
	this->n = n;
	tree.setup(n);

	insertNodeIntoTree(0, 0); //A raiz
	ImplicitPointer current(0, 1, 0);
	int tot = n, built  = 0, percentageToPrint = 0;
	for(int i = 0; i < n; ++i){
		++built;
		if(percentageToPrint <= built) {
			printf("\rÁrvore %.2lf%% concluída", double(built)/tot * 100);
			fflush(stdout);
			percentageToPrint += tot/10;
		}
		int wprime = -1;
		int w;
		bool isTerm;
		
		for(w = split(current, text[i], &isTerm); !isTerm; w = split(current, text[i], &isTerm)) {			
			//Cria uma folha representando o sufixo [i..] e 'pendura' em w 			
			int leaf = insertNodeIntoTree(i, -1);
			
			tree.addChild(w, leaf);
			if(wprime != -1)
				tree.setFieldValue(wprime, SUFFIX_LINK, w);

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
			tree.setFieldValue(wprime, SUFFIX_LINK, current.v);

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
	printf("\n");
	if(dotFile)
		fclose(dotFile);
}

int SuffixTree2::split(ImplicitPointer prt, char ch, bool* isTerm){
	if(prt.isImplicit()) {
		int toIdx = tree.getChild(prt.v, text[prt.st], text);
		int toStart = tree.getFieldValue(toIdx, START);
		*isTerm = text[toStart + prt.strSize()] == ch;

		if(*isTerm) 
			return prt.v;
		else {//Aqui 'prt' se torna explícito

			int w = insertNodeIntoTree(toStart, toStart + prt.strSize() - 1); //O novo locus explícito de 'prt'. A aresta (prt.v, w) é um prefixo de (prt.v, to)

			tree.setFieldValue(toIdx, START, toStart + prt.strSize());
			tree.setFieldValue(w, FIRST_CHILD, toIdx);

			//É preciso substituir 'to' por 'w' na lista de adjacência de 'prt.v'
			tree.setFieldValue(w, SIBLING, tree.getFieldValue(toIdx, SIBLING));
			int vFirstChild = tree.getFieldValue(prt.v, FIRST_CHILD);
			if(vFirstChild == toIdx) 
				tree.setFieldValue(prt.v, FIRST_CHILD, w);
			else {
				int prev = -1, cur = vFirstChild;
				while(cur != toIdx) {
					prev = cur;
					cur = tree.getFieldValue(cur, SIBLING);
				}
				tree.setFieldValue(prev, SIBLING, w);
			}
			tree.setFieldValue(toIdx, SIBLING, -1);
			
			return w;	
		}
	} else { //Não é implícito.
		*isTerm = (tree.getChild(prt.v, ch, text) != -1);
		return prt.v;
	}
}

ImplicitPointer SuffixTree2::followSuffixLink(ImplicitPointer prt){
	if(prt.v != 0)
		prt.v = tree.getFieldValue(prt.v, SUFFIX_LINK);
	else
		++prt.st;

	assert(prt.v != -1);

	return prt;
}

void SuffixTree2::canonise(ImplicitPointer& prt){
	while(prt.isImplicit()) {
		int from = prt.v, to = tree.getChild(from, text[prt.st], text);
		int end = tree.getFieldValue(to, END);
		int size = (end == -1? n-1 : end) - tree.getFieldValue(to, START) + 1;
		if(size <= prt.strSize()){
			prt.st += size;
			prt.v = to;
		}else
			break;
	}
}

int SuffixTree2::insertNodeIntoTree(int lblStart, int lblEnd) {
	tree.insertNode(lblStart, lblEnd);
	tree.setFieldValue(tree.treeSize-1, SUFFIX_LINK, -1);

	return tree.treeSize-1;
}

void SuffixTree2::findMatchings(const char* pat, size_t m, bool countOnly) {
	int i = 0; //tamanho do prefixo de pat já casado
	int curIdx = 0; //locus do vértice logo abaixo do locus de 'pat'
	SuffixTreeNode cur;
	int height = 0; //altura do vértice 'cur'
	
	while(i < m){//enquanto o padrão não foi todo consumido
		curIdx = tree.getChild(curIdx, pat[i], text); 
		if(curIdx == -1) //não dá para estender o padrão
			break;
		cur = tree.getNode(curIdx);
		height += cur.end - cur.start + 1;

		//Tenta consumir os caracteres da aresta
		int j = cur.start;
		while(i < m && j <= cur.end && pat[i] == text[j]) 
			++i, ++j;

		//Houve um mismatch
		if(i < m && j <= cur.end) 
			break;
	}
	int occs = 0;
	if(i == m)
		occs = cur.leaves;
	printf("%d ocorrências encontradas\n", occs);

	//imprime as ocorrências
	if(!countOnly && i == m) {
		Printer printer(text, n, pat, m);
		getMatchings(pat, m, cur, height, printer); //Registra todas as ocorrências do padrão. Para isso é preciso achar todas as folhas abaixo de 'cur'
		printer.print();//imprime as ocorrências

	}
}

/*
* Registra todas as ocorrências do padrão no texto
*/
void SuffixTree2::getMatchings(const char* pat, size_t m, SuffixTreeNode& node, int nodeHeight, Printer& printer) {
	if(node.isLeaf()) //É folha
		printer.addMatching(n-nodeHeight);
	else {
		for(int nt = node.firstChild; nt != -1; nt = tree.getFieldValue(nt, SIBLING)){
			SuffixTreeNode next = tree.getNode(nt);
			int edgeSize = next.end - next.start + 1;
			getMatchings(pat, m, next, nodeHeight + edgeSize, printer); 
		}
	}
}

void SuffixTree2::printTree(int step) {
	if(!dotFile)
		return;

	fprintf(dotFile, "digraph Tree_%d{\n", step);
	
	for(int i = 0; i < tree.treeSize; ++i)
		fprintf(dotFile, "%d [label=\"%d\"]\n", i, i);
	_printTreeRec(0, step);	

	fprintf(dotFile, "}\n");
}

void SuffixTree2::_printTreeRec(int cur, int step){
	SuffixTreeNode node = tree.getNode(cur);

	if(node.sl != -1)
		fprintf(dotFile, "%d -> %d [style=dotted,color=red]\n", cur, node.sl);

	for(int nt = node.firstChild; nt != -1; nt = tree.getFieldValue(nt, SIBLING)){
		SuffixTreeNode next = tree.getNode(nt);
		int labelSize = (next.end == -1 ? step : next.end) - next.start + 1;
		if(step == n-1 && next.end == -1) { //o caractere '$' faz parte do label
			fprintf(dotFile, "%d -> %d [label=\"%.*s", cur, nt, labelSize-1, text+next.start);
			fprintf(dotFile, "$\"]\n");
		} else
	 		fprintf(dotFile, "%d -> %d [label=\"%.*s\"]\n", cur, nt, labelSize, text + next.start);

		_printTreeRec(nt, step);
	}

}

