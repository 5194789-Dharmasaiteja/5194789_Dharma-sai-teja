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
char* ltrim(char* str);
char* rtrim(char* str);
char** split_string(char* str);

int parse_int(char* str);

// -------- Generate first q primes --------
void generate_primes(int q, int* primes) {
    int count = 0;
    int num = 2;
    while (count < q) {
        int is_prime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes[count++] = num;
        }
        num++;
    }
}

/*
 * Complete the 'waiter' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY number
 *  2. INTEGER q
 */
int* waiter(int number_count, int* number, int q, int* result_count) {
    int* primes = malloc(q * sizeof(int));
    generate_primes(q, primes);

    int capacity = number_count * (q + 1);
    int* result = malloc(capacity * sizeof(int));
    int res_idx = 0;

    // Work stack A starts as the given numbers
    int* A = malloc(number_count * sizeof(int));
    int A_size = number_count;
    for (int i = 0; i < number_count; i++) {
        A[i] = number[i];
    }

    for (int i = 0; i < q; i++) {
        int prime = primes[i];
        int* nextA = malloc(A_size * sizeof(int));
        int nextA_size = 0;
        int* B = malloc(A_size * sizeof(int));
        int B_size = 0;

        // Process stack A from top to bottom (reverse order given)
        for (int j = A_size - 1; j >= 0; j--) {
            if (A[j] % prime == 0) {
                B[B_size++] = A[j];
            } else {
                nextA[nextA_size++] = A[j];
            }
        }

        // Add B (in the order popped) to result
        for (int j = B_size - 1; j >= 0; j--) {
            result[res_idx++] = B[j];
        }

        free(B);
        free(A);
        A = nextA;
        A_size = nextA_size;
    }

    // After q iterations, append remaining A stack
    for (int j = A_size - 1; j >= 0; j--) {
        result[res_idx++] = A[j];
    }

    free(A);
    free(primes);

    *result_count = res_idx;
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    char** number_temp = split_string(rtrim(readline()));

    int* number = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int number_item = parse_int(*(number_temp + i));
        *(number + i) = number_item;
    }

    int result_count;
    int* result = waiter(n, number, q, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));
        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");
    fclose(fptr);

    return 0;
}

// ----------- Helpers -------------
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
        if (!data) return '\0';
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
    if (!str) return '\0';
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
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
