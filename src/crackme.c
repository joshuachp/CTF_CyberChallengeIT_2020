#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_AMOUNT 4
#define PRIME (((uint32_t)1 << 31) - 1)

volatile char hint[] =
    "You should really find where functions go dynamically....\n If you are "
    "really persistent you should try to void* that god damn't table.";
volatile uint32_t table_index;

typedef union {
  uint8_t bytes[4];
  uint32_t number;
} rng_helper;

uint32_t rand_gen(uint32_t *state) {
  rng_helper *s = (rng_helper *)state;
  const uint8_t random_permutation[256] = {
      121, 54,  58,  207, 95,  198, 231, 228, 218, 254, 203, 88,  200, 28,  50,
      190, 147, 110, 237, 4,   184, 59,  197, 116, 139, 178, 68,  112, 57,  12,
      115, 42,  153, 107, 210, 236, 124, 181, 84,  192, 143, 179, 113, 90,  150,
      162, 219, 86,  37,  99,  187, 145, 16,  255, 29,  33,  130, 204, 227, 97,
      62,  217, 149, 31,  40,  34,  15,  41,  208, 156, 64,  35,  146, 7,   154,
      209, 10,  232, 71,  137, 133, 225, 205, 148, 164, 132, 224, 51,  79,  101,
      65,  131, 92,  17,  189, 195, 89,  136, 125, 253, 47,  213, 46,  81,  222,
      70,  21,  158, 85,  22,  100, 60,  3,   117, 93,  123, 151, 170, 8,   48,
      199, 241, 134, 230, 239, 216, 91,  56,  36,  221, 196, 18,  24,  83,  43,
      32,  171, 163, 191, 11,  180, 248, 26,  73,  250, 211, 104, 183, 98,  96,
      201, 144, 235, 249, 166, 27,  72,  206, 193, 44,  127, 160, 108, 103, 129,
      9,   251, 243, 155, 55,  159, 39,  168, 240, 105, 78,  194, 161, 75,  202,
      173, 13,  238, 185, 45,  74,  244, 118, 152, 82,  14,  229, 169, 30,  87,
      141, 226, 102, 128, 6,   135, 233, 52,  177, 25,  1,   174, 119, 49,  223,
      247, 19,  106, 157, 67,  61,  212, 2,   246, 188, 122, 172, 175, 69,  80,
      120, 167, 20,  252, 109, 94,  23,  77,  38,  234, 214, 126, 220, 245, 176,
      182, 138, 114, 111, 215, 53,  0,   5,   186, 76,  66,  63,  142, 140, 242,
      165};
  for (size_t i = 0; i < 1024; i++) {
    s->number ^= (s->number << 7);
    s->number ^= 0xf0131b13;
    for (size_t j = 0; j < 4; j++)
      s->bytes[j] = random_permutation[(uint32_t)s->bytes[j]];
  }
  rng_helper n = (*s);
  for (size_t i = 0; i < 1024; i++) {
    n.number += 0x3df920b9;
    n.number ^= n.number << 3;
    for (size_t j = 0; j < 4; j++)
      n.bytes[j] = random_permutation[(uint32_t)n.bytes[j]];
  }
  return n.number;
}

void process_data(const uint8_t *data, size_t len, uint32_t key) {
  for (size_t i = 0; i < len; i++)
    printf("%c", data[i] ^ (uint8_t)rand_gen(&key));
  printf("\n");
}

const uint8_t secret[] = {
    0xB4, 0x44, 0xA0, 0xB4, 0x31, 0xF0, 0x13, 0x0C, 0x76, 0xC8, 0xE0, 0x62,
    0xF7, 0x98, 0xC5, 0x29, 0x1B, 0x33, 0xFA, 0x94, 0x4A, 0x79, 0x3D, 0xD8,
    0x2E, 0xA9, 0xCB, 0x80, 0xFF, 0xF2, 0x0E, 0xE6, 0x00, 0x07, 0xB1, 0x6E,
    0x25, 0x43, 0x87, 0xD7, 0x6A, 0x22, 0xAD, 0x99, 0x28, 0xB4, 0xB7, 0x28,
    0x94, 0xCF, 0xD4, 0x37, 0x87, 0xBE, 0xED, 0xE5, 0xDE, 0x9A, 0xA1, 0x1B,
    0xA7, 0xF6, 0xD2, 0x6F, 0x15, 0x45, 0xEB, 0x26, 0x4E, 0x63, 0xFD, 0x50,
    0x72, 0x9E, 0x43, 0x97, 0xDC, 0x11, 0x71, 0x0E, 0xC9, 0x8F, 0x42, 0x17,
    0x4D, 0x29, 0x01, 0x01, 0x92, 0x53, 0x56, 0xBF, 0x28, 0x81, 0x92, 0xE3,
    0x9F, 0x2F, 0xB4, 0xD9, 0xE9, 0x6B, 0x5C, 0x85, 0xF7, 0x28, 0xBB, 0xDB,
    0xD2, 0x7F, 0xFC, 0x9A, 0x53, 0xD7, 0x0A, 0xED, 0x1B, 0xC3, 0xBE, 0x49,
    0xD5, 0xD3, 0xA9, 0x59, 0xDA, 0x77, 0xDC, 0x83, 0x39};

