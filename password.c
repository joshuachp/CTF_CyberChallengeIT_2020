#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define INT_AMOUNT 4
#define PRIME (((uint32_t) 1 << 31) - 1)

// Remove this when releasing the crackme!
#define IS_DEBUG

// The password will be "CTF-AEKmgKRRIHP3"
// This gives these 4 ints: 0x49674143 0x484B4554 0x50524B46 0x33526D2D

typedef union {
    char bytes[INT_AMOUNT * sizeof(uint32_t)];
    uint32_t numbers[INT_AMOUNT];
} pass_data;

#ifdef IS_DEBUG
// Useful for showing how the data is being processed
void dump_data(pass_data data){
    for (uint32_t i = 0; i < sizeof(uint32_t) * INT_AMOUNT; i++){
        printf("0x%02X ", data.bytes[i]);
    }
    puts("");
    for (uint32_t i = 0; i < sizeof(INT_AMOUNT); i++){
        printf("0x%08X ", data.numbers[i]);
    }
    puts("");
}
#endif

uint32_t modular_exponentiation(uint32_t x, uint32_t y, uint32_t p);
uint8_t first_check(uint32_t n);


uint8_t first_check(uint32_t n) {
    return modular_exponentiation(0xDEADBEEF, n, PRIME) == 0x3863CE45;
}



// Computes x ^ y mod p (where ^ is exponentiation)
// x and y can be anything != 0, p must be prime!
uint32_t modular_exponentiation(uint32_t x, uint32_t y, uint32_t p) {
    // 64 bit because we don't want to overflow it on squaring
    uint64_t counter = 1;
    uint64_t base = x;
    while (y > 0){
        if (y % 2 == 1) {
            counter = (counter * base) % p;
        }
        base = (base * base) % p;
        y = y / 2;
    }
    return (uint32_t) counter;
}

int main(int argc, const char** argv) {
    if (argc <= 1) {
        puts("Error, you need to pass a string argument.");
        return 0;
    }

    const char* password = argv[1];
    if (strlen(password) != INT_AMOUNT * sizeof(uint32_t)) {
        puts("lol no");
        return 0;
    }

    pass_data to_compute = (const pass_data) {0};   // Initialize union to zero

    // We want to scramble the input bytes, here's some examples:
    // 0123 4567 89AB CDEF      INPUT
    // 048C 159D 26AE 37BF      OUTPUT
    //
    // 0123 4567                INPUT
    // 0415 2637                OUTPUT
    //
    // 0123 4567 89AB           INPUT
    // 0481 5926 A37B           OUTPUT
    uint32_t k = 0;
    for (uint32_t i = 0; i < sizeof(uint32_t); i++){ 
        for (uint32_t j = 0; j < INT_AMOUNT; j++){
            to_compute.bytes[k] = password[sizeof(uint32_t) * j + i];
            k++;
        }
    }
    
    #ifdef IS_DEBUG
    dump_data(to_compute);
    printf("First check: %d\n", first_check(to_compute.numbers[0]));
    #endif

}