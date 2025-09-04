#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char* readline();
char* ltrim(char* str);
char* rtrim(char* str);
int parse_int(char* str);

typedef struct TrieNode {
    bool isEnd;
    struct TrieNode* children[26]; // for lowercase a-z
} TrieNode;

TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEnd = false;
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL;
    }
    return node;
}

bool insert(TrieNode* root, char* word) {
    TrieNode* curr = root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int idx = word[i] - 'a';
        if (curr->children[idx] == NULL) {
            curr->children[idx] = createNode();
        }
        curr = curr->children[idx];

        // If we encounter an already finished word while still inserting -> bad
        if (curr->isEnd && i != len - 1) {
            return false;
        }
    }

    if (curr->isEnd) {
        return false; // duplicate word
    }
    curr->isEnd = true;

    // If this word is prefix of a previously inserted longer word
    for (int j = 0; j < 26; j++) {
        if (curr->children[j] != NULL) {
            return false;
        }
    }
    return true;
}

void noPrefix(int words_count, char** words) {
    TrieNode* root = createNode();

    for (int i = 0; i < words_count; i++) {
        if (!insert(root, words[i])) {
            printf("BAD SET\n");
            printf("%s\n", words[i]);
            return;
        }
    }
    printf("GOOD SET\n");
}

int main() {
    int n = parse_int(ltrim(rtrim(readline())));
    char** words = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* word = readline();
        words[i] = word;
    }

    noPrefix(n, words);
    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (1) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) break;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);
        data[data_length] = '\0';
    }
    return data;
}

char* ltrim(char* str) {
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
