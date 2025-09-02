#include <stdint.h>

#include "../include/uart.h"
#include "../include/utils.h"

#include "my_typedef.h"





#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "croutine.h"


#define test_time 10
void mytest(int n){
	int tick=0;
	for(int i=0;i<n;i++){
		for(int j=0;j<1000;j++){
			tick+=1;
		}
	}
}




QueueHandle_t Count_Handler;
#define MaxCount 2
#define InitialCount   2
#define Count_WAIT_MAX 1

QueueHandle_t RecursiveMutex_Handler;




// Task8 (CountingSemaphore)
int abc8;
#define TASK8_TASK_PRIO     8
#define TASK8_STK_SIZE      256
TaskHandle_t Task8Task_Handler;

void task8_task(void *pvParameters)
{
    abc8=0;
	
	BaseType_t err;

	

	while(1) {
		abc8++;

        while((xSemaphoreTake(Count_Handler,Count_WAIT_MAX))==pdFALSE){
            ee_printf("task8 Count take failed!\n");
        }
        ee_printf("task8 Count taken!\n");
        mytest(test_time);
        xSemaphoreGive(Count_Handler);
        ee_printf("task8 Count given!\n");
        vTaskDelay(1);

		//taskYIELD();
		//vTaskDelay(10);
	}
}




// Task9 (CountingSemaphore)
int abc9;
#define TASK9_TASK_PRIO     8
#define TASK9_STK_SIZE      256
TaskHandle_t Task9Task_Handler;

void task9_task(void *pvParameters)
{
    abc9=0;
	
	BaseType_t err;

	

	while(1) {
		abc9++;

        while((xSemaphoreTake(Count_Handler,Count_WAIT_MAX))==pdFALSE){
            ee_printf("task9 Count take failed!\n");
        }
        ee_printf("task9 Count taken!\n");
        mytest(test_time);
        xSemaphoreGive(Count_Handler);
        ee_printf("task9 Count given!\n");
        vTaskDelay(1);

		//taskYIELD();
		//vTaskDelay(10);
	}
}


// Task10 (CountingSemaphore)
int abc10;
#define TASK10_TASK_PRIO     8
#define TASK10_STK_SIZE      256
TaskHandle_t Task10Task_Handler;

void task10_task(void *pvParameters)
{
    abc10=0;
	
	BaseType_t err;

	

	while(1) {
		abc10++;

        while((xSemaphoreTake(Count_Handler,Count_WAIT_MAX))==pdFALSE){
            ee_printf("task10 Count take failed!\n");
        }
        ee_printf("task10 Count taken!\n");
        mytest(test_time);
        xSemaphoreGive(Count_Handler);
        ee_printf("task10 Count given!\n");
        vTaskDelay(1);

		//taskYIELD();
		//vTaskDelay(10);
	}
}




// Task11 (CountingSemaphore)
int abc11;
#define TASK11_TASK_PRIO     8
#define TASK11_STK_SIZE      256
TaskHandle_t Task11Task_Handler;

void task11_task(void *pvParameters)
{
    abc11=0;
	
	BaseType_t err;

	

	while(1) {
		abc11++;

        while((xSemaphoreTake(Count_Handler,Count_WAIT_MAX))==pdFALSE){
            ee_printf("task11 Count take failed!\n");
        }
        ee_printf("task11 Count taken!\n");
        mytest(test_time);
        xSemaphoreGive(Count_Handler);
        ee_printf("task11 Count given!\n");
        vTaskDelay(1);

		//taskYIELD();
		//vTaskDelay(10);
	}
}






/*RecursiveMutex*/


#define Recursive_WAIT_MAX 3

void Recursive_test(int task_num,int n){
    if(n==0)return;
   

    while((xSemaphoreTakeRecursive(RecursiveMutex_Handler,Recursive_WAIT_MAX))==pdFALSE){
        ee_printf("%*stask%d Recursive_%d take failed!\n",2*(3-n)+40*(task_num-12),"",task_num,n);
    }
    ee_printf("%*stask%d Recursive_%d taken!\n",2*(3-n)+40*(task_num-12),"",task_num,n);


            mytest(test_time);//do something

            Recursive_test(task_num,n-1);


    xSemaphoreGiveRecursive(RecursiveMutex_Handler);
    ee_printf("%*stask%d Recursive_%d given!\n",2*(3-n)+40*(task_num-12),"",task_num,n);

    return;

}


// Task12 (RecursiveMutex)
int abc12;
#define TASK12_TASK_PRIO     7
#define TASK12_STK_SIZE      256
TaskHandle_t Task12Task_Handler;

void task12_task(void *pvParameters)
{
    abc12=0;
	
	BaseType_t err;

	

	while(1) {
		abc12++;

        Recursive_test(12,3);
        vTaskDelay(1);

		//taskYIELD();
		//vTaskDelay(10);
	}


    // while(1){
    //     while((xSemaphoreTakeRecursive(RecursiveMutex_Handler,Recursive_WAIT_MAX))==pdFALSE){
    //         ee_printf("task%d Recursive_%d take failed!\n",12,2);
    //     }
    //     ee_printf("task%d Recursive_%d taken!\n",12,2);

    //     mytest(10);

    //             while((xSemaphoreTakeRecursive(RecursiveMutex_Handler,Recursive_WAIT_MAX))==pdFALSE){
    //                 ee_printf("task%d Recursive_%d take failed!\n",12,1);
    //             }
    //             ee_printf("task%d Recursive_%d taken!\n",12,1);

    //             mytest(10);

                        



    //             xSemaphoreGiveRecursive(RecursiveMutex_Handler);
    //             ee_printf("task%d Recursive_%d given!\n",12,1);        



    //     xSemaphoreGiveRecursive(RecursiveMutex_Handler);
    //     ee_printf("task%d Recursive_%d given!\n",12,2); 

    //     vTaskDelay(1);

    // }
}



