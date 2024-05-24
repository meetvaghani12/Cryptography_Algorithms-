#include<stdio.h>
#include <stdbool.h>

int power(int x, int n, int mod) {
    int result = 1;
    while (n) {
        if (n & 1)
            result = (result * x) % mod;
        n = n / 2;
        x = (x * x) % mod;  
    }
    return result;
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

int modInverse(int a, int m) {
    int x, y;
    extendedEuclidean(a, m, &x, &y);
    return (x % m + m) % m;
}

int generator(int p)
{
    bool arr[p];
    for(int i=1;i<p;i++)
    {
        arr[i] = 0;
    }
    int i = 2;
    while(1)
    {
        for(int j=1;j<=p-1;j++)
        {
            int g = power(i,j,p);
            if(arr[g])
            {
                break;
            }
            else
            {
                arr[g] = 1;
                if(j==p-1)
                {
                    return i;
                }
            }
        }
        i++;
        for(int i=1;i<p;i++)
        {
            arr[i] = 0;
        }
    }
}

int main() {
    int p = 43, g, x = 7, r = 26, y, M = 14, c1, c2, k, Plaintext;
    g = generator(p);
    y = power(g, x, p);
    c1 = power(g, r, p);
    c2 = (power(y, r, p) * M) % p;
    k = power(c1, x, p);
    Plaintext = (c2 * modInverse(k, p)) % p;

    printf("Public key: (%d,%d,%d)\n", p, g, y);
    printf("Private key: (%d,%d,%d)\n", p, g, x);
    printf("cipher text pair: (%d,%d)\n", c1, c2);
    printf("Decrypted Plaintext: %d\n", Plaintext);

    return 0;
}
