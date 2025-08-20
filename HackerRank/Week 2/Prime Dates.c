#include <stdio.h>

int isLeap(int y){ return (y%400==0) || (y%4==0 && y%100!=0); }

int daysInMonth(int m, int y){
    static const int d[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (m==2) ? (isLeap(y)?29:28) : d[m-1];
}

void nextDate(int *d, int *m, int *y){
    (*d)++;
    if (*d > daysInMonth(*m, *y)) {
        *d = 1;
        (*m)++;
        if (*m > 12) { *m = 1; (*y)++; }
    }
}

int main(void){
    int d1,m1,y1,d2,m2,y2;
    if (scanf("%d-%d-%d %d-%d-%d", &d1,&m1,&y1, &d2,&m2,&y2) != 6) return 0;

    int dd=d1, mm=m1, yy=y1;
    int count = 0;

    while (1) {
        // Concatenate dd, mm, yy -> x (e.g., 02-08-2024 -> 2082024)
        int x = dd;
        x = x*100 + mm;
        x = x*10000 + yy;

        if (x % 4 == 0 || x % 7 == 0) count++;

        if (dd==d2 && mm==m2 && yy==y2) break;
        nextDate(&dd,&mm,&yy);
    }

    printf("%d\n", count);
    return 0;
}
