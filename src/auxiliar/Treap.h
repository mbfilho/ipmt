#ifndef TREAP_H
#define TREAP_H
#include "Global.h"
#include "TreapNode.h"

template <class Comparator>
class Treap {
public:
	TreapNode* root;
	Comparator comparator;

	Treap(Comparator c): comparator(c) {
		root = NULL;
	}

	~Treap() {
		deallocateAll(root);
	}

	void deallocateAll(TreapNode* root) {
		if(!root) return;
		deallocateAll(root->right);
		deallocateAll(root->left);
		delete root;
	}

	//Coloca todo mundo <= val em L e o restante em R
	void insert(int value) {
		TreapNode* newNode = new TreapNode(value);

		if(root == NULL) 
			root = newNode;
		else {
			TreapNode* L, *R;
			split(root, value, L, R);
			R = merge(newNode, R);
			root = merge(L, R);
		}
	}
	void remove(int value) {
		root = _remove(root, value);
	}

	void printInOrder() {
		_printInOrder(root);
		printf("\n");	
	}

	void _printInOrder(TreapNode* root) {
		if(!root) return;

		_printInOrder(root->left);
		printf("%d ", root->key);
		_printInOrder(root->right);
	}
private:
	TreapNode* _remove(TreapNode* root, int value) {
		if(root->key == value) {
			TreapNode* tmp = merge(root->left, root->right);
			delete root;
			return tmp;
		}
		if(comparator(root->key, value)) //key < value  
			root->right = _remove(root->right, value);
		else	
			root->left = _remove(root->left, value);

		return root;
	}
	TreapNode* merge(TreapNode* L, TreapNode* R) {
		if(L == NULL) return R;
		if(R == NULL) return L;

		if(L->y < R->y) {
			L->right = merge(L->right, R);
			return L;
		} else {
			R->left = merge(L, R->left);
			return R;
		}
	}
	void split(TreapNode* root, int val, TreapNode*& L, TreapNode*& R) {
		if(root == NULL)  //árvore vazia
			L = R = NULL;
		else {
			if(comparator(val, root->key)) { //val < key
				R = root;
				split(root->left, val, L, root->left);
			} else { //val <= key
				L = root;
				split(root->right, val, root->right, R);
			}
		}
	}

};


#endif
