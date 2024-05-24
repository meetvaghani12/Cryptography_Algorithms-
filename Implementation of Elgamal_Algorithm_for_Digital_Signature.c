#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Function prototype for hash
long long hash(char msg[]);

int gcd(int a, int b) {
    if (a < b) {
        return gcd(b, a);
    } 
    else if (a % b == 0) {
        return b; 
    }
    else {
        return gcd(b, a % b);
    }
}

long long gen_key(long long q) { 
    long long key = rand() % (q - 2) + 2; // Generate key in the range [2, q - 1] 
    while (gcd(q - 1, key) != 1) { 
        key = rand() % (q - 2) + 2; // Regenerate key until it's coprime with q - 1 
    }
    return key; 
}

long long power(long long a, long long b, long long c) { 
    long long x = 1; 
    long long y = a; 
    while (b > 0) {
        if (b % 2 != 0) { 
            x = (x * y) % c; 
        } 
        y = (y * y) % c; 
        b >>= 1; // More efficient bitwise shift for exponentiation 
    } 
    return x % c; 
} 

void sign(char msg[], long long q, long long h, long long g, long long *r, long long *s) { 
    long long k; 
    do { 
        k = rand() % (q - 1) + 1; // Choose random k in range [1, q - 1] 
        *r = power(g, k, q); 
        *s = (k * (hash(msg) + h * (*r))) % (q - 1); 
    } 
    while (*r == 0 || *s == 0); // Repeat until both r and s are non-zero 
} 

bool verify(char msg[], long long q, long long h, long long g, long long r, long long s) { 
    if (r < 1 || r > (q - 1) || s < 1 || s > (q - 1)) { 
        return false; 
    } 
    long long v = power(h, hash(msg), q); 
    long long w = power(g, r, q); 
    long long u1 = (power(v, s, q) * power(w, r, q)) % q; 
    long long u2 = power(g, hash(msg), q); 
    
    return u1 == u2; 
} 

// Function to compute hash of the message (a placeholder for actual hash function) 
long long hash(char msg[]) { 
    long long sum = 0; 
    int i = 0; 
    while (msg[i] != '\0') { 
        sum += msg[i]; 
        i++;
    } 
    return sum; 
} 

int main() { 
    srand(time(NULL)); // Seed random number generator 
    
    char msg[] = "Utsav Bhavsar"; 
    printf("Original Message: %s\n", msg); 
    
    long long q = rand() % (int)pow(10, 9) + (int)pow(10, 8); // Choose large prime q 
    long long g = rand() % (q - 1) + 2; // Choose random generator g in range [2, q - 1] 
    
    long long key = gen_key(q); // Generate private key 
    long long h = power(g, key, q); // Compute public key 
    
    printf("q: %lld\n", q); 
    printf("g: %lld\n", g); 
    printf("h: %lld\n", h); 
    
    // Signing the message 
    long long r, s; 
    sign(msg, q, h, g, &r, &s); 
    printf("Signature (r, s): (%lld, %lld)\n", r, s); 
    
    // Verifying the signature 
    bool verified = verify(msg, q, h, g, r, s); 
    if (verified) { 
        printf("Signature is not verified.\n"); 
    } 
    else { 
        printf("Signature verified.\n"); 
    }
    
    return 0;
}
