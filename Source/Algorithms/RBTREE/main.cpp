

#include "RBTREE.h"
#include <assert.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <algorithm>
#include <numeric>

using namespace   Catch::Matchers;


TEST_CASE("Preprocessor PropertyMacro", "[Preprocessor]") {

struct Tree* T = InitRBTree();
  assert(T->root ==T->nil);
  RBTreeInsert(T, MakeTreeNode(0));

assert(T->root->key == 0);
assert(T->root->left == T->nil);
assert(T->root->right == T->nil);


RBTreeInsert(T, MakeTreeNode(-1));
RBTreeInsert(T, MakeTreeNode(1));
assert(T->root->color == Black);
assert(T->root->key == 0);
assert(T->root->left->key == -1);
assert(T->root->right->key == 1);

RBTreeInsert(T, MakeTreeNode(-2));
assert(T->root->key == 0);

}

TEST_CASE("RB TREE PropertyMacro", "[Preprocessor]") {

	const volatile int a = 4;
	struct Tree* T = InitRBTree();
	//assert(T->root == T->nil);
	RBTreeInsert(T, MakeTreeNode(1));
	RBTreeInsert(T, MakeTreeNode(2));
	RBTreeInsert(T, MakeTreeNode(3));
	RBTreeInsert(T, MakeTreeNode(4));
	RBTreeInsert(T, MakeTreeNode(5));
	RBTreeInsert(T, MakeTreeNode(6));
	RBTreeInsert(T, MakeTreeNode(7));
	RBTreeInsert(T, MakeTreeNode(8));
	RBTreeInsert(T, MakeTreeNode(9));
	assert(T->root->color == Black);

}


