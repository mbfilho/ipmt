#include "SuffixTreeNode.h"

SuffixTreeNode::SuffixTreeNode() {
}

SuffixTreeNode::SuffixTreeNode(int start, int end) {
	this->start = start;
	this->end = end;
	firstChild = sibling = -1;
	leaves = 0;
}

void SuffixTreeNode::addChild(int nodeIdx, SuffixTreeNode& node) {
	node.sibling = firstChild;
	firstChild = nodeIdx;	
}

int SuffixTreeNode::getChild(char ch, const char* text, vector<SuffixTreeNode>& nodes) {
	for(int cur = firstChild; cur != -1; cur = nodes[cur].sibling) {
		if(text[nodes[cur].start] == ch)
			return cur;
	}
	return -1;
}

bool SuffixTreeNode::isLeaf() {
	return firstChild == -1;
}
