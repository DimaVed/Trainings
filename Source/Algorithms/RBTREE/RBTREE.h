#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

enum color_t { RED, BLACK };

struct tree_t {
  struct tree_t* left;
  struct tree_t* right;
  struct tree_t* p;
  int key;
  enum color_t color;
};

struct Tree {
  struct tree_t* root;
  struct tree_t* nil;
};

typedef void (*visit_t)(const struct tree_t*);


struct Tree* InitRBTree();
void printTree(struct Tree* T);
void remove(struct Tree* T, int  key);
void insert(struct Tree* T, int key);
struct tree_t* UpperBound(Tree* T, int value);
struct tree_t* Next(struct tree_t* cur);
struct tree_t* Prev(struct tree_t* cur);

struct tree_t* Search(struct Tree* T, int value);
struct tree_t* RBTreeMin(struct tree_t* top);
struct tree_t* RBTreeMax(struct tree_t* node);
struct tree_t* MakeTreeNode(int val);

//
void tree_add_value(struct tree_t* top, int value);
void tree_has_value(struct tree_t* top, int value);
void tree_visit_range(struct tree_t* top, int l, int r, visit_t v);
void tree_free(struct tree_t* top);

