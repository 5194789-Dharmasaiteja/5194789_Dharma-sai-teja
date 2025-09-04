#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *left;
    struct node *right;
};

struct node* insert(struct node* root, int data) {
    if (root == NULL) {
        struct node* node = (struct node*)malloc(sizeof(struct node));
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        return node;
    } else {
        if (data <= root->data) {
            root->left = insert(root->left, data);
        } else {
            root->right = insert(root->right, data);
        }
        return root;
    }
}

/* Find Lowest Common Ancestor in BST */
struct node* lca(struct node* root, int v1, int v2) {
    if (root == NULL) return NULL;

    if (v1 < root->data && v2 < root->data)
        return lca(root->left, v1, v2);
    else if (v1 > root->data && v2 > root->data)
        return lca(root->right, v1, v2);
    else
        return root;
}

int main() {
    struct node* root = NULL;
    int t, data;
    scanf("%d", &t);
    while (t-- > 0) {
        scanf("%d", &data);
        root = insert(root, data);
    }
    int v1, v2;
    scanf("%d%d", &v1, &v2);
    struct node *ans = lca(root, v1, v2);
    printf("%d", ans->data);
    return 0;
}
