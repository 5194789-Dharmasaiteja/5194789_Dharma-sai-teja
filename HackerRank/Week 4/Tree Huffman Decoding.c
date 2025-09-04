#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int freq;
    char data;
    struct node *left, *right;
};

void decode_huff(struct node* root, char* s) {
    struct node* current = root;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '0')
            current = current->left;
        else
            current = current->right;

        if (current->left == NULL && current->right == NULL) {
            printf("%c", current->data);
            current = root;
        }
    }
}

struct node* newNode(char data) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data = data;
    temp->left = temp->right = NULL;
    return temp;
}

int main() {
    // Build a small sample Huffman tree manually
    struct node* root = newNode('*'); // root (*)
    root->left = newNode('A');        // '0' -> A
    root->right = newNode('*');       // '1'
    root->right->left = newNode('B'); // '10' -> B
    root->right->right = newNode('C');// '11' -> C

    char encoded[] = "0110110"; // Example encoding
    decode_huff(root, encoded);

    return 0;
}
