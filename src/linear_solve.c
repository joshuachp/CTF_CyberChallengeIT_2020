#include <stdio.h>

int main(void) {
  // Value is: [198.  96. 146. 185.]
  unsigned int a = 198, b = 96, c = 146, d = 185;
  unsigned int n = (a) | (b << 8) | (c << 16) | (d << 24);
  printf("Value %i\n", n);
}