uint32_t modular_exponentiation(uint32_t x, uint32_t y, uint32_t p);
uint32_t first_check(uint32_t n);
uint32_t second_check(uint32_t n);
uint32_t third_check(uint32_t n);
uint32_t *fourth_check(uint32_t n);
uint32_t fifth_check(uint32_t n);
uint32_t sixth_check(uint32_t n);
uint32_t seventh_check(uint32_t n);

bool f(uint32_t n) {
  uint32_t k = first_check(n);
  return k == 0xaec4b368;
}
bool h(uint32_t n) {
  uint32_t k = second_check(n);
  return k == 0x191820f5;
}
bool i(uint32_t n) {
  uint32_t k = third_check(n);
  return k == 0x668568f4;
}
bool j(uint32_t n) { // 3113377990 Is the solution
  uint32_t *k = fourth_check(n);
  /*for (uint32_t i = 0; i < 4; i++){
      printf("%d - ", k.numbers[i]);
  }
  printf("\n");*/
  uint32_t vals[4] = {61319, 38111, 48787, 48939};
  for (uint32_t i = 0; i < 4; i++) {
    if (vals[i] != k[i]) {
      return false;
    }
  }
  return true;
}

bool k(uint32_t n) {
  uint32_t k = fifth_check(n);
  return k == 0xf10e0130;
}
bool l(uint32_t n) {
  uint32_t k = sixth_check(n);
  return k == 0x269b237e;
}
bool m(uint32_t n) {
  uint32_t k = seventh_check(n);
  return k == 0xc9a588bc;
}

uint32_t first_check(uint32_t n) {
  return modular_exponentiation(0xDEADBEEF, n, PRIME);
}

uint32_t second_check(uint32_t n) {
  // n = first_check(n);
  const uint8_t *text =
      (uint8_t
           *)"To be fair, you have to have a very high IQ to understand Rick "
             "and Morty. The humour is extremely subtle, and without a solid "
             "grasp of theoretical physics most of the jokes will go over a "
             "typical viewer's head. There's also Rick's nihilistic outlook, "
             "which is deftly woven into his uint8_tacterisation- his "
             "personal "
             "philosophy draws heavily from Narodnaya Volya literature, for "
             "instance. The fans understand this stuff; they have the "
             "intellectual capacity to truly appreciate the depths of "
             "these "
             "jokes, to realise that they're not just funny- they say "
             "something deep about LIFE. As a consequence people who dislike "
             "Rick & Morty truly ARE idiots- of course they wouldn't "
             "appreciate, for instance, the humour in Rick's existential "
             "catchphrase \"Wubba Lubba Dub Dub,\" which itself is a cryptic "
             "reference to Turgenev's Russian epic Fathers and Sons. I'm "
             "smirking right now just imagining one of those addlepated "
             "simpletons scratching their heads in confusion as Dan Harmon's "
             "genius wit unfolds itself on their television screens. What "
             "fools.. how I pity them. And yes, by the way, i DO have a Rick "
             "& Morty tattoo. And no, you cannot see it. It's for the ladies' "
             "eyes only- and even then they have to demonstrate that they're "
             "within 5 IQ points of my own (preferably lower) beforehand. "
             "Nothin personnel kid";
  uint8_t *bytes = (uint8_t *)&n;
  for (uint32_t i = 0; i < 1297; i++) {
    bytes[i % 4] ^= text[i];
  }
  return n;
}

