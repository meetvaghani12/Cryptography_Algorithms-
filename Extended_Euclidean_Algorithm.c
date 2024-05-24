#include <stdio.h>

int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

int extendedEuclidean(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    int x1, y1;
    int gcd_val = extendedEuclidean(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd_val;
}

int main() {
    int a, b;
    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    int x, y;
    int gcd_extended = extendedEuclidean(a, b, &x, &y);
    printf("Extended Euclidean Algorithm:\n");
    printf("x = %d, y = %d, gcd = %d\n", x, y, gcd_extended);

    return 0;
}

