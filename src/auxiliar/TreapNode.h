#ifndef TREAP_NODE_H
#define TREAP_NODE_H

class TreapNode {
public:
	int key, y;
	TreapNode* left, *right;
	
	TreapNode(int key) {
		this->key = key;
		y = rand();
		left = right = NULL;
	}
};

#endif
