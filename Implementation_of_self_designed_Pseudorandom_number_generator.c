#include <stdio.h> 
#include <stdlib.h> 

unsigned long long int custom_rand(unsigned long long int seed) { 
    unsigned long long int multiplier = 6364136223846793005ULL; 
    unsigned long long int increment = 1442695040888963407ULL; 
    unsigned long long int modulus = 18446744073709551615ULL; // 2^64 - 1 (64-bit unsigned integer max value) 
    
    // Calculate the next pseudo-random number using LCG algorithm 
    seed = (multiplier * seed + increment) % modulus; 
    
    return seed; 
} 

int main() { 
    unsigned long long int seed = 12345; // Initial seed value 
    int i; 
    
    // Generate and print 10 pseudo-random numbers 
    
    printf("Pseudo-random numbers generated:\n"); 
    for (i = 0; i < 10; i++) { 
        seed = custom_rand(seed);
        printf("%llu\n", seed); 
    } 
    
    return 0; 
}
