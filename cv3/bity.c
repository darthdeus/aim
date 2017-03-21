#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* int t[] = { */
/* #define B1(x) x, !x, !x, x */
/* #define B2(x) B1(x), B1(!x), B1(!x), B1(x) */
/* #define B3(x) B2(x), B2(!x), B2(!x), B2(x) */
/* #define B4(x) B3(x), B3(!x), B3(!x), B3(x) */
/*   B4(0) */
/* } */
/*  */
/*  */
/* int t[] = { */
/* #define B1(x) x, !x, !x, x */
/* #define B2(x) B1(+x), B1(!x), B1(!x), B1(+x) */
/* #define B3(x) B2(+x), B2(!x), B2(!x), B2(+x) */
/* #define B4(x) B3(+x), B3(!x), B3(!x), B3(+x) */
/*   B4(0) */
/* } */


#define SBI(x, n) ((x) | (1 << n))
#define CBI(x, n) ((x) & ~(1 << n))

#define GET(x, n) (((x) & (1 << n)) >> n)

// **********************************

#define FORALL(GEN)  \
  GEN(u8, uint8_t)   \
  GEN(u16, uint16_t) \
  GEN(u32, uint32_t) \
  GEN(u64, uint64_t)

#define GENERIC_FORALL(prefix, x) \
  _Generic(x,  \
      uint8_t: prefix##_u8,   \
      uint16_t: prefix##_u16, \
      uint32_t: prefix##_u32, \
      uint64_t: prefix##_u64)(x)

// **********************************

#define DEF_POPCNT(suffix, type) \
  int popcnt_##suffix(type x) {  \
    size_t n = sizeof(x);        \
    int cnt = 0;                 \
    while (n--) {                \
      cnt += GET(x, n);          \
    }                            \
                                 \
    return cnt;                  \
  }

// *********************************

#define popcnt(x) GENERIC_FORALL(popcnt, x)
FORALL(DEF_POPCNT);

// 1.
#define MOCNINA2(x) ({ int cnt = popcnt(x); cnt == 0 || cnt == 1; })

uint8_t tab[] = {
  0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};

// 2.
uint8_t zrcadleni(uint8_t x) {
  uint8_t res = 0;

  res |= tab[(x & 0x0F)] << 4;
  res |= tab[(x & 0xF0) >> 4];

  return res;
}

// 3.
#define P2(x)     (((x) & 0b01)   ^ (((x) & 0b10) >> 1))
#define P4(x)  P2 (((x) & 0b11)   ^ (((x) & 0b1100) >> 2))
#define P8(x)  P4 (((x) & 0xf)    ^ (((x) & 0xf0) >> 4))
#define P16(x) P8 (((x) & 0xff)   ^ (((x) & 0xff00) >> 8))
#define P32(x) P16(((x) & 0xffff) ^ (((x) & 0xffff0000) >> 16))

// nebo posunu o 1 doprava a vyxorim
// pak posunu o 2 doprava a vyxorim
// pak posunu o 4 doprava a vyxorim

/* uint32_t parita(uint32_t x) { */
/*   (x & 0xffff) ^ ((x & 0xffff0000) >> 16) */
/* } */

int main() {
  int x = SBI(8, 2);
  int y = CBI(6, 2);

  printf("%x, %x", 123, zrcadleni(123));

  for (uint32_t i = 0; i < 10; i++) {
    printf("%d, je mocnina 2: %d, parita: %d\n", i, MOCNINA2(i), P32(i));
  }

  printf("%d\n", popcnt(7u));
}
