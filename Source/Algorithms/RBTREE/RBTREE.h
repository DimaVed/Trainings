#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

enum color_t { Red, Black };

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
void RBTreeInsert(struct Tree* T, struct tree_t* z);
struct tree_t* MakeTreeNode(int val);

//
void tree_add_value(struct tree_t* top, int value);
void tree_has_value(struct tree_t* top, int value);
void tree_visit_range(struct tree_t* top, int l, int r, visit_t v);
void tree_free(struct tree_t* top);

