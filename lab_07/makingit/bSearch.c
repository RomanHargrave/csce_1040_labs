#include "defs.h"
#include "sortAndSearch.h"

int binarySearch(DBrecord *A[], DBrecord *key, Field member, int low, int high)
{
   int mid;
   int result;

   if( low > high ) return low-1;  // failure

   mid = (high + low) / 2;
   
   result = compare(A[mid], key, member);

   if( result == 0 ) return mid;
   else if( result > 0 ) return binarySearch(A,key,member,low,mid-1);
   else return binarySearch(A,key,member,mid+1,high); 
}
