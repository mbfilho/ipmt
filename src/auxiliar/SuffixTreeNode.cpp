#include "SuffixTreeNode.h"


SuffixTreeNode::SuffixTreeNode() {
}

SuffixTreeNode::SuffixTreeNode(int start, int end) {
	this->start = start;
	this->end = end;
	children = new map<char, int>();
}

