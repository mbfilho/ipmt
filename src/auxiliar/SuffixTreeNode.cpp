#include "SuffixTreeNode.h"
#include <cstdio>

SuffixTreeNode::SuffixTreeNode() {
}

SuffixTreeNode::SuffixTreeNode(int start, int end) {
	this->start = start;
	this->end = end;
	sl = firstChild = sibling = -1;
	leaves = 0;
}

SuffixTreeNode::SuffixTreeNode(const SuffixTreeNode& ot) {
	start = ot.start;
	end = ot.end;
	sl = ot.sl;
	firstChild = ot.firstChild;
	sibling = ot.sibling;
	leaves = ot.leaves;
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
