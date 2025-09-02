
#include "my_typedef.h"

#include "uart.h"
#define F_N 16
float data[F_N];
 
int middleFilter(float in_data)
{
    int sum = 0;
    float temp[F_N];
    float change;
    int i,j;

    for(i=0; i<F_N-1; i++)
    {
        data[i]=data[i+1];
    }
    data[F_N-1] = in_data;

    for(i=0; i<F_N; i++) {
        temp[i] = data[i];
        //ee_printf("%d,", (int)temp[i]);
    }
    for(i=1; i<F_N; i++)

        for(j=0; j<F_N-i; j++)
        {
            if(temp[j] > temp[j+1])
            {
                change = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = change;
            }
        }

    for(i=1; i<F_N-1; i++)
        sum = sum + temp[i];
    return(sum/(F_N-2));
}

#include "rtthread.h"
extern struct rt_timer timer1;

int task_filter(void)
{
    int sum = 0;

    for( int i = 0; i < 16; ++i ) {
        float ff = middleFilter( 123.4 );
        ee_printf( "t5 A%d\n", (int)ff );
        sum += ff;
    }
    
    if( (int)sum == 1039 ) {
        return 0;
    }
    else {
        return 1;
    }
}

