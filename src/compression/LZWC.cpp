#include "LZWC.h"

LZWC::LZWC(const char* fileName):Compressor(fileName) {
	currentNode = 0;
	trie.push_back(TrieNode());//Root
	for(int i = 0; i < 256; ++i) {
		trie.push_back(TrieNode());
		trie[0].addChild(i, trie.size()-1, trie.back());
	}
}

void LZWC::writeText(const char* text, int size) {
	for(int i = 0; i < size; ++i)
		writeByte(text[i]+128);
}

void LZWC::writeByte(int arg) {
	int nextNode = trie[currentNode].getChild(arg, trie);
	if(nextNode == -1) {
		encodeAndWriteInt(currentNode); //O indice do termo no dicionário
	
		//Insere o termo no dicionário	
		trie.push_back(TrieNode());
		trie[currentNode].addChild(arg, trie.size()-1, trie.back());
		
		currentNode = trie[0].getChild(arg, trie);
	} else 
		currentNode = nextNode;
}

void LZWC::flushAndClose() {
	if(currentNode != 0) {
		encodeAndWriteInt(currentNode);
	}

	flush(true);
	close();
}


