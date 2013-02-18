#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "bits.h"

// state    bits        penalties   ranks   sum
// 0        000000000   0           0       0
// 1        000000001   1           1       1
// 2        000000010   2           2       2
// 3        000000011   12          10      3
// 4        000000100   3           3       3
// 5        000000101   13          11      4
// 6        000000110   23          ??      5
// 7        000000111   123         ??      6
// 8        000001000   4           4       4
// 9        000001001   14          12      5
// 10       000001010   24          ??      6
// 16       000010000   5           5       5
// 32       000100000   6           6       6
// 64       001000000   7           7       7
// 128      010000000   8           8       8
// 256      100000000   9           9       9
// 384      110000000   89          ??      17
// 448      111000000   789         ??      24
// 511      111111111   123456789   511     45

int min(int i, int j);
int * createSortPerm(int *array, int length, int *ranks, int *uniqs);

int main()
{
    int numTiles = 9;

    Bits *bits = createBits(numTiles);
    int length = bitsMax(bits); // 512
    int max = numTiles * (numTiles + 1) / 2;    // 45

    int *pens = (int*)malloc(sizeof(int) * length);             // 00011 -> 12  (penalties, appended)
    int *sums = (int*)malloc(sizeof(int) * length);             // 00012 -> 3   (added)
    float *scores = (float*)malloc(sizeof(float) * length);     // expected penalty using perfect strategy

    int numMoves = 13;
    float b = 36;
    float probs[] = {0, 0, 1/b, 2/b, 3/b, 4/b, 5/b, 6/b, 5/b, 4/b, 3/b, 2/b, 1/b};

    for (int i = 0; i < length; i++)
    {
        bitsSet(bits, i);
        pens[i] = bitsScore(bits);
        sums[i] = bitsSum(bits);
    }

    int *sumUniq = (int*)malloc(sizeof(int) * max);             // sumPerm[sumUniq[i]] first move worth i
    int *sumPerm = createSortPerm(sums, length, 0, sumUniq);    // sums[sumPerm[i]] is sorted
    int *penNorm = (int*)malloc(sizeof(int) * length);          // normalized penalties
    int *penPerm = createSortPerm(pens, length, penNorm, 0);    // pens[penPerm[i]] is sorted

    for (int s = 0; s < length; s++)
    {
        int state = penPerm[s]; // in order of increasing penalty
        bitsSet(bits, state);

        float mean = 0;

        for (int n = 0; n < numMoves; n++)
        {
            if (probs[n] == 0)
            {
                continue;
            }

            float minScore = penNorm[state];
            int minMove = 0;

            int minInd = sumUniq[n];
            int maxInd = sumUniq[n+1];
            for (int m = minInd; m < maxInd; m++)
            {
                int move = sumPerm[m];
                int valid = bitsTest(bits, move);
                if (valid)
                {
                    int resultState = state - move;
                    float score = scores[resultState];
                    if (minScore > score)
                    {
                        minScore = score;
                        minMove = move;
                    }

                    // all moves
                    printf(" \t");
                    printBits(state, numTiles);
                    printf(" ");
                    printBits(move, numTiles);
                    printf(" ");
                    printBits(resultState, numTiles);
                    printf(" %f\n", score);

                }
            }

            mean += minScore * probs[n];

            // best move
            printf(" %d\t", n);
            printBits(state, numTiles);
            printf(" ");
            printBits(minMove, numTiles);
            printf(" ");
            printBits(state - sumPerm[minMove], numTiles);
            printf(" %f\n", minScore);

        }

        scores[state] = mean;

        // score of state
        printBits(state, numTiles);
        printf(" %f\n", mean);

    }

    for (int i = 0; i < length; i++)
    {
        printBits(i, numTiles);
        printf(" %f\n", scores[i]);
    }

    free(pens);
    free(sums);
    free(sumUniq);
    free(sumPerm);
    free(penNorm);
    free(penPerm);
}

int min(int i, int j)
{
    return i < j ? i : j;
}

int * createSortPerm(int *array, int length, int *ranks, int *uniq)
{
    int *perm = (int*)malloc(sizeof(int) * length);

    int min, value, j, i;
    int last = -1;
    for (i = 0; i < length;)
    {
        min = INT_MAX;
        for (j = 0; j < length; j++)
        {
            value = array[j];
            if (min > value && value > last)
            {
                min = value;
            }
        }

        if (uniq)
        {
            uniq[min] = i;
        }

        for (j = 0; j < length; j++)
        {
            value = array[j];
            if (value == min)
            {
                perm[i] = j;
                if (ranks)
                {
                    ranks[j] = i;
                }
                i++;
            }
        }
        last = min;
    }

    if (uniq)
    {
        uniq[value+1] = length;
    }

    return perm;
}
