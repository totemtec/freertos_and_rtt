
#include <stdio.h>
#include "my_typedef.h"

float max;
int mi, mj;
void maxSum_1(float a[], int n)
{
	for(int i = 0;i < n; i++)
	{
        //ee_printf("t4 x%d\n",i);

		for (int j = i; j < n; j++)
		{
			float sum = 0;
			for (int k = i; k <= j; k++)
				sum += a[k];
 
			if (sum > max)
			{
				max = sum;
				mi = i;
				mj = j;
			}
		}
		//ee_printf_a("max %d\n",(int)max);
	}
}

int task_maxs(void) {
 
	float a[9] = { -1.01, 2.3, -3.7, 2.5, 6.34, -3.9, 4.6, 8.32, -6.987 };
	
	max = a[0];
	mi = mj = 0;
 
	maxSum_1(a, 9);
	
	//ee_printf("t4 [%d,%d]=%d\n",mi,mj,(int)max);

	if( (int)max == 17 ) {
		return 0;
	}
	else {
		return 1;
	}
}


