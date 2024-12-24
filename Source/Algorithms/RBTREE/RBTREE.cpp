

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RBTREE.h"





struct tree_t* MakeTreeNode(int val) {
	struct tree_t* res = (struct tree_t*)calloc(1, sizeof(struct tree_t));
	res->key = val;
	res->right = res->left = res->p = 0;
	return res;
}

struct Tree* InitRBTree() {
	struct Tree* T = (struct Tree*)calloc(1, sizeof(Tree));
	T->nil = MakeTreeNode(0);
	T->nil->color = BLACK;
	T->root = 0;
	return T;
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

void LeftRotateRB(struct Tree* T, struct tree_t* node) {
	struct tree_t* child = node->right;
	node->right = child->left;
	if (node->right != 0)
		node->right->p = node;
	child->p = node->p;
	if (node->p == 0)
		T->root = child;
	else if (node == node->p->left)
		node->p->left = child;
	else
		node->p->right = child;
	child->left = node;
	node->p = child;
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


void RightRotateRB(struct Tree* T, struct tree_t* node) {
	struct tree_t* child = node->left;
	node->left = child->right;
	if (node->left != 0)
		node->left->p = node;
	child->p = node->p;
	if (node->p == 0)
		T->root = child;
	else if (node == node->p->left)
		node->p->left = child;
	else
		node->p->right = child;
	child->right = node;
	node->p = child;
}

void swap_color(struct tree_t* l, struct tree_t* r) {
	enum color_t tmp = l->color;
	l->color = r->color;
	r->color = l->color;
}

//----------------------------------
void fixInsert(struct Tree* T, struct tree_t* node)
{
	struct tree_t* parent = nullptr;
	struct tree_t* grandparent = nullptr;
	while (node != T->root && node->color == RED
		&& node->p->color == RED) {
		parent = node->p;
		grandparent = parent->p;
		if (parent == grandparent->left) {
			struct tree_t* uncle = grandparent->right;
			if (uncle != nullptr
				&& uncle->color == RED) {
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				node = grandparent;
			}
			else {
				if (node == parent->right) {
					LeftRotateRB(T,parent);
					node = parent;
					parent = node->p;
				}
				RightRotateRB(T,grandparent);

				swap_color(parent, grandparent);
				node = parent;
			}
		}
		else {
			struct tree_t* uncle = grandparent->left;
			if (uncle != nullptr
				&& uncle->color == RED) {
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				node = grandparent;
			}
			else {
				if (node == parent->left) {
					RightRotateRB(T,parent);
					node = parent;
					parent = node->p;
				}
				LeftRotateRB(T,grandparent);
				swap_color(parent, grandparent);
				node = parent;
			}
		}
	}
	T->root->color = BLACK;
}



void fixDelete(struct Tree* T, struct tree_t* node)
{
	while (node != T->root && node->color == BLACK) {
		if (node == node->p->left) {
			struct tree_t* sibling = node->p->right;
			if (sibling->color == RED) {
				sibling->color = BLACK;
				node->p->color = RED;
				LeftRotateRB(T,node->p);
				sibling = node->p->right;
			}
			if ((sibling->left == nullptr
				|| sibling->left->color == BLACK)
				&& (sibling->right == nullptr
					|| sibling->right->color
					== BLACK)) {
				sibling->color = RED;
				node = node->p;
			}
			else {
				if (sibling->right == nullptr
					|| sibling->right->color == BLACK) {
					if (sibling->left != nullptr)
						sibling->left->color = BLACK;
					sibling->color = RED;
					RightRotateRB(T,sibling);
					sibling = node->p->right;
				}
				sibling->color = node->p->color;
				node->p->color = BLACK;
				if (sibling->right != nullptr)
					sibling->right->color = BLACK;
				LeftRotateRB(T,node->p);
				node = T->root;
			}
		}
		else {
			struct tree_t* sibling = node->p->left;
			if (sibling->color == RED) {
				sibling->color = BLACK;
				node->p->color = RED;
				RightRotateRB(T,node->p);
				sibling = node->p->left;
			}
			if ((sibling->left == nullptr
				|| sibling->left->color == BLACK)
				&& (sibling->right == nullptr
					|| sibling->right->color
					== BLACK)) {
				sibling->color = RED;
				node = node->p;
			}
			else {
				if (sibling->left == nullptr
					|| sibling->left->color == BLACK) {
					if (sibling->right != nullptr)
						sibling->right->color = BLACK;
					sibling->color = RED;
					LeftRotateRB(T,sibling);
					sibling = node->p->left;
				}
				sibling->color = node->p->color;
				node->p->color = BLACK;
				if (sibling->left != nullptr)
					sibling->left->color = BLACK;
				RightRotateRB(T,node->p);
				node = T->root;
			}
		}
	}
	node->color = BLACK;
}



struct tree_t*  RBTreeMin(struct tree_t* node) {
	struct tree_t* current = node;
	while (current->left != nullptr)
		current = current->left;
	return current;

}


struct tree_t* RBTreeMax(struct tree_t* node) {
	while ( node->right)
	{
		node = node->right;
	}
	return node;
}


void transplant(struct tree_t *root, struct tree_t* u, struct tree_t* v)
{
	if (u->p == 0)
		root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	if (v != nullptr)
		v->p = u->p;
}



void printHelper(struct tree_t* root, char* indent, int last)
{
	if (root != 0) {
		printf ("%s",  indent);
		if (last) {
			printf("%s", "R---:");
			strcat(indent, "   ");
			
		}
		else {
			printf("%s", "L---:");
			strcat(indent , "|  ");
		}
		
		printf("%d", root->key);
		printf("(");
		printf(root->color == RED ? "RED" : "BLACK");
		printf(")\n");
		char ind1[2024]{0};
		char ind2[2024]{ 0 };
		strcpy(ind1, indent);
		strcpy(ind2, indent);


		printHelper(root->left, ind1, false);
		printHelper(root->right, ind2, true);
	}
}


void insert(struct Tree* T, int key)
{
	struct tree_t* node = MakeTreeNode(key);
	struct tree_t* parent = 0;
	struct tree_t* current = T->root;
	while (current != 0) {
		parent = current;
		if (node->key < current->key)
			current = current->left;
		else
			current = current->right;
	}
	node->p = parent;
	if (parent == 0)
		T->root = node;
	else if (node->key < parent->key)
		parent->left = node;
	else
		parent->right = node;
	fixInsert(T,node);
}

void remove(struct Tree* T,int  key)
{
	struct tree_t* node = T->root;
	struct tree_t* z = nullptr;
	struct tree_t* x = nullptr;
	struct tree_t* y = nullptr;
	while (node != nullptr) {
		if (node->key == key) {
			z = node;
		}

		if (node->key <= key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}

	if (z == nullptr) {
		printf ( "Key not found in the tree" );
		return;
	}

	y = z;
	color_t yOriginalColor = y->color;
	if (z->left == 0) {
		x = z->right;
		transplant(T->root, z, z->right);
	}
	else if (z->right == 0) {
		x = z->left;
		transplant(T->root, z, z->left);
	}
	else {
		y = RBTreeMin(z->right);
		yOriginalColor = y->color;
		x = y->right;
		if (y->p == z) {
			if (x != 0)
				x->p = y;
		}
		else {
			transplant(T->root, y, y->right);
			y->right = z->right;
			y->right->p = y;
		}
		transplant(T->root, z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	delete z;
	if (yOriginalColor == BLACK) {
		fixDelete(T,x);
	}
}

void printTree(struct Tree* T)
{
	if (T->root == 0)
		printf ( "Tree is empty.\n" );
	else {
		printf("Red-Black Tree:\n" );
		char indent[1024]{0};
		printHelper( T->root, indent, true);
	}
}



struct tree_t* Search(Tree* T, int value)
{
	struct tree_t* x = T->root;
	while ( x!=0&& value !=x->key)
	{
		if (value < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	return x;
}

struct tree_t* LowerBound(struct tree_t* node) {
	

	struct tree_t* res;

	// To do impl lower bound
	return res;

}
