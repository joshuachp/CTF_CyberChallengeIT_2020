#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define INT_AMOUNT 4
#define PRIME (((uint32_t) 1 << 31) - 1)

#define FIRST_CHECK 0x3863CE45
#define SECOND_CHECK 0x07043373
#define THIRD_CHECK
#define FOURTH_CHECK


// Remove this when releasing the crackme!
#define IS_DEBUG

// The password will be "CTF-AEKmgKRRIHP3"
// This gives these 4 ints: 0x49674143 0x484B4554 0x50524B46 0x33526D2D

typedef union {
    char bytes[INT_AMOUNT * sizeof(uint32_t)];
    uint32_t numbers[INT_AMOUNT];
} pass_data;

typedef struct {
    uint32_t numbers[INT_AMOUNT];
} vector;

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
uint32_t first_check(uint32_t n);
uint32_t second_check(uint32_t n);
uint32_t third_check(uint32_t n);
vector fourth_check(uint32_t n);

uint32_t first_check(uint32_t n) {
    return modular_exponentiation(0xDEADBEEF, n, PRIME);
}

uint32_t second_check(uint32_t n){
    //n = first_check(n);
    const uint8_t* text = (uint8_t*) "To be fair, you have to have a very high IQ to understand Rick and Morty. The humour is extremely subtle, and without a solid grasp of theoretical physics most of the jokes will go over a typical viewer's head. There's also Rick's nihilistic outlook, which is deftly woven into his characterisation- his personal philosophy draws heavily from Narodnaya Volya literature, for instance. The fans understand this stuff; they have the intellectual capacity to truly appreciate the depths of these jokes, to realise that they're not just funny- they say something deep about LIFE. As a consequence people who dislike Rick & Morty truly ARE idiots- of course they wouldn't appreciate, for instance, the humour in Rick's existential catchphrase \"Wubba Lubba Dub Dub,\" which itself is a cryptic reference to Turgenev's Russian epic Fathers and Sons. I'm smirking right now just imagining one of those addlepated simpletons scratching their heads in confusion as Dan Harmon's genius wit unfolds itself on their television screens. What fools.. how I pity them. And yes, by the way, i DO have a Rick & Morty tattoo. And no, you cannot see it. It's for the ladies' eyes only- and even then they have to demonstrate that they're within 5 IQ points of my own (preferably lower) beforehand. Nothin personnel kid";
    uint8_t* bytes = (uint8_t*)&n;
    for (uint32_t i = 0; i < 1297; i++){
        bytes[i % 4] ^= text[i];
    }
    return n;
}

uint32_t third_check(uint32_t n){   // Maybe too computation intensive, will see whether a change is warranted
    //n = second_check(n);
    const uint8_t random_permutation[256] = { 152, 209, 239, 114, 59, 44, 107, 142, 121, 201, 72, 185, 31, 204, 82, 60, 213, 205, 155, 81, 207, 61, 147, 233, 66, 87, 138, 183, 11, 220, 26, 122, 19, 198, 219, 24, 177, 106, 100, 99, 30, 143, 95, 141, 68, 124, 221, 84, 29, 105, 216, 180, 46, 232, 39, 130, 1, 245, 37, 52, 73, 45, 148, 5, 53, 16, 40, 129, 48, 22, 62, 65, 165, 215, 103, 151, 70, 83, 109, 33, 125, 131, 234, 123, 206, 203, 116, 56, 42, 214, 3, 89, 96, 94, 156, 15, 195, 196, 51, 189, 197, 146, 108, 179, 160, 132, 91, 64, 170, 243, 75, 35, 55, 173, 10, 240, 191, 120, 92, 113, 118, 171, 144, 149, 28, 80, 200, 236, 238, 126, 76, 172, 181, 139, 230, 167, 227, 182, 211, 21, 136, 134, 137, 222, 13, 38, 12, 184, 104, 241, 77, 93, 174, 32, 41, 79, 176, 218, 210, 175, 188, 199, 57, 229, 168, 231, 2, 150, 36, 159, 47, 247, 85, 166, 248, 145, 228, 25, 74, 67, 158, 157, 212, 115, 162, 187, 135, 98, 154, 255, 0, 97, 14, 161, 71, 194, 119, 9, 190, 88, 249, 169, 27, 225, 153, 163, 110, 217, 78, 164, 237, 140, 8, 253, 58, 63, 43, 54, 235, 226, 111, 186, 102, 101, 69, 7, 250, 254, 34, 23, 246, 127, 251, 252, 4, 17, 117, 224, 223, 18, 112, 193, 244, 192, 6, 242, 49, 202, 208, 86, 20, 133, 128, 90, 178, 50 };
    for (int i = 0; i < 1024; i++){ // In case of too much computation, change the loop here to a value like 4 or 8
        n = modular_exponentiation(i + 2, n, PRIME);
        uint8_t* bytes = (uint8_t*) &n;
        for (int j = 0; j < sizeof(uint32_t); j++){
            bytes[j] = random_permutation[bytes[j]];
        }
    }
    return n;
}

vector fourth_check(uint32_t n){  // This will be the equivalent of finding a solution to a 4x4 linear system
    uint32_t a = n & 0xFF;
    uint32_t b = (n >> 8) & 0xFF;
    uint32_t c = (n >> 16) & 0xFF;
    uint32_t d = (n >> 24) & 0xFF;

    uint32_t v1 = 'J' * a + 'o' * b + 'j' * c + 'o' * d;
    uint32_t v2 = 'D' * a + 'I' * b + 'O' * c + '!' * d;
    uint32_t v3 = 'Z' * a + 'A' * b + 'W' * c + 'A' * d;
    uint32_t v4 = 'R' * a + 'U' * b + 'D' * c + 'O' * d;
    return (vector) {.numbers = {v1, v2, v3, v4}};
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
    printf("First check: 0x%08X\n", first_check(to_compute.numbers[0]));
    printf("Second check: 0x%08X\n", second_check(to_compute.numbers[1]));
    #endif

}