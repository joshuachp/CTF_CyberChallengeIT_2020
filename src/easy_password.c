#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define INT_AMOUNT 4
#define PRIME (((uint32_t) 1 << 31) - 1)

typedef struct {
    uint32_t numbers[INT_AMOUNT];
} vector;

const uint8_t secret[] = {0x26,0xEC,0x97,0x59,0x1D,0xC3,0x8D,0xE4,0x77,0x69,0xB7,0xA0,0x6F,0x17,0x8F,0xD2,0x10,0x5E,0x37,0xF1,0xB7,0x6D,0x2B,0x84,0x9F,0x5D,0x04,0xCD,0xDA,0xBC,0x72,0x75,0xB5,0xA4,0x01,0x1A,0xD6,0xD7,0x4F,0x71,0x67,0xAE,0xA1,0x6F,0x2C,0xD2,0x92,0x11,0x06,0xCD,0xA6,0xED,0x7D,0x2C,0xEC,0xC4,0x56,0x1C,0x93,0xDD,0x46,0x73,0x39,0xB1,0xF3,0x6F,0x16,0xD8,0x9B,0x45,0x5E,0x37,0xF2,0xEF,0x37,0x7D,0xB8,0x96,0x0F,0x04,0xC3,0x8B,0xE3,0x2A,0x3C,0xB0,0xAA,0x56,0x44,0x8A,0x80,0x40,0x0D,0x66,0xAF,0xF5,0x6E,0x2B,0x86,0x98,0x0F,0x06,0xCA,0xF9,0xB4,0x78,0x20,0xBA,0xAE,0x54,0x1E,0x94,0xD5,0x4E,0x75,0x30,0xF7,0xA6,0x3E,0x41,0xD0,0xC8,0x10,0x09,0x9F,0xA1,0xBB,0x33};
void encrypt(const char* data, uint32_t key){
    uint32_t s = 1337;
    for(uint32_t i = 0; data[i] != 0; i++){
        uint8_t c = (s * key) & 0xFF;
        printf("0x%02X,", data[i] ^ c);
        s++;
    }
}

void decrypt(const uint8_t* data, uint32_t len, uint32_t key){
    uint32_t s = 1337;
    for(uint32_t i = 0; i < len; i++){
        uint8_t c = (s * key) & 0xFF;
        printf("%c", data[i] ^ c);
        s++;
    }
    printf("\n");
}

uint32_t modular_exponentiation(uint32_t x, uint32_t y, uint32_t p);
uint32_t first_check(uint32_t n);
uint32_t second_check(uint32_t n);
uint32_t third_check(uint32_t n);
vector fourth_check(uint32_t n);
uint32_t fifth_check(uint32_t n);
uint32_t sixth_check(uint32_t n);
uint32_t seventh_check(uint32_t n);

uint8_t f(uint32_t n){
    uint32_t k = first_check(n);
    return k == 0xaec4b368;
}
uint8_t h(uint32_t n){
    uint32_t k = second_check(n);
    return k == 0x191820f5;
}
uint8_t i(uint32_t n){
    uint32_t k = third_check(n);
    return k == 0x668568f4;
}
uint8_t j(uint32_t n){  // 3113377990 Is the solution
    vector k = fourth_check(n);
    /*for (int i = 0; i < 4; i++){
        printf("%d - ", k.numbers[i]);
    }
    printf("\n");*/
    uint32_t vals[4] = {61319, 38111, 48787, 48939};
    for (int i = 0; i < 4; i++){
        if (vals[i] != k.numbers[i]){
            return 0;
        }
    }
    return 1;
}

uint8_t k(uint32_t n){
    uint32_t k = fifth_check(n);
    return k == 0xf10e0130;
}
uint8_t l(uint32_t n){
    uint32_t k = sixth_check(n);
    return k == 0x269b237e;
}
uint8_t m(uint32_t n){
    uint32_t k = seventh_check(n);
    return k == 0xc9a588bc;
}


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
    for (uint32_t i = 0; i < 1024; i++){ // In case of too much computation, change the loop here to a value like 4 or 8
        n = modular_exponentiation(i + 2, n, PRIME);
        uint8_t* bytes = (uint8_t*) &n;
        for (uint32_t j = 0; j < sizeof(uint32_t); j++){
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

uint32_t fifth_check(uint32_t n){   // Optional
    // Computes the collatz conjecture for n
    uint32_t counter = 0;
    while (n > 1){
        counter++;
        if (n % 2 == 1) {
            n = 3*n + 1;
        }
        else{
            n = n / 2;
        }
    }
    return counter;
}

uint32_t sixth_check(uint32_t n){   // Optional
    // Computes nth fibonacci mod PRIME
    uint32_t a = 1;
    uint32_t b = 1;
    uint32_t tmp = 1;
    for (uint32_t i = 0; i < n; i++){
        tmp = b;
        b = (a + b) % PRIME;
        a = tmp;
    }
    return a;
}

uint32_t seventh_check(uint32_t n){ // Optional
    return n; // Todo
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

uint8_t (*table[7]) (uint32_t) = {f, h, i, j, k, l, m};

int main(int argc, char** argv){
    if (argc <= 1){
        printf("lol no\n");
        return 0;
    }
    uint32_t v = atoi(argv[1]);
    uint32_t counter = modular_exponentiation(3, 43, 7); // Returns 3

    if (table[0]){ // Should be patched away!
        counter = -1;
    }
    
    if (table[counter](v)){ // The solution is 3113377990
        printf("Ayy, that's pretty good! (Don't try to trick me, you won't get the real deal)\n");
        printf("One last step.\nHere's yours the password: (SHA2)\n");
        decrypt(secret, 128, v);
    }
    else{
        printf("lol no x2\n");
    }
    return 0;
}