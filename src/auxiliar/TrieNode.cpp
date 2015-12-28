#include "TrieNode.h"

TrieNode::TrieNode() {
	firstChild = -1;
	sibling = -1;
}


int TrieNode::getChild(int label, vector<TrieNode>& nodes) {
	for(int cur = firstChild; cur != -1; cur = nodes[cur].sibling) {
		if(nodes[cur].edgeLabel == label)
			return cur;
	}

	return -1;
}

void TrieNode::addChild(int label, int childIdx, TrieNode& child) {
	child.sibling = firstChild;
	firstChild = childIdx;
	child.edgeLabel = label;	
}
