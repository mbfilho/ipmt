#include "SuffixTreeNode.h"


SuffixTreeNode::SuffixTreeNode() {
}

SuffixTreeNode::SuffixTreeNode(int start, int end) {
	this->start = start;
	this->end = end;
	children = new map<char, int>();
	sl = -1;
}

void SuffixTreeNode::setChild(char ch, int node) {
	(*children)[ch] = node;
}

int SuffixTreeNode::getChild(char ch) {
	return (*children)[ch];
}

bool SuffixTreeNode::hasChild(char ch) {
	return (*children).count(ch) != 0;
}
