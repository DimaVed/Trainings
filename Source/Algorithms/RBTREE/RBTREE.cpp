

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "RBTREE.h"





struct tree_t* MakeTreeNode(int val) {
	struct tree_t* res = (struct tree_t*)calloc(1, sizeof(struct tree_t));
	res->key = val;
	return res;
}

struct Tree* InitRBTree() {

	struct Tree* T = (struct Tree*)calloc(1, sizeof(Tree));
	T->nil = MakeTreeNode(0);
	T->nil->color = Black;
	T->root = T->nil;
	return T;
}

struct tree_t* RBTreeSearch(struct tree_t* top, int value) {
	while (top != 0 && value != top->key) {
		if (value < top->key) {
			top = top->left;
		}
		else {
			top = top->right;
		}
	}
	return top;
}
struct tree_t* RBTreeMin(struct tree_t* top) {
	while (top->left != 0) {
		top = top->left;
	}
	return top;
}
struct tree_t* RBTreeMax(struct tree_t* top) {
	while (top->right != 0) {
		top = top->right;
	}
	return top;
}

struct tree_t* RBTreeSuccessor(struct tree_t* x) {
	if (x->right == 0) {
		return RBTreeMin(x->right);
	}
	struct tree_t* y = x->p;
	while (y != 0 && x != y->right) {
		x = y;
		y = y->p;
	}
	return y;
}

struct tree_t* TreePredcessor(struct tree_t* x) {
	if (x->left == 0) {
		return RBTreeMax(x->left);
	}
	struct tree_t* y = x->p;
	while (y != 0 && x != y->left) {
		x = y;
		y = y->p;
	}
	return y;
}


//-------------Rotation algos------------------

//   Before Rotation:
//   
//       x                                              
//        \                                             
//         y                                                         
//        / \                                                     
//       a   b                                                     
//   
//   After Left Rotation:
//   
//         y
//        / \
//       x   b
//        \
//         a


//Left Rotation Steps :
//Set y to be the right child of x.
//Move y’s left subtree to x’s right subtree.
//Update the parent of xand y.
//Update x’s parent to point to y instead of x.
//Set y’s left child to x.
//Update x’s parent to y.

void LeftRotateRB(struct Tree* T, struct tree_t* x) {
	struct tree_t* y = x->right;
	x->right = y->left;
	if (y->left != T->nil) {
		y->left->p = x;
	}
	y->p = x->p;
	if (x->p == nullptr) {
		T->root = y;
	}
	else if (x == x->p->left) {
		x->p->left = y;
	}
	else {
		x->p->right = y;
	}
	y->left = x;
	x->p = y;
}



//   Befor Right Rotation:    
//   
//         x
//        /
//       y
//      / \
//     a   b
//   
//   After Right Rotation:
//   
//       y
//      / \
//     a   x
//        /
//       b
//   
//   
//Right rotation steps  :
//Set y to be the left child of x.
//Move y’s right subtree to x’s left subtree.
//Update the parent of xand y.
//Update x’s parent to point to y instead of x.
//Set y’s right child to x.
//Update x’s parent to y.


void RightRotateRB(struct Tree* T, struct tree_t* x) {
	struct tree_t* y = x->left;
	x->left = y->right;
	if (y->right != T->nil) {
		y->right->p = x;
	}
	y->p = x->p;
	if (x->p == T->nil) {
		T->root = y;
	}
	else if (x == x->p->right) {
		x->p->right = y;
	}
	else {
		x->p->left = y;
	}
	y->right = x;
	x->p = y;
}




void TreeInsertFix(struct Tree* T, struct tree_t* z) {
	while (z->p->color == Red) {
		if (z->p == z->p->p->left) {
			struct tree_t* y = z->p->p->right;
			if (y->color == Red) {
				z->p->color == Black;
				y->color = Black;
				z->p->p->color = Red;
				z = z->p->p;
			}
			else if (z == z->p->right) {
				z = z->p;
				LeftRotateRB(T, z);
			}
			z->p->color = Black;
			z->p->p->color = Red;
			RightRotateRB(T, z->p->p);
		}
		else {
			struct tree_t* y = z->p->p->left;
			if (y->color == Red) {
				z->p->color == Black;
				y->color = Black;
				z->p->p->color = Red;
				z = z->p->p;
			}
			else if (z == z->p->left) {
				z = z->p;
				RightRotateRB(T, z);
			}
			z->p->color = Black;
			z->p->p->color = Red;
			LeftRotateRB(T, z->p->p);
		}
	}
	T->root->color = Black;
}

void RBTreeInsert(struct Tree* T, struct tree_t* z) {
	struct tree_t* y = T->nil;
	struct tree_t* x = T->root;
	while (x != T->nil) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	z->p = y;
	if (y == T->nil) {
		T->root = z;
	}
	else if (z->key < y->key) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = T->nil;
	z->right = T->nil;
	z->color = Red;
	TreeInsertFix(T, z);
}

void Transplant(struct Tree* T, struct tree_t* u, struct tree_t* v) {
	if (u->p == T->nil) {
		T->root = v;
	}
	else if (u == u->p->left) {
		u->p->left = v;
	}
	else {
		u->p->right = v;
	}
	v->p = u->p;
}

void RBTreeDelete(struct Tree* T, struct tree_t* z) {
	if (z->left == 0) {
		Transplant(T, z, z->right);
	}
	else if (z->right == 0) {
		Transplant(T, z, z->left);
	}
	else {
		struct tree_t* y = RBTreeMin(z->right);
		if (y->p != z) {
			Transplant(T, y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		Transplant(T, z, y);
		y->left = z->left;
		y->left->p = y;
	}
}

void tree_add_value(struct tree_t* top, int value) {
	// IMPL
}
void tree_has_value(struct tree_t* top, int value) {
}
void tree_visit_range(struct tree_t* top, int l, int r, visit_t v) {
}

void tree_free(struct tree_t* top) {
}