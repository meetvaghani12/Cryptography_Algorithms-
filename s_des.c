#include <stdio.h>

// Permutation P10
int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};

// Permutation P8
int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};

// Initial permutation IP
int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};

// Expansion permutation EP
int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};

// Permutation P4
int P4[] = {2, 4, 3, 1};

// S-Boxes
int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

void permutation(int *input, int *permutation, int size) {
    int temp[size];
    for (int i = 0; i < size; i++) {
        temp[i] = input[permutation[i] - 1];
    }
    for (int i = 0; i < size; i++) {
        input[i] = temp[i];
    }
}

void generateSubKeys(int *key, int *k1, int *k2) {
    permutation(key, P10, 10);

    // Split the 10-bit key into two 5-bit halves
    int leftHalf[5];
    int rightHalf[5];
    for (int i = 0; i < 5; i++) {
        leftHalf[i] = key[i];
        rightHalf[i] = key[i + 5];
    }

    // Circular left shift
    int temp = leftHalf[0];
    for (int i = 0; i < 4; i++) {
        leftHalf[i] = leftHalf[i + 1];
        rightHalf[i] = rightHalf[i + 1];
    }
    leftHalf[4] = temp;
    rightHalf[4] = temp;

    // Combine the halves and apply P8 permutation
    for (int i = 0; i < 5; i++) {
        key[i] = leftHalf[i];
        key[i + 5] = rightHalf[i];
    }
    permutation(key, P8, 8);

    // Save the result in k1
    for (int i = 0; i < 8; i++) {
        k1[i] = key[i];
    }

    // Circular left shift again
    temp = leftHalf[0];
    for (int i = 0; i < 4; i++) {
        leftHalf[i] = leftHalf[i + 1];
        rightHalf[i] = rightHalf[i + 1];
    }
    leftHalf[4] = temp;
    rightHalf[4] = temp;

    // Combine the halves and apply P8 permutation
    for (int i = 0; i < 5; i++) {
        key[i] = leftHalf[i];
        key[i + 5] = rightHalf[i];
    }
    permutation(key, P8, 8);

    // Save the result in k2
    for (int i = 0; i < 8; i++) {
        k2[i] = key[i];
    }
}

void initialPermutation(int *input) {
    permutation(input, IP, 8);
}

void expansionPermutation(int *input, int *expanded) {
    permutation(input, EP, 8);
    for (int i = 0; i < 8; i++) {
        expanded[i] = input[i];
    }
}

void applySBox(int *input, int *output, int sBox[4][4]) {
    int row = input[0] * 2 + input[3];
    int col = input[1] * 2 + input[2];
    int value = sBox[row][col];
    for (int i = 0; i < 2; i++) {
        output[i] = (value >> (1 - i)) & 1;
    }
}

void fFunction(int *input, int *subKey, int *output) {
    int expanded[8];
    expansionPermutation(input, expanded);
    for (int i = 0; i < 8; i++) {
        expanded[i] ^= subKey[i];
    }

    int sBoxInput1[4];
    int sBoxInput2[4];
    for (int i = 0; i < 4; i++) {
        sBoxInput1[i] = expanded[i];
        sBoxInput2[i] = expanded[i + 4];
    }

    int sBoxOutput1[2];
    int sBoxOutput2[2];
    applySBox(sBoxInput1, sBoxOutput1, S0);
    applySBox(sBoxInput2, sBoxOutput2, S1);

    for (int i = 0; i < 2; i++) {
        output[i] = sBoxOutput1[i];
        output[i + 2] = sBoxOutput2[i];
    }
    permutation(output, P4, 4);
}

void swap(int *left, int *right) {
    for (int i = 0; i < 4; i++) {
        int temp = left[i];
        left[i] = right[i];
        right[i] = temp;
    }
}

void encrypt(int *plaintext, int *key) {
    int k1[8];
    int k2[8];
    generateSubKeys(key, k1, k2);

    // Initial permutation
    initialPermutation(plaintext);

    // Split the 8-bit plaintext into two 4-bit halves
    int left[4];
    int right[4];
    for (int i = 0; i < 4; i++) {
        left[i] = plaintext[i];
        right[i] = plaintext[i + 4];
    }

    // Round 1
    int fOutput[4];
    fFunction(right, k1, fOutput);
    for (int i = 0; i < 4; i++) {
        fOutput[i] ^= left[i];
    }
    swap(left, fOutput);

    // Round 2
    fFunction(right, k2, fOutput);
    for (int i = 0; i < 4; i++) {
        fOutput[i] ^= left[i];
    }
    for (int i = 0; i < 4; i++) {
        plaintext[i] = right[i];
        plaintext[i + 4] = fOutput[i];
    }
    // Final permutation (Inverse of initial permutation)
    int finalPermutation[] = {4, 1, 3, 5, 7, 2, 8, 6};
    permutation(plaintext, finalPermutation, 8);
}
void decrypt(int *ciphertext, int *key) {
    int k1[8];
    int k2[8];
    generateSubKeys(key, k1, k2);

    // Initial permutation
    initialPermutation(ciphertext);

    // Split the 8-bit ciphertext into two 4-bit halves
    int left[4];
    int right[4];
    for (int i = 0; i < 4; i++) {
        left[i] = ciphertext[i];
        right[i] = ciphertext[i + 4];
    }

    // Round 1 (use k2 instead of k1 for decryption)
    int fOutput[4];
    fFunction(right, k2, fOutput);
    for (int i = 0; i < 4; i++) {
        fOutput[i] ^= left[i];
    }
    swap(left, fOutput);

    // Round 2 (use k1 instead of k2 for decryption)
    fFunction(right, k1, fOutput);
    for (int i = 0; i < 4; i++) {
        fOutput[i] ^= left[i];
    }
    for (int i = 0; i < 4; i++) {
        ciphertext[i] = right[i];
        ciphertext[i + 4] = fOutput[i];
    }

    // Final permutation (Inverse of initial permutation)
    int finalPermutation[] = {4, 1, 3, 5, 7, 2, 8, 6};
    permutation(ciphertext, finalPermutation, 8);
}

int main() {
    int key[] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0};   // 10-bit key

    int k1[8];
    int k2[8];
    generateSubKeys(key, k1, k2);

    printf("\n\nKEY GENERATION");
    printf("\nKey 1: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", k1[i]);
    }

    printf("\nKey 2: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", k2[i]);
    }

    int plaintext[] = {1, 0, 1, 0, 0, 1, 0, 0};  // 8-bit plaintext
    int key[] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0};   // 10-bit key
    int ciphertext[8];

    printf("\nS-DES Encryption and Decryption");
    printf("\n\nPlaintext: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", plaintext[i]);
    }

    encrypt(plaintext, key);

    printf("\nEncrypted Text: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", plaintext[i]);
    }
    decrypt(plaintext, key);

    printf("\nDecrypted Text: ");
    for (int i = 0; i < 8; i++) {
        printf("%d", plaintext[i]);
    }
    return 0;
}
