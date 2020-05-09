import numpy as np

"""
  uint32_t a = n & 0xFF;
  uint32_t b = (n >> 8) & 0xFF;
  uint32_t c = (n >> 16) & 0xFF;
  uint32_t d = (n >> 24) & 0xFF;

  uint32_t v1 = 'J' * a + 'o' * b + 'j' * c + 'o' * d;
  uint32_t v2 = 'D' * a + 'I' * b + 'O' * c + '!' * d;
  uint32_t v3 = 'Z' * a + 'A' * b + 'W' * c + 'A' * d;
  uint32_t v4 = 'R' * a + 'U' * b + 'D' * c + 'O' * d;

  uint32_t vals[4] = {61319, 38111, 48787, 48939};
"""


def char_to_str(chrs):
    return [ord(c) for c in chrs]


incognite = np.array(
    [char_to_str("Jojo"), char_to_str("DIO!"), char_to_str("ZAWA"), char_to_str("RUDO")]
)
valori = np.array([61319, 38111, 48787, 48939])
risultato = np.linalg.solve(incognite, valori)

print("Values are: {}".format(risultato))


binary = ["{0:08b}".format(int(b)) for b in risultato]
binary.reverse()
print(int("".join(binary), 2))
