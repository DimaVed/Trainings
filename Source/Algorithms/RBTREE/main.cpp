

#include "RBTREE.h"
#include <assert.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <algorithm>
#include <numeric>
#include <set>

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

  CHECK(26 == UpperBound(T, 25)->key);
  CHECK(2  == UpperBound(T, 1)->key);
  CHECK(3 == UpperBound(T, 2)->key);
  CHECK(6 == UpperBound(T, 4)->key);
  CHECK(8 == UpperBound(T, 7)->key);
  CHECK(18 == UpperBound(T, 12)->key);
  CHECK(0 == UpperBound(T, 27));

  assert(T->root->color == BLACK);
}

TEST_CASE("std:: map RB Tree", "[RBTREE]") {
	std::set <int> s;
	s.insert( 7);
	s.insert( 3);
	s.insert( 18);
	s.insert( 10);
	s.insert( 22);
	s.insert( 8);
	s.insert( 11);
	s.insert( 26);
	s.insert( 2);
	s.insert( 6);

	assert(26 == *s.upper_bound( 25));
	
	
	auto it = s.upper_bound(1);
	CHECK(2 ==  *s.upper_bound(1));
	CHECK(3 ==  *s.upper_bound( 2));
	CHECK(6 ==  *s.upper_bound( 4));
	CHECK(8 ==  *s.upper_bound( 7));
	CHECK(18 == *s.upper_bound( 12));
	CHECK(s.end() ==s.upper_bound( 27));


	
}

TEST_CASE("Check Iterate ++", "[RBTREE]") {


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

	struct tree_t* it = RBTreeMin(T->root);
	CHECK(it->key == 2);
	it = Next(it);
	CHECK(it->key == 3);
	it = Next(it);
	CHECK(it->key == 6);
	it = Next(it);
	CHECK(it->key == 7);
	it = Next(it);
	CHECK(it->key == 8);
	it = Next(it);
	CHECK(it->key == 10);
	it = Next(it);
	CHECK(it->key == 11);
	it = Next(it);
	CHECK(it->key == 18);
	it = Next(it);
	CHECK(it->key == 22);
	it = Next(it);
	CHECK(it->key == 26);
	it = Next(it);
	CHECK(it ==0);
}

TEST_CASE("Check Iterate --", "[RBTREE]") {


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

	struct tree_t* it = RBTreeMax(T->root);
	CHECK(it->key == 26);
	it = Prev(it);
	CHECK(it->key == 22);
	it = Prev(it);
	CHECK(it->key == 18);
	it = Prev(it);
	CHECK(it->key == 11);
	it = Prev(it);
	CHECK(it->key == 10);
	it = Prev(it);
	CHECK(it->key == 8);
	it = Prev(it);
	CHECK(it->key == 7);
	it = Prev(it);
	CHECK(it->key == 6);
	it = Prev(it);
	CHECK(it->key == 3);
	it = Prev(it);
	CHECK(it->key == 2);
	it = Prev(it);
	CHECK(it == 0);
}