#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <strings.h>

#define TABLESIZE 100000
#define SEARCHSIZE 10000000

int h1(int k)
{
    return k % TABLESIZE;
}

//int h2(int k)
//{
//    return 1;
//}

int search(int htable[TABLESIZE], int key)
{
    int index, orig;
    int inc;

    index   = h1(key);
    orig    = index;
    inc     = index + key + 5;

    while (1) {

        printf("\rExpect: %06li key %06li ; have %06li", orig, key, index);

        if ( htable[index] == key ) {
            return index;
        } else if (htable[index] == -1 ) {
            return index;
        } else {
            index = (index + inc) % TABLESIZE;
        }


    }
}


int main()
{
    int A[TABLESIZE];
    int i, key, index;

// read the data

    for(i = 0; i < TABLESIZE / 2; i++) {
        scanf("%d", &A[i]);
    }

// Now search for data repeatedly

    for(i = 0; i < SEARCHSIZE; i++)
    {
        key = random() % 500000;


        index = search(A,key);

        if( index != -1  )
        {
            assert(key = A[index]);
        }

        printf("\n[%03.03f%%] Done\n", ((float) i / (float) SEARCHSIZE) * 100);
    }

    return 0;
}
