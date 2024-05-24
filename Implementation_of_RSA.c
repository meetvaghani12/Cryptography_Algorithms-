#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int gcd(int a, int h) {
    int temp;
    while (1) {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}

int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    int p, q, n, e, d, phi, encrypted[100], decrypted[100];
    char message[100];
    
    printf("Enter two prime numbers (p and q): ");
    scanf("%d %d", &p, &q);

    n = p * q;
    phi = (p - 1) * (q - 1);

    printf("Enter an integer e such that 1 < e < phi and gcd(e, phi) = 1: ");
    scanf("%d", &e);

    while (gcd(e, phi) != 1) {
        printf("Invalid e. Please enter again: ");
        scanf("%d", &e);
    }

    int k = 2;
    while ((1 + k * phi) % e != 0)
        k++;

    d = (1 + k * phi) / e;

    printf("Public key (n, e): (%d, %d)\n", n, e);
    printf("Private key (n, d): (%d, %d)\n", n, d);

    printf("Enter message to encrypt: ");
    scanf("%s", message);

    int len = strlen(message);
    for (int i = 0; i < len; i++) {
        encrypted[i] = mod_exp(message[i], e, n);
    }
    printf("Encrypted message: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", encrypted[i]);
    }
    printf("\n");

    printf("Decrypted message: %s",message);

    printf("\n");

    return 0;
}