uint32_t third_check(uint32_t n) { // Maybe too computation uint32_tensive, will
                                   // see whether a change is warranted
  // n = second_check(n);
  const uint8_t random_permutation[256] = {
      152, 209, 239, 114, 59,  44,  107, 142, 121, 201, 72,  185, 31,  204, 82,
      60,  213, 205, 155, 81,  207, 61,  147, 233, 66,  87,  138, 183, 11,  220,
      26,  122, 19,  198, 219, 24,  177, 106, 100, 99,  30,  143, 95,  141, 68,
      124, 221, 84,  29,  105, 216, 180, 46,  232, 39,  130, 1,   245, 37,  52,
      73,  45,  148, 5,   53,  16,  40,  129, 48,  22,  62,  65,  165, 215, 103,
      151, 70,  83,  109, 33,  125, 131, 234, 123, 206, 203, 116, 56,  42,  214,
      3,   89,  96,  94,  156, 15,  195, 196, 51,  189, 197, 146, 108, 179, 160,
      132, 91,  64,  170, 243, 75,  35,  55,  173, 10,  240, 191, 120, 92,  113,
      118, 171, 144, 149, 28,  80,  200, 236, 238, 126, 76,  172, 181, 139, 230,
      167, 227, 182, 211, 21,  136, 134, 137, 222, 13,  38,  12,  184, 104, 241,
      77,  93,  174, 32,  41,  79,  176, 218, 210, 175, 188, 199, 57,  229, 168,
      231, 2,   150, 36,  159, 47,  247, 85,  166, 248, 145, 228, 25,  74,  67,
      158, 157, 212, 115, 162, 187, 135, 98,  154, 255, 0,   97,  14,  161, 71,
      194, 119, 9,   190, 88,  249, 169, 27,  225, 153, 163, 110, 217, 78,  164,
      237, 140, 8,   253, 58,  63,  43,  54,  235, 226, 111, 186, 102, 101, 69,
      7,   250, 254, 34,  23,  246, 127, 251, 252, 4,   17,  117, 224, 223, 18,
      112, 193, 244, 192, 6,   242, 49,  202, 208, 86,  20,  133, 128, 90,  178,
      50};
  for (uint32_t i = 0; i < 1024;
       i++) { // In case of too much computation, change the loop here to a
              // value like 4 or 8
    n = modular_exponentiation(i + 2, n, PRIME);
    uint8_t *bytes = (uint8_t *)&n;
    for (uint32_t j = 0; j < sizeof(uint32_t); j++) {
      bytes[j] = random_permutation[(uint32_t)bytes[j]];
    }
  }
  return n;
}

uint32_t *fourth_check(uint32_t n) { // This will be the equivalent of finding a
                                     // solution to a 4x4 linear system
  uint32_t a = n & 0xFF;
  uint32_t b = (n >> 8) & 0xFF;
  uint32_t c = (n >> 16) & 0xFF;
  uint32_t d = (n >> 24) & 0xFF;

  uint32_t v1 = 'J' * a + 'o' * b + 'j' * c + 'o' * d;
  uint32_t v2 = 'D' * a + 'I' * b + 'O' * c + '!' * d;
  uint32_t v3 = 'Z' * a + 'A' * b + 'W' * c + 'A' * d;
  uint32_t v4 = 'R' * a + 'U' * b + 'D' * c + 'O' * d;

  uint32_t *res = (uint32_t *)malloc(sizeof(uint32_t) * INT_AMOUNT);
  res[0] = v1;
  res[1] = v2;
  res[2] = v3;
  res[3] = v4;

  return res;
}

uint32_t fifth_check(uint32_t n) { // Optional
                                   // Computes the collatz conjecture for n
  uint32_t counter = 0;
  while (n > 1) {
    counter++;
    if (n % 2 == 1) {
      n = 3 * n + 1;
    } else {
      n = n / 2;
    }
  }
  return counter;
}

uint32_t sixth_check(uint32_t n) { // Optional
                                   // Computes nth fibonacci mod PRIME
  uint32_t a = 1;
  uint32_t b = 1;
  uint32_t tmp = 1;
  for (uint32_t i = 0; i < n; i++) {
    tmp = b;
    b = (a + b) % PRIME;
    a = tmp;
  }
  return a;
}

uint32_t seventh_check(uint32_t n) { // Optional
  return n;                          // Todo
}

// Computes x ^ y mod p (where ^ is exponentiation)
// x and y can be anything != 0, p must be prime!
uint32_t modular_exponentiation(uint32_t x, uint32_t y, uint32_t p) {
  // 64 bit because we don't want to overflow it on squaring
  long counter = 1;
  long base = x;
  while (y > 0) {
    if (y % 2 == 1) {
      counter = (counter * base) % p;
    }
    base = (base * base) % p;
    y = y / 2;
  }
  return (uint32_t)counter;
}

bool (*table[7])(uint32_t) = {f, h, i, j, k, l, m};

int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("lol no\n");
    return 0;
  }
  uint32_t v = atoi(argv[1]);

// Activate me from the compiler
#ifdef IS_DEBUG
  uint8_t buf[1024] = {0};
  printf("Write the password to encrypt (max 1024 uint8_ts): ");
  fgets(buf, 1024, stdin);
  size_t len = strlen(buf);
  process_data((uint8_t *)buf, len, v);
#else
  if ("stuck" != hint) {
    // Returns 3
    table_index = modular_exponentiation(3, 43, 7);
  }
  // The solution is 3113377990
  if (table[table_index](v)) {
    puts(" Ayy, that's pretty good! (Don't try to trick me, you won't get the "
         "real deal)\n One last step.\n You'll get 128 bytes, the dump of a "
         "hex-digest (SHA2).\n It's dangerous to go alone. Take this clip if "
         "you're stuck it can help.\n I hope somebody was looking at that or "
         "not just use the force.");
    process_data(secret, 128, v);
  } else {
    printf("lol no x2\n");
  }
#endif
  return 0;
}
