

#include "RBTREE.h"
#include <assert.h>

void TestInsertion() {

struct Tree* T = InitRBTree();
  assert(T->root ==T->nil);
  TreeInsert(T, MakeTreeNode(0));

assert(T->root->key == 0);
//assert(T->root->color == Black);
}


int main() {

	TestInsertion();

assert(1 == 1);
}
