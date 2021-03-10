#include<stdio.h>
#include <math.h>
#include<stdlib.h>


int main() {
    int n, k;
    scanf("%d", &n);
    int a = (int) sqrt(n) + 2;
    int **p;
    p = (int **) malloc(sizeof(int *) * a);
    for (size_t i = 0; i < a; i++) {
        /* code */
        p[i] = (int *) malloc(sizeof(int) * a);
    }

    int temp = a - 2;
    int total = 0;
    int x = 1, y = 1;
    for (size_t i = 1; i <= n; i++) {
        /* code */
        if (i - total <= temp) {
            if (i - total < temp) {
                p[x][y++] = i;
            } else {
                p[x++][y] = i;
            }
        } else if (i - total <= 2 * temp - 1) {
            if (i - total < 2 * temp - 1) {
                p[x++][y] = i;
            } else {
                p[x][y--] = i;
            }
        } else if (i - total <= 3 * temp - 2) {
            if (i - total < 3 * temp - 2) {
                p[x][y--] = i;
            } else {
                p[x--][y] = i;
            }
        } else if (i - total <= 4 * temp - 4) {
            if (i - total < 4 * temp - 4) {
                p[x--][y] = i;
            } else {
                p[x][y++] = i;
                total = i;
                temp -= 2;
            }
        }
    }
    for (size_t i = 1; i <= a - 2; i++) {
        /* code */
        for (size_t j = 1; j <= a - 2; j++) {
            /* code */
            printf("%2d ", p[i][j]);
        }
        putchar('\n');
    }


    for (size_t i = 0; i < a; i++) {
        /* code */
        free(p[i]);
    }
    free(p);

    return 0;
}