// Task13 (RecursiveMutex)
int abc13;
#define TASK13_TASK_PRIO     7
#define TASK13_STK_SIZE      256
TaskHandle_t Task13Task_Handler;

void task13_task(void *pvParameters)
{
    abc13=0;
	
	BaseType_t err;

	

	while(1) {
		abc13++;

        Recursive_test(13,2);
        vTaskDelay(1);

		//taskYIELD();
		//vTaskDelay(10);
	}
}





/*Binary_Semaphore*/

QueueHandle_t Binary_Handler;
#define Binary_WaitMax  20

// Task14 (Binary_Semaphore)
int abc14;
#define TASK14_TASK_PRIO     7
#define TASK14_STK_SIZE      256
TaskHandle_t Task14Task_Handler;

void task14_task(void *pvParameters)
{
    abc14=0;
	
	BaseType_t err;

	

	while(1) {
		abc14++;

        while((xSemaphoreGive(Binary_Handler))==pdFALSE);
            ee_printf("task14 running\n");
            vTaskDelay(1);
        

		//taskYIELD();
		//vTaskDelay(10);
	}
}




// Task15 (Binary_Semaphore)
int abc15;
#define TASK15_TASK_PRIO     7
#define TASK15_STK_SIZE      256
TaskHandle_t Task15Task_Handler;

void task15_task(void *pvParameters)
{
    abc15=0;
	
	BaseType_t err;

	

	while(1) {
		abc15++;

        xSemaphoreTake(Binary_Handler,Binary_WaitMax);
            ee_printf("task15 running\n");
            vTaskDelay(2);
        

		//taskYIELD();
		//vTaskDelay(10);
	}
}





extern bool ee_open;

int main_Semaphore(){


    ee_open = true;
    ee_printf("run in main_Semaphore\n");

    // /*Counting*/
    // xTaskCreate((TaskFunction_t )task8_task,
    //                 (const char*    )"task8",
    //                 (uint16_t       )TASK8_STK_SIZE,
    //                 (void*          )NULL,
    //                 (UBaseType_t    )TASK8_TASK_PRIO,
    //                 (TaskHandle_t*  )&Task8Task_Handler);

    // xTaskCreate((TaskFunction_t )task9_task,
    //                 (const char*    )"task9",
    //                 (uint16_t       )TASK9_STK_SIZE,
    //                 (void*          )NULL,
    //                 (UBaseType_t    )TASK9_TASK_PRIO,
    //                 (TaskHandle_t*  )&Task9Task_Handler);

    // xTaskCreate((TaskFunction_t )task10_task,
    //                 (const char*    )"task10",
    //                 (uint16_t       )TASK10_STK_SIZE,
    //                 (void*          )NULL,
    //                 (UBaseType_t    )TASK10_TASK_PRIO,
    //                 (TaskHandle_t*  )&Task10Task_Handler);

    // xTaskCreate((TaskFunction_t )task11_task,
    //                 (const char*    )"task11",
    //                 (uint16_t       )TASK11_STK_SIZE,
    //                 (void*          )NULL,
    //                 (UBaseType_t    )TASK11_TASK_PRIO,
    //                 (TaskHandle_t*  )&Task11Task_Handler);


    // /*RecursiveMutex*/
    // xTaskCreate((TaskFunction_t )task12_task,
    //                 (const char*    )"task12",
    //                 (uint16_t       )TASK12_STK_SIZE,
    //                 (void*          )NULL,
    //                 (UBaseType_t    )TASK12_TASK_PRIO,
    //                 (TaskHandle_t*  )&Task12Task_Handler);

    // xTaskCreate((TaskFunction_t )task13_task,
    //                 (const char*    )"task13",
    //                 (uint16_t       )TASK13_STK_SIZE,
    //                 (void*          )NULL,
    //                 (UBaseType_t    )TASK13_TASK_PRIO,
    //                 (TaskHandle_t*  )&Task13Task_Handler);



    /*BinaryMutex*/
    xTaskCreate((TaskFunction_t )task14_task,
                    (const char*    )"task14",
                    (uint16_t       )TASK14_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK14_TASK_PRIO,
                    (TaskHandle_t*  )&Task14Task_Handler);

    xTaskCreate((TaskFunction_t )task15_task,
                    (const char*    )"task15",
                    (uint16_t       )TASK15_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK15_TASK_PRIO,
                    (TaskHandle_t*  )&Task15Task_Handler);




    Count_Handler = xQueueCreateCountingSemaphore( MaxCount , InitialCount );
	if (Count_Handler == NULL)ee_printf("Creat CountingSemaphore failed!!!\r\n");
	else ee_printf("CountingSemaphore Created\n");

    RecursiveMutex_Handler = xSemaphoreCreateRecursiveMutex();
    if (RecursiveMutex_Handler == NULL)ee_printf("Creat RecursiveMutex failed!!!\r\n");
	else ee_printf("RecursiveMutex Created\n");

    Binary_Handler = xSemaphoreCreateBinary();	
    if (Binary_Handler == NULL)ee_printf("Creat BinaryMutex failed!!!\r\n");
	else ee_printf("BinaryMutex Created\n");

    ee_printf("free start\n" );

    vTaskStartScheduler();

    

    while(1)
    {
        ee_printf("shouldn't run at here!!\n");
    }
}
