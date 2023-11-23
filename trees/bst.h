#ifndef BST_H
#define BST_H

typedef struct t_btree {
    int listik;
    struct t_btree *left_vetochka;
    struct t_btree *reght_vetochka;
} tree;

tree *bstree_create_node(int item);

int is_bigger_than_listik(int item, int listik);

void bstree_insert(tree *root, int item, int (*cmpf)(int, int));

void tryaska(tree *root);

void applyfix(int listik);
void bstree_apply_infix(tree *root, void (*applyf)(int));
void bstree_apply_prefix(tree *root, void (*applyf)(int));
void bstree_apply_postfix(tree *root, void (*applyf)(int));

#endif
