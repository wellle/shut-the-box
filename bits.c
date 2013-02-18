#include <stdio.h>
#include <stdlib.h>

#include "bits.h"

struct Bits
{
    int length;
    int bits;
};

Bits * createBits(int length)
{
    Bits *bits;
    bits = (Bits*)malloc(sizeof(*bits));
    if (bits != NULL)
    {
        bits->length = length;
        bits->bits = 0;
        /* bits->bits = (1 << bits->length) - 1;   // 111111111 */
    }
    return bits;
}

void destroyBits(Bits *bits)
{
    free(bits);
}

void bitsPrintHeader(Bits *bits)
{
    for (int i = 0; i < bits->length; i++)
    {
        printf("%d", i+1);
    }
}

void bitsPrint(Bits *bits)
{
    printBits(bits->bits, bits->length);
}

void bitsSet(Bits *bits, int i)
{
    bits->bits = i;
}

int bitsTest(Bits *bits, int i)
{
    return !(~bits->bits & i);
}

int bitsShut(Bits *bits, int i)
{
    if (~bits->bits & i)
    {
        return 0;
    }

    bits->bits -= i;
    return 1;
}

// sum(00011) = 12 (append set digits)
int bitsSum(Bits *bits)
{
    int sum = 0;
    for (int i = 0; i < bits->length; i++)
    {
        if (getBit(bits->bits, i))
        {
            sum += i + 1;
        }
    }
    return sum;
}

// sum(00012) = 3 (add set digits)
int bitsScore(Bits *bits)
{
    int score = 0;
    for (int i = 0; i < bits->length; i++)
    {
        if (getBit(bits->bits, i))
        {
            score = 10 * score + i + 1;
        }
    }
    return score;
}

int bitsMax(Bits *bits)
{
    return 1 << bits->length;
}

int getBit(int bits, int i)
{
    int bit = (bits >> i) & 1;
    return bit;
}

void printBits(int bits, int length)
{
    for (int i = 0; i < length; i++)
    {
        int bit = getBit(bits, i);
        char character = bit ? 'X' : '.';
        printf("%c", character);
    }
}

