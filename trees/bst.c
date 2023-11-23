#include "bst.h"

#include <stdio.h>
#include <stdlib.h>

tree* bstree_create_node(int item) {
    tree* vetochka = malloc(sizeof(tree));
    vetochka->listik = item;
    vetochka->left_vetochka = NULL;
    vetochka->reght_vetochka = NULL;
    return vetochka;
}

int is_bigger_than_listik(int item, int listik) {
    int res = 0;
    if (item > listik) {
        res = 1;
    }
    return res;
}

void bstree_insert(tree* root, int item, int (*cmpf)(int, int)) {
    while ((cmpf(item, root->listik) && root->reght_vetochka != NULL) ||
           (!cmpf(item, root->listik) && root->left_vetochka != NULL)) {
        if (item > root->listik) {
            root = root->reght_vetochka;
        } else {
            root = root->left_vetochka;
        }
    }
    tree* new_listik = bstree_create_node(item);
    if (item > root->listik) {
        root->reght_vetochka = new_listik;
    } else {
        root->left_vetochka = new_listik;
    }
}

void tryaska(tree* root) {
    if (root != NULL) {
        if (root->left_vetochka != NULL) tryaska(root->left_vetochka);
        if (root->reght_vetochka != NULL) tryaska(root->reght_vetochka);
        free(root);
    }
}

void applyfix(int listik) { printf("%d\n", listik); }

void bstree_apply_infix(tree* root, void (*applyf)(int)) {
    if (root->left_vetochka != NULL) bstree_apply_infix(root->left_vetochka, applyf);
    applyf(root->listik);
    if (root->reght_vetochka != NULL) bstree_apply_infix(root->reght_vetochka, applyf);
}
void bstree_apply_prefix(tree* root, void (*applyf)(int)) {
    applyf(root->listik);
    if (root->left_vetochka != NULL) bstree_apply_infix(root->left_vetochka, applyf);
    if (root->reght_vetochka != NULL) bstree_apply_infix(root->reght_vetochka, applyf);
}
void bstree_apply_postfix(tree* root, void (*applyf)(int)) {
    if (root->left_vetochka != NULL) bstree_apply_infix(root->left_vetochka, applyf);
    if (root->reght_vetochka != NULL) bstree_apply_infix(root->reght_vetochka, applyf);
    applyf(root->listik);
}
