
#include "my_typedef.h"

int Find(int arr[], int n, int key);
 int BinarySearch1(int arr[], int size, int key);

#define N1 12
int fa[N1] = { 1,3,5,9,12,23,34,65,73,89,94,99 };

int task_find(void)
{
    int n = Find( fa, N1, 34 );
    //ee_printf("t6 uf%d\n", n);

    int n1 = BinarySearch1( fa, N1, 73 );
    //ee_printf("t6 bf%d\n", n1);
    
    if( n == 6 && n1 == 8 ) {
        return 0;
    }
    else {
        return 1;
    }
}

int Find(int arr[], int n, int key)
{
    for(int i=0; i<n; ++i)
    {
     if(arr[i] == key)
       return i;
    }
    return -1;
}

 int BinarySearch1(int arr[], int size, int key) 
{
    int low = 0;
    int high = size-1;
    int mid;
    while(low <= high)
    {
      mid = low+((high-low)>>1);
      if(key < arr[mid])
          high = mid-1;
      else if(key > arr[mid])
          low = mid+1;
      else
          return mid;
        }
    return -1;
}


