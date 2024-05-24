#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Function to compute the greatest common divisor (GCD) of two numbers
long long gcd(long long a, long long b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

// Function to compute modular exponentiation: (base^exp) % mod
long long power(long long base, long long exp, long long mod) {
    long long result = 1;
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

// Function to generate keys for ElGamal digital signature
void generateElGamalKeys(long long *privateKey, long long *publicKey, long long p, long long g) {
    // Choose a random private key 'x' in the range [2, p-2]
    *privateKey = rand() % (p - 2) + 2;

    // Compute the corresponding public key 'y'
    *publicKey = power(g, *privateKey, p);
}

// Function to sign a message using ElGamal digital signature
void signMessage(const char *message, long long *signature, long long privateKey, long long p, long long g) {
    // Generate a random number 'k' in the range [2, p-2]
    long long k = rand() % (p - 2) + 2;

    // Compute the first part of the signature: r = (g^k mod p)
    signature[0] = power(g, k, p);

    // Compute the hash of the message
    long long hashMessage = 0;
    int i = 0;
    while (message[i] != '\0') {
        hashMessage += message[i];
        i++;
    }

    // Compute the second part of the signature: s = (k^(-1) * (hash(message) - privateKey * r) mod (p - 1))
    long long inverseK = 0;
    while ((k * inverseK) % (p - 1) != 1) {
        inverseK++;
    }

    signature[1] = (inverseK * (hashMessage - privateKey * signature[0])) % (p - 1);
}

// Function to verify a digital signature using ElGamal
bool verifySignature(const char *message, const long long *signature, long long publicKey, long long p, long long g) {
    // Compute the hash of the message
    long long hashMessage = 0;
    int i = 0;
    while (message[i] != '\0') {
        hashMessage += message[i];
        i++;
    }

    // Compute the value v1 = (y^r mod p)
    long long v1 = power(publicKey, signature[0], p);
    // Compute the value v2 = (r^s mod p)
    long long v2 = power(signature[0], signature[1], p);

    // Compute the value v3 = ((v1 * v2) mod p)
    long long v3 = (v1 * v2) % p;

    // Compute the value v4 = (g^hashMessage mod p)
    long long v4 = power(g, hashMessage, p);

    // If v3 is equal to v4 (mod p), the signature is valid; otherwise, it's invalid
    return v3 == v4;
}

int main() {
    srand(time(NULL));

    // Choose a large prime 'p' and a generator 'g'
    long long p = 104729; // A large prime number
    long long g = 3; // A primitive root modulo 'p'

    // Generate keys for digital signature
    long long privateKey, publicKey;
    generateElGamalKeys(&privateKey, &publicKey, p, g);
    printf("Private Key: %lld\n", privateKey);
    printf("Public Key: %lld\n", publicKey);

    // Message to be signed
    const char *message = "Hello, world!";

    // Sign the message
    long long signature[2];
    signMessage(message, signature, privateKey, p, g);
    printf("Signature (r, s): (%lld, %lld)\n", signature[0], signature[1]);

    // Verify the signature
    bool isValid = verifySignature(message, signature, publicKey, p, g);
    if (isValid) {
        printf("Signature is valid.\n");
    } else {
        printf("Signature is invalid.\n");
    }

    return 0;
}
