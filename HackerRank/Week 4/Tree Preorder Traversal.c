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
    if(root == NULL) {
        struct node* node = (struct node*)malloc(sizeof(struct node));
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        return node;
    } else {
        if(data <= root->data) {
            root->left = insert(root->left, data);
        } else {
            root->right = insert(root->right, data);
        }
        return root;
    }
}

void preOrder(struct node *root) {
    if (root == NULL) return;

    printf("%d ", root->data);   // Print root
    preOrder(root->left);        // Go left
    preOrder(root->right);       // Go right
}

int main() {
    struct node* root = NULL;
    int t, data;

    scanf("%d", &t);
    while(t-- > 0) {
        scanf("%d", &data);
        root = insert(root, data);
    }

    preOrder(root);
    return 0;
}
