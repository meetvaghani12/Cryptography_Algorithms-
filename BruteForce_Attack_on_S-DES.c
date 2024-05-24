#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Pair
{
    int *first;
    int *second;
} Pair;

int B2I(int a, int b)
{
    return (a << 1) + b;
}

int *generateKey(int n, int sizeArr)
{
    int *key = (int *)malloc(sizeArr * sizeof(int));
    for (int i = 0; i < sizeArr; i++)
    {
        key[sizeArr - 1 - i] = n % 2;
        n >>= 1;
    }
    return key;
}

int *Pn(int key[], int P[], int n)
{
    int *temp = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        temp[i] = key[P[i] - 1];
    }
    return temp;
}

int *axorb(int arr1[], int arr2[], int n)
{
    int *temp = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        temp[i] = arr1[i] ^ arr2[i];
    }
    return temp;
}

int *SBOX(int key1xorep[])
{
    int S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
    int S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

    int l_xor[4], r_xor[4];
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            l_xor[i] = key1xorep[i];
        else
            r_xor[i - 4] = key1xorep[i];
    }

    int r1 = B2I(l_xor[0], l_xor[3]);
    int c1 = B2I(l_xor[1], l_xor[2]);
    int r2 = B2I(r_xor[0], r_xor[3]);
    int c2 = B2I(r_xor[1], r_xor[2]);

    int *val1 = generateKey(S0[r1][c1], 2);
    int *val2 = generateKey(S1[r2][c2], 2);

    int outs0s1[4];
    for (int i = 0; i < 4; i++)
    {
        if (i < 2)
            outs0s1[i] = val1[i];
        else
            outs0s1[i] = val2[i - 2];
    }
    int *Ps0s1 = Pn(outs0s1, (int[]){2, 4, 3, 1}, 4);
    free(val1);
    free(val2);
    return Ps0s1;
}

int *LS(int shift, int arr[])
{
    int *tempKey2 = (int *)malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        if (i < 5)
            tempKey2[(5 + i - shift) % 5] = arr[i];
        else
            tempKey2[(5 + i - shift) % 5 + 5] = arr[i];
    }
    return tempKey2;
}

Pair keyGeneration(int key[])
{
    int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
    int P8Arr[] = {6, 3, 7, 4, 8, 5, 10, 9};

    int *tempKey1 = Pn(key, P10, 10);
    int *tempKey2 = LS(1, tempKey1);
    int *key1 = Pn(tempKey2, P8Arr, 8);
    int *tempKey3 = LS(2, tempKey2);
    int *key2 = Pn(tempKey3, P8Arr, 8);

    Pair result;
    result.first = key1;
    result.second = key2;

    free(tempKey1);
    free(tempKey2);
    free(tempKey3);

    return result;
}

int *step2(int l_ip[], int r_ip[], int key[])
{
    int *ep = Pn(r_ip, (int[]){4, 1, 2, 3, 2, 3, 4, 1}, 8);
    int *key1xorep = axorb(key, ep, 8);
    int *p4 = SBOX(key1xorep);
    int *lipxorp4 = axorb(l_ip, p4, 4);

    free(ep);
    free(key1xorep);
    free(p4);

    return lipxorp4;
}

int *Encryption(int key[], int data[])
{
    int *ip = Pn(data, (int[]){2, 6, 3, 1, 4, 8, 5, 7}, 8);

    int l_ip[4], r_ip[4];
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            l_ip[i] = ip[i];
        else
            r_ip[i - 4] = ip[i];
    }

    Pair keys = keyGeneration(key);
    int *lipxorp4 = step2(l_ip, r_ip, keys.first);
    int *ripxorp42 = step2(r_ip, lipxorp4, keys.second);

    int combiner42l4[8];
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            combiner42l4[i] = ripxorp42[i];
        else
            combiner42l4[i] = lipxorp4[i - 4];
    }

    int *cipherText = Pn(combiner42l4, (int[]){4, 1, 3, 5, 7, 2, 8, 6}, 8);

    free(ip);
    free(lipxorp4);
    free(ripxorp42);

    return cipherText;
}

bool isEqual(int arr1[], int arr2[])
{
    for (int i = 0; i < 8; i++)
    {
        if (arr1[i] != arr2[i])
        {
            return false;
        }
    }
    return true;
}

void display(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int key[10] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0};
    int data[3][8] = {{1, 0, 0, 1, 0, 1, 1, 1},
                      {1, 1, 0, 1, 0, 1, 0, 1},
                      {1, 0, 0, 0, 0, 1, 1, 0}};
    int ct[3][8] = {{0, 0, 1, 1, 1, 0, 0, 0},
                    {1, 0, 1, 0, 0, 1, 1, 1},
                    {0, 1, 0, 0, 1, 1, 1, 0}};

    int totalKeys = 0;
    int arrKey[1000][10];

    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < 1024; j++)
            {
                int *generatedKey = generateKey(j, 10);
                int *cipherText = Encryption(generatedKey, data[i]);
                if (isEqual(ct[i], cipherText))
                {
                    for (int k = 0; k < 10; k++)
                    {
                        arrKey[totalKeys][k] = generatedKey[k];
                    }
                    totalKeys++;
                }
                free(generatedKey);
                free(cipherText);
            }
        }
        else
        {
            int n = totalKeys;
            totalKeys = 0;
            for (int j = 0; j < n; j++)
            {
                int *cipherText = Encryption(arrKey[j], data[i]);
                if (isEqual(ct[i], cipherText))
                {
                    for (int k = 0; k < 10; k++)
                    {
                        arrKey[totalKeys][k] = arrKey[j][k];
                    }
                    totalKeys++;
                }
                free(cipherText);
            }
        }
    }
    printf("Found key: ");
    display(arrKey[0], 10);
    printf("Key used: ");
    display(key, 10);

    return 0;
}
