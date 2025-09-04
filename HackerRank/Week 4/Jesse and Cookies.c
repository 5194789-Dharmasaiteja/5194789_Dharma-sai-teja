#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'cookies' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER k
 *  2. INTEGER_ARRAY A
 */

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Simple min-heap implementation
typedef struct {
    int *arr;
    int size;
    int cap;
} MinHeap;

void heap_init(MinHeap *h, int n) {
    h->arr = (int*)malloc(sizeof(int) * (n+5));
    h->size = 0;
    h->cap = n+5;
}

void heap_swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

void heap_push(MinHeap *h, int val) {
    int i = h->size++;
    h->arr[i] = val;
    while (i > 0) {
        int p = (i-1)/2;
        if (h->arr[p] <= h->arr[i]) break;
        heap_swap(&h->arr[p], &h->arr[i]);
        i = p;
    }
}

int heap_pop(MinHeap *h) {
    if (h->size == 0) return -1;
    int root = h->arr[0];
    h->arr[0] = h->arr[--h->size];
    int i = 0;
    while (1) {
        int l = 2*i+1, r = 2*i+2, smallest = i;
        if (l < h->size && h->arr[l] < h->arr[smallest]) smallest = l;
        if (r < h->size && h->arr[r] < h->arr[smallest]) smallest = r;
        if (smallest == i) break;
        heap_swap(&h->arr[i], &h->arr[smallest]);
        i = smallest;
    }
    return root;
}

int heap_top(MinHeap *h) {
    return (h->size > 0) ? h->arr[0] : -1;
}

int cookies(int k, int A_count, int* A) {
    MinHeap h;
    heap_init(&h, A_count);

    for (int i = 0; i < A_count; i++) {
        heap_push(&h, A[i]);
    }

    int ops = 0;
    while (h.size > 0 && heap_top(&h) < k) {
        if (h.size < 2) {
            free(h.arr);
            return -1;
        }
        int least = heap_pop(&h);
        int second = heap_pop(&h);
        int new_cookie = least + 2*second;
        heap_push(&h, new_cookie);
        ops++;
    }

    free(h.arr);
    return ops;
}


int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int k = parse_int(*(first_multiple_input + 1));

    char** A_temp = split_string(rtrim(readline()));

    int* A = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int A_item = parse_int(*(A_temp + i));

        *(A + i) = A_item;
    }

    int result = cookies(k, n, A);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}