#ifndef SUFFIX_TREE_2_H
#define SUFFIX_TREE_2_H

#include "Global.h"
#include "Index.h"
#include "SuffixTreeAux.h"
#include "Printer.h"
#include "ImplicitPointer.h"


class SuffixTree2 : public Index {
	public:
		SuffixTree2(const char* dotFileName);

		void build(const char* text, size_t n);

		void findMatchings(const char* pat, size_t m, bool countOnly); 

		void serialize(Compressor* compressor); 
		void deserialize(Decompressor* decompressor); 
	private:
		SuffixTreeAux tree;
		const char* text; //O texto
		size_t n; //O tamanho do texto

		int insertNodeIntoTree(int lblStart, int lblEnd);

		ImplicitPointer followSuffixLink(ImplicitPointer prt);
		void canonise(ImplicitPointer& prt);
		int split(ImplicitPointer prt, char ch, bool* isTerm);

		FILE* dotFile;//Arquivo para imprimir a árvore graficamente

		void printTree(int step);
		void _printTreeRec(int cur, int step); 

		void getMatchings(const char* pat, size_t m, SuffixTreeNode& node, int nodeHeight, Printer& printer);
};

#endif
