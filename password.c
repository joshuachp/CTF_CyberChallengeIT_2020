#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define INT_AMOUNT 4

typedef union {
    char bytes[INT_AMOUNT * sizeof(uint32_t)];
    uint32_t numbers[INT_AMOUNT];
} pass_data;

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
    // Equivalent of python's pow(3, 1337, 17)
    printf("Test numbers: %d ^ %d %% %d = %d\n", 3, 1337, 17, modular_exponentiation(3, 1337, 17));

    printf("Before: %08X\nAfter: %d", to_compute.numbers[0], modular_exponentiation(to_compute.numbers[0], 101, 17));


}