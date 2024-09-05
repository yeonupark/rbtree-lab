#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
void rbtree_insert_fixup(rbtree* t,node_t * current);
void right_rotate(rbtree* t, node_t* current);
void left_rotate(rbtree* t, node_t* current);
node_t* get_rbtree_min(rbtree *t, node_t *subtree_root);
void rbtree_erase_fixup(rbtree* t, node_t* node);
void free_node(rbtree* tree, node_t* node);
void transplant(rbtree* t, node_t* original_node, node_t* new_node);
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  if(p==NULL){
    return NULL;
  }

  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  if(nil_node==NULL){
    free(p);
    return NULL;
  }
  nil_node->color=RBTREE_BLACK;
  nil_node->left = nil_node->right = nil_node->parent = nil_node;
  p->nil = nil_node;
  p->root=p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {

  if (t != NULL) {
    free_node(t, t->root);
    free(t->nil);
    free(t);
  }
}

void free_node(rbtree* tree, node_t* node) {

  if (node == tree->nil) {
    return;
  }
  free_node(tree, node->left);
  free_node(tree, node->right);
  free(node);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {

  node_t* node=(node_t*)calloc(1,sizeof(node_t));
  node-> color=RBTREE_RED;
  node->key=key;
  node->left=t->nil;
  node->right=t->nil;

  node_t *prev = t->nil;
  node_t *current = t->root;

  while (current != t->nil) {
    prev = current;
    if (key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  node->parent = prev;

  if (prev == t->nil) {
    t->root = node;
  } else if (key < prev->key) {
    prev->left = node;
  } else {
    prev->right = node;
  }

  rbtree_insert_fixup(t,node);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* start = t->root;
  if (start==t->nil){
    return NULL;
  }
  while(start!=t->nil){
    if(start->key==key){
      return start;
    }
    if(key>start->key){
      start=start->right;
    }else{
      start=start->left;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* start=t->root;

  if (start==t->nil){
    return NULL;
  }
  while (start->left!=t->nil){
    start=start->left;
  }
  return start;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* start=t->root;

  if (start==t->nil){
    return NULL;
  }
  while (start->right!=t->nil){
    start=start->right;
  }
  return start;
}
int rbtree_erase(rbtree *t, node_t *z) {
    node_t *x;
    node_t *y = z;
    color_t original_color = y->color;

    if (z->left == t->nil) {
        x = z->right;
        transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        transplant(t, z, z->left);
    } else {
        y = get_rbtree_min(t, z->right);
        original_color = y->color;
        x = y->right;
        if (y != z->right) {
            transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else {
            x->parent = y;
        }
        transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (original_color == RBTREE_BLACK) {
        rbtree_erase_fixup(t, x);
    }
    return 0;
}

// --------------------------------------------------//

void rbtree_insert_fixup(rbtree* t,node_t * current){
  while (current->parent->color==RBTREE_RED){
    node_t* uncle;
    if (current->parent==current->parent->parent->left){
      uncle=current->parent->parent->right;
      if(uncle->color==RBTREE_RED){
        current->parent->color=RBTREE_BLACK;
        uncle->color=RBTREE_BLACK;
        current->parent->parent->color=RBTREE_RED;
        current=current->parent->parent;
      }
      else{
      if (current==current->parent->right){
        current=current->parent;
        left_rotate(t,current);
      }
      current->parent->parent->color=RBTREE_RED;
      current->parent->color=RBTREE_BLACK;
      right_rotate(t,current->parent->parent);
    }

  }else{
    uncle=current->parent->parent->left;
    if(uncle->color==RBTREE_RED){
        current->parent->color=RBTREE_BLACK;
        uncle->color=RBTREE_BLACK;
        current->parent->parent->color=RBTREE_RED;
        current=current->parent->parent;
      }
      else
      {
      if (current==current->parent->left){
        current=current->parent;
        right_rotate(t,current);
      }
      current->parent->parent->color=RBTREE_RED;
      current->parent->color=RBTREE_BLACK;
      left_rotate(t,current->parent->parent);
    }
  }
  }
  t->root->color=RBTREE_BLACK;
}

void right_rotate(rbtree* t, node_t* current) {
    if (current == t->nil) {
        return;
    }

    node_t* left_child = current->left;
    if (left_child == t->nil) {
        return;
    }

    node_t* right_child_of_left = left_child->right;

    left_child->parent = current->parent;

    if (current->parent != t->nil) {
        if (current == current->parent->left) {
            current->parent->left = left_child;
        } else {
            current->parent->right = left_child;
        }
    } else {
        t->root = left_child;
    }

    left_child->right = current;
    current->parent = left_child;

    current->left = right_child_of_left;
    if (right_child_of_left != t->nil) {
        right_child_of_left->parent = current;
    }

    if (current == t->root) {
        t->root = left_child;
    }
}
void left_rotate(rbtree* t, node_t* current) {
    if (current == t->nil) {
        return;
    }

    node_t* right_child = current->right;
    if (right_child == t->nil) {
        return;
    }

    node_t* left_child_of_right = right_child->left;

    right_child->parent = current->parent;

    if (current->parent != t->nil) {
        if (current == current->parent->right) {
            current->parent->right = right_child;
        } else {
            current->parent->left = right_child;
        }
    } else {
        t->root = right_child;
    }

    right_child->left = current;
    current->parent = right_child;

    current->right = left_child_of_right;
    if (left_child_of_right != t->nil) {
        left_child_of_right->parent = current;
    }

    if (current == t->root) {
        t->root = right_child;
    }
}

void transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
  v->parent = u->parent;

}

node_t* get_rbtree_min(rbtree *t, node_t *n) {
    if (n == NULL) {
    return NULL;
  }
  node_t *x = n;
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}

void rbtree_erase_fixup(rbtree* t, node_t* node) {
  while (node != t->root && node->color == RBTREE_BLACK) {
    if (node == node->parent->left) {
      node_t* brother = node->parent->right;
      if (brother->color == RBTREE_RED) {
        brother->color=RBTREE_BLACK;
        node->parent->color=RBTREE_RED;
        left_rotate(t, node->parent);
        brother = node->parent->right;
      }
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK) {
        brother->color = RBTREE_RED;
        node = node->parent;
      }
      else {
        if (brother->right->color == RBTREE_BLACK) {
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          right_rotate(t, brother);
          brother = node->parent->right;
        }
        brother->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        brother->right->color = RBTREE_BLACK;
        left_rotate(t, node->parent);
        node = t->root;
      }
    }
    else {
      node_t* brother = node->parent->left;
      if (brother->color == RBTREE_RED) {
        brother->color = RBTREE_BLACK;
        node->parent->color = RBTREE_RED;
        right_rotate(t, node->parent);
        brother = node->parent->left;
      }
      if (brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK) {
        brother->color = RBTREE_RED;
        node = node->parent;
      }
      else {
        if (brother->left->color == RBTREE_BLACK) {
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          left_rotate(t, brother);
          brother = node->parent->left;
        }
        brother->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        brother->left->color = RBTREE_BLACK;
        right_rotate(t, node->parent);
        node = t->root;
      }
    }
  }
  node->color = RBTREE_BLACK;
}

void inorder_to_array(const rbtree *t, node_t* node, key_t **arr) {
  if (node != t->nil) {
    // 왼쪽 서브트리 순회
    inorder_to_array(t, node->left, arr);

    // 현재 노드의 키를 배열에 저장하고, 배열 포인터 +1 해서 다음 배열 주소로 넘어가기
    **arr = node->key;
    (*arr)++;

    // 오른쪽 서브트리 순회
    inorder_to_array(t, node->right, arr);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t->root == t->nil){
    return 0;
  }
  // 중위 순회 시작
  // inorder_to_array(t,t->root,arr,n,0);
  
  inorder_to_array(t, t->root, &arr);

  return 0;  // 성공적으로 배열에 변환됨
}