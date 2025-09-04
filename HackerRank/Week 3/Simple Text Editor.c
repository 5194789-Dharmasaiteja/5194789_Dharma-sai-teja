#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int type;    // 1 = inverse is delete k; 2 = inverse is append str
    int k;       // used when inverse is delete k (from an original append)
    char *str;   // used when inverse is append str (from an original delete)
} Op;

typedef struct {
    Op *data;
    int top;
    int cap;
} Stack;

static void stack_init(Stack *st, int cap) {
    st->data = (Op*)malloc(sizeof(Op) * cap);
    st->top = 0;
    st->cap = cap;
}
static void stack_push(Stack *st, Op op) {
    st->data[st->top++] = op;
}
static Op stack_pop(Stack *st) {
    return st->data[--st->top];
}
static int stack_empty(Stack *st) { return st->top == 0; }
static void stack_free(Stack *st) {
    // free only dynamically allocated fields that remain (should be none if used correctly)
    for (int i = 0; i < st->top; i++) {
        if (st->data[i].type == 2 && st->data[i].str) free(st->data[i].str);
    }
    free(st->data);
}

/* Dynamic string buffer for editor text */
typedef struct {
    char *buf;
    int len;
    int cap;
} DStr;

static void dstr_init(DStr *s) {
    s->cap = 1;
    s->len = 0;
    s->buf = (char*)malloc(s->cap);
    s->buf[0] = '\0';
}
static void dstr_ensure(DStr *s, int need_extra) {
    int need = s->len + need_extra + 1;
    if (need > s->cap) {
        while (s->cap < need) {
            if (s->cap < 1024) s->cap *= 2;
            else s->cap += s->cap / 2; // grow 1.5x for large sizes
        }
        s->buf = (char*)realloc(s->buf, s->cap);
    }
}
static void dstr_append(DStr *s, const char *w, int wlen) {
    dstr_ensure(s, wlen);
    memcpy(s->buf + s->len, w, wlen);
    s->len += wlen;
    s->buf[s->len] = '\0';
}
static void dstr_delete_last(DStr *s, int k) {
    if (k > s->len) k = s->len;
    s->len -= k;
    s->buf[s->len] = '\0';
}
static void dstr_free(DStr *s) { free(s->buf); }

int main(void) {
    int Q;
    if (scanf("%d", &Q) != 1) return 0;

    DStr text;
    dstr_init(&text);

    Stack undo;
    stack_init(&undo, Q);

    // temp buffer for reading appended strings (total input length <= 1e6)
    // Using a large static buffer to avoid repeated allocations.
    static char tmp[1000005];

    for (int i = 0; i < Q; i++) {
        int type;
        scanf("%d", &type);

        if (type == 1) {
            // append W
            scanf("%s", tmp);
            int wlen = (int)strlen(tmp);

            // perform
            dstr_append(&text, tmp, wlen);

            // push inverse: delete wlen
            Op op;
            op.type = 1;   // inverse is delete k
            op.k = wlen;
            op.str = NULL;
            stack_push(&undo, op);

        } else if (type == 2) {
            // delete k
            int k;
            scanf("%d", &k);
            if (k > text.len) k = text.len;

            // store the deleted substring for undo
            Op op;
            op.type = 2;   // inverse is append str
            op.k = 0;
            op.str = (char*)malloc(k + 1);
            memcpy(op.str, text.buf + (text.len - k), k);
            op.str[k] = '\0';

            // perform delete
            dstr_delete_last(&text, k);

            // push inverse
            stack_push(&undo, op);

        } else if (type == 3) {
            // print k-th character
            int k;
            scanf("%d", &k);
            // problem guarantees valid k
            if (k >= 1 && k <= text.len) {
                putchar(text.buf[k - 1]);
                putchar('\n');
            } else {
                // If ever invalid, print newline to be safe
                putchar('\n');
            }

        } else if (type == 4) {
            // undo last (modifying) operation
            if (!stack_empty(&undo)) {
                Op op = stack_pop(&undo);
                if (op.type == 1) {
                    // inverse of append: delete k
                    dstr_delete_last(&text, op.k);
                    // nothing to free
                } else if (op.type == 2) {
                    // inverse of delete: append stored string
                    int wlen = (int)strlen(op.str);
                    dstr_append(&text, op.str, wlen);
                    free(op.str);
                }
            }
        }
    }

    dstr_free(&text);
    stack_free(&undo);
    return 0;
}
