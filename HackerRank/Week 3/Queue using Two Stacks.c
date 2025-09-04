#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

// Two stacks
int stack1[MAX], stack2[MAX];
int top1 = -1, top2 = -1;

// Stack operations
void push1(int x) { stack1[++top1] = x; }
int pop1() { return stack1[top1--]; }
int peek1() { return stack1[top1]; }
int isEmpty1() { return top1 == -1; }

void push2(int x) { stack2[++top2] = x; }
int pop2() { return stack2[top2--]; }
int peek2() { return stack2[top2]; }
int isEmpty2() { return top2 == -1; }

// Queue operations
void enqueue(int x) {
    push1(x);
}

void dequeue() {
    if (isEmpty2()) {
        while (!isEmpty1()) {
            push2(pop1());
        }
    }
    if (!isEmpty2()) pop2();
}

void printFront() {
    if (isEmpty2()) {
        while (!isEmpty1()) {
            push2(pop1());
        }
    }
    if (!isEmpty2()) {
        printf("%d\n", peek2());
    }
}

int main() {
    int q;
    scanf("%d", &q);
    while (q--) {
        int type;
        scanf("%d", &type);
        if (type == 1) {
            int x;
            scanf("%d", &x);
            enqueue(x);
        } else if (type == 2) {
            dequeue();
        } else if (type == 3) {
            printFront();
        }
    }
    return 0;
}

