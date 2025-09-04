#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

typedef struct {
    ll *a;
    int size;
    int cap;
} Heap;

void heap_init(Heap *h) {
    h->cap = 16;
    h->size = 0;
    h->a = (ll*)malloc(sizeof(ll) * h->cap);
}

void heap_free(Heap *h) {
    free(h->a);
    h->a = NULL;
    h->size = 0;
    h->cap = 0;
}

void heap_swap(ll *x, ll *y) {
    ll t = *x; *x = *y; *y = t;
}

void heap_ensure_cap(Heap *h) {
    if (h->size >= h->cap) {
        h->cap <<= 1;
        h->a = (ll*)realloc(h->a, sizeof(ll) * h->cap);
    }
}

void heap_push(Heap *h, ll val) {
    heap_ensure_cap(h);
    int i = h->size++;
    h->a[i] = val;
    // sift up
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->a[p] <= h->a[i]) break;
        heap_swap(&h->a[p], &h->a[i]);
        i = p;
    }
}

int heap_empty(Heap *h) {
    return h->size == 0;
}

ll heap_top(Heap *h) {
    return h->a[0];
}

void heap_sift_down(Heap *h, int i) {
    while (1) {
        int l = 2*i + 1;
        int r = 2*i + 2;
        int smallest = i;
        if (l < h->size && h->a[l] < h->a[smallest]) smallest = l;
        if (r < h->size && h->a[r] < h->a[smallest]) smallest = r;
        if (smallest == i) break;
        heap_swap(&h->a[i], &h->a[smallest]);
        i = smallest;
    }
}

void heap_pop(Heap *h) {
    if (h->size == 0) return;
    h->a[0] = h->a[h->size - 1];
    h->size--;
    if (h->size > 0) heap_sift_down(h, 0);
}

// Remove matching tops between two heaps (lazy deletion)
void clean_tops(Heap *H, Heap *D) {
    while (!heap_empty(H) && !heap_empty(D) && heap_top(H) == heap_top(D)) {
        heap_pop(H);
        heap_pop(D);
    }
}

int main(void) {
    int q;
    if (scanf("%d", &q) != 1) return 0;

    Heap H, D; // main heap and delete heap
    heap_init(&H);
    heap_init(&D);

    for (int i = 0; i < q; i++) {
        int type;
        scanf("%d", &type);
        if (type == 1) {
            ll x; scanf("%lld", &x);
            heap_push(&H, x);
        } else if (type == 2) {
            ll x; scanf("%lld", &x);
            // lazy delete: push to delete-heap
            heap_push(&D, x);
            clean_tops(&H, &D);
        } else if (type == 3) {
            clean_tops(&H, &D);
            if (!heap_empty(&H)) {
                printf("%lld\n", heap_top(&H));
            }
        }
    }

    heap_free(&H);
    heap_free(&D);
    return 0;
}
