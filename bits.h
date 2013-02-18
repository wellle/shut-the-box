#ifndef BITS_H
#define BITS_H

typedef struct Bits Bits;

Bits * createBits(int numTiles);
void destroyBits(Bits *bits);

void bitsPrintHeader(Bits *bits);
void bitsPrint(Bits *bits);
void bitsSet(Bits *bits, int i);

int bitsTest(Bits *bits, int i);    // 0 or 1
int bitsShut(Bits *bits, int i);    // result after shutting

int bitsSum(Bits *bits);    // 00011 -> 3
int bitsScore(Bits *bits);  // 00011 -> 12

int bitsMax(Bits *bits);    // 00000 -> 32

void printBits(int bits, int length);
int getBit(int bits, int i);

#endif // BITS_H
