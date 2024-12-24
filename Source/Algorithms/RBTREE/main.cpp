

#include "RBTREE.h"
#include <assert.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <algorithm>
#include <numeric>

using namespace   Catch::Matchers;


TEST_CASE("RB TREE ins", "[RBTREE]") {

struct Tree* T = InitRBTree();
  assert(T->root ==0);
  insert(T, 7);
  insert(T, 3);
  insert(T, 18);
  insert(T, 10);
  insert(T, 22);
  insert(T, 8);
  insert(T, 11);
  insert(T, 26);
  insert(T, 2);
  insert(T, 6);

  printTree(T);

  struct tree_t* res = Search(T, 26);

  assert(T->root->color == BLACK);
}

TEST_CASE("RB TREE ins  color", "[RBTREE]") {

	struct Tree* T = InitRBTree();
	assert(T->root == 0);
	insert(T, 0);
	insert(T, -1);
	insert(T, 1);
	printTree(T);
	
	

	assert(1 == 1);
}


