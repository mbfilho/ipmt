#include "LZ78C.h"

LZ78C::LZ78C(const char* fileName): Compressor(fileName) {
	currentNode = 0;
	trie.push_back(TrieNode());//Root
}

void LZ78C::writeByte(int arg) {
	int nextNode = trie[currentNode].getChild(arg, trie);
	if(nextNode == -1) {
		encodeAndWriteInt(currentNode); //O indice do termo no dicionário
		encodeAndWriteInt(arg); //O índice do elemento 'mismatching'	
	
		//Insere o termo no dicionário	
		trie.push_back(TrieNode());
		trie[currentNode].addChild(arg, trie.size()-1, trie.back());
		
		currentNode = 0;
	} else 
		currentNode = nextNode;
}


void LZ78C::flushAndClose() {
	if(currentNode != 0) {
		encodeAndWriteInt(currentNode);
		encodeAndWriteInt(256);//Um caracter imaginário fora do alfabeto
	}

	flush(true);
	close();
}


