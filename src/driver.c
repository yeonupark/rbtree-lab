#include "rbtree.h"
#include "rbtree.c"
#include <stdio.h>
void printTree(rbtree* tree);
int main(int argc, char *argv[]) {
    rbtree *t = new_rbtree();
    node_t* node;
    node_t* one;
    rbtree_insert(t, 10);
    rbtree_insert(t, 5);
    rbtree_insert(t, 8);
    rbtree_insert(t, 34);
    rbtree_insert(t, 67);
    rbtree_insert(t, 23);
    rbtree_insert(t, 156);
    rbtree_insert(t, 24);
    rbtree_insert(t, 2);
    rbtree_insert(t, 12);
    rbtree_insert(t, 24);
    rbtree_insert(t, 36);
    rbtree_insert(t, 990);
    rbtree_insert(t, 25);
    printTree(t);

    node=rbtree_find(t,5);
    if (node!=t->nil){
        printf("%d 를 발견하였습니다.\n",node->key);
    }else{
        printf("검색할 수 없습니다.\n");
    }

    node=rbtree_min(t);
    printf("최솟값은 %d 입니다.\n",node->key);

    node=rbtree_max(t);
    printf("최대값은 %d 입니다.\n",node->key);

    key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
    const size_t n = sizeof(arr) / sizeof(arr[0]);
    
    for (int i=0;i<n-5;i++){
        one=rbtree_find(t,arr[i]);
        printf("%d",one->key);
        rbtree_erase(t,one);
    }
    printf("\n");
    printTree(t);
    int a = rbtree_to_array(t, arr, n);
    printf("%d\n", *arr);
    printf("%d\n", arr[1]);
    return 0;
}

void inorderTraversal(node_t* node, node_t* nil) {
    if (node == nil) {
        return;
    }
    inorderTraversal(node->left, nil);

    printf("%d ", node->key);

    inorderTraversal(node->right, nil);
}

void printTree(rbtree* tree) {
    inorderTraversal(tree->root, tree->nil);
    printf("\n");
}