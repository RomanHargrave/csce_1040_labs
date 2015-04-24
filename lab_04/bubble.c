#include <stdio.h>

int A[50000];

int main()
{
   int i, j, temp;

   for(i = 0; i < 50000; i++)
       A[i] = random() % 1000000; 
   
   for(i = 0; i < 50000; i++)
       for(j = 0; j < 50000-i-1; j++)
       {
           if( A[j] > A[j+1] )
           {
               temp = A[j];
               A[j] = A[j+1];
               A[j+1] = temp;
           }
       }
   for(i = 0; i < 5000; i++)
   {
       for(j = 0; j < 10; j++)
           printf("%8d",A[i*10+j]);
       printf("\n");
   }

    return 0;
}
