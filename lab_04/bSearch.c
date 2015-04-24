int binarySearch(int A[], int key, int low, int high)
{
   int mid;
   int result;

   if( low > high ) return -1;  // failure

   mid = (high + low) / 2;
   
   if( A[mid] == key ) return mid;
   else if( A[mid] > key ) return binarySearch(A,key,low,mid-1);
   else return binarySearch(A,key,mid+1,high); 
}
