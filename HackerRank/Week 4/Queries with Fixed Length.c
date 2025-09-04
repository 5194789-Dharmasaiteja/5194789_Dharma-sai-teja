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

// Deque structure for sliding window maximum
typedef struct {
    int *data;
    int front, back;
} Deque;

Deque* create_deque(int n) {
    Deque* dq = malloc(sizeof(Deque));
    dq->data = malloc(n * sizeof(int));
    dq->front = 0;
    dq->back = -1;
    return dq;
}

bool empty(Deque* dq) {
    return dq->front > dq->back;
}

int front(Deque* dq) {
    return dq->data[dq->front];
}

int back(Deque* dq) {
    return dq->data[dq->back];
}

void pop_front(Deque* dq) {
    dq->front++;
}

void pop_back(Deque* dq) {
    dq->back--;
}

void push_back(Deque* dq, int val) {
    dq->data[++dq->back] = val;
}

// Function to compute answer for one query
int min_of_max_subarrays(int* arr, int n, int k) {
    Deque* dq = create_deque(n);
    int result = INT_MAX;

    for (int i = 0; i < n; i++) {
        // remove indices out of window
        if (!empty(dq) && front(dq) <= i - k) {
            pop_front(dq);
        }

        // maintain decreasing deque
        while (!empty(dq) && arr[back(dq)] <= arr[i]) {
            pop_back(dq);
        }
        push_back(dq, i);

        // record window maximum
        if (i >= k - 1) {
            int window_max = arr[front(dq)];
            if (window_max < result) {
                result = window_max;
            }
        }
    }

    free(dq->data);
    free(dq);

    return result;
}

int* solve(int arr_count, int* arr, int queries_count, int* queries, int* result_count) {
    int* results = malloc(queries_count * sizeof(int));
    *result_count = queries_count;

    for (int qi = 0; qi < queries_count; qi++) {
        int k = queries[qi];
        results[qi] = min_of_max_subarrays(arr, arr_count, k);
    }

    return results;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));
    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    char** arr_temp = split_string(rtrim(readline()));
    int* arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = parse_int(*(arr_temp + i));
    }

    int* queries = malloc(q * sizeof(int));
    for (int i = 0; i < q; i++) {
        queries[i] = parse_int(ltrim(rtrim(readline())));
    }

    int result_count;
    int* result = solve(n, arr, q, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", result[i]);
        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }
    fprintf(fptr, "\n");

    fclose(fptr);
    return 0;
}

// Helpers (same as before)
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);
    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) return NULL;
    }
    if (data[data_length - 1] == '\n') data[data_length - 1] = '\0';
    return data;
}

char* ltrim(char* str) {
    if (!str) return NULL;
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return NULL;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;
    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
