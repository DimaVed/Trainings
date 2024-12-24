

#include "RBTREE.h"
#include <assert.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <algorithm>
#include <numeric>

using namespace Catch::Matchers;

TEST_CASE("RB TREE ins", "[RBTREE]") {
  struct Tree* T = InitRBTree();
  assert(T->root == 0);
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

  

  assert(26 == LowerBound(T, 25)->key);
  assert(2 == LowerBound(T, 1)->key);
  assert(2 == LowerBound(T, 2)->key);
  assert(6 == LowerBound(T, 4)->key);
  assert(7 == LowerBound(T, 7)->key);

  assert(T->root->color == BLACK);
}

TEST_CASE("RB TREE ins  color", "[RBTREE]") {
  struct Tree* T = InitRBTree();
  assert(T->root == 0);
  insert(T, 0);
  insert(T, -1);
  insert(T, 1);
  printTree(T);
  struct tree_t* not_exist = Search(T, -2);
  struct tree_t* exist = Search(T, -1);
  assert(not_exist == 0);
  assert(exist != 0);

  assert(1 == 1);
}
