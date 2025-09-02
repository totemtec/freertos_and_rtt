
//https://blog.csdn.net/m0_70358541/article/details/130409214

#include "my_typedef.h"
#include <stdio.h>
#include "utils.h"

void swap(float *a, float *b);


#define N 6
float sarr[N] = { 9.8, 5.4, 8.2, 1.3 ,4.7, 2.3 };
void BubSort_test();

static uint32 ck = 0;

void swap(float *a, float *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int BubSort_main() {
    
	ck = 0;
    for (int i = 0; i < N; i++) {
        
        //ee_printf("t2 %d %d %d %d %d %d\n", (int)sarr[0],(int)sarr[1],(int)sarr[2],(int)sarr[3],(int)sarr[4],(int)sarr[5]);

        for (int j = 0; j + 1 < N - i; j++) {
            if (sarr[j] > sarr[j + 1]) {
                swap(&sarr[j], &sarr[j + 1]);
            }
        }

        //ee_printf("111\n");
    }
    for( int i = 0; i < N; ++i ) {
        ck = check_iter( ck, (int)sarr[i] );
        //ee_printf("222\n");
    }
    //ee_printf("t2 ck%d\n", ck);

    if( ck == 282 ) {
        return 0;
    }
    else {
        return 1;
    }
}



