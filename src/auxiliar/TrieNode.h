#ifndef TRIENODE_H
#define TRIENODE_H
#include "Global.h"

class TrieNode {
public:
	TrieNode(); 

	int idx; //O valor armazenado neste nó, i.e., o índice deste do rótulo deste nó no dicionário.

	int getChild(int label, vector<TrieNode>& nodes);

	/*
	* Premissas: 'child' ainda não foi inserido na árvore (i.e., não é filho de ninguém) E este nó não tem um filho com label = 'label'
	*/
	void addChild(int label, int childIdx, TrieNode& child);

private:
	int firstChild;//O primeiro filho deste nó
	
	int sibling;//O irmão deste nó. Ou seja, o próximo filho da lista de filhos do pai deste nó
	
	int edgeLabel;//O rótulo da aresta (pai, esteNó)
};

#endif
