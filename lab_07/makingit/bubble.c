#include "defs.h"

void swap(DBrecord *A[], int, int);

void bubbleSort(DBrecord *A[], int num, Field f)
{
    int i,j;
    for(i = 0; i < num-1; i++)
    {
        for(j = 0; j < num-i-1; j++)
        {
            if( isGreater(A[j],A[j+1],f) )
                swap (A,j,j+1);
        }
    }
}


Boolean isGreater(DBrecord *a, DBrecord *b, Field f)
{
    if( compare(a,b,f) > 0 ) 
        return True;
    return False;
}


void swap(DBrecord *A[], int i, int j)
{
    DBrecord *temp;
    temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

int compare(DBrecord *a, DBrecord *b, Field f)
{
    switch (f)
    {
        case Age:
            if( a->age > b->age ) 
                return 1;
            if( a->age < b->age )
                return -1;
            else
                return 0;

        case Year:
            if( a->year > b->year ) 
                return 1;
            if( a->year < b->year ) 
                return -1;
            else 
                return 0;

        case GPA:
            if( a->gpa > b->gpa ) 
                return 1;
            if( a->gpa < b->gpa ) 
                return -1;
            else 
                return 0;

        case GradYear:
            if( a->gradYear > b->gradYear ) 
                return 1;
            if( a->gradYear < b->gradYear ) 
                return -1;
            else 
                return 0;

        case LastName:
            return strcmp(a->lName, b->lName);

        case FirstName:
            return strcmp(a->fName, b->fName);

        case ID:
            return strcmp(a->idNum, b->idNum);

	default:
            printf("Unexpected field value %d\n",f);
            assert(0);

    }
}

