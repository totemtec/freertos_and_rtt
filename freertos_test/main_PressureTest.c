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

#include "task1_big.h"
#include "task2_sort.h"
#include "task3_path.h"
#include "task4_maxs.h"
#include "task5_filter.h"
#include "task6_find.h"
#include "task7_float_test.h"
#include "task_rvv/free_main.h"

int ok_num = 0;


// Task1   
#define TASK1_PRIO     8
#define TASK_STK_SIZE      1024
TaskHandle_t Task1_Handler;
int t1_err = 0;
void task1(void *pvParameters){
    t1_err += task_big();
    ee_printf("N1:%d\n", t1_err );
	ok_num++;


    vTaskSuspend(Task1_Handler);
    while( 1 ) {
        //taskYIELD();
    }
}




// Task2   
#define TASK2_PRIO     8
TaskHandle_t Task2_Handler;
int t2_err = 0;
void task2(void *pvParameters){

    //ee_printf("run in task2\n");

    t2_err += BubSort_main();
	t2_err += BubSort_main();
	t2_err += BubSort_main();
	t2_err += BubSort_main();
	t2_err += BubSort_main();
    ee_printf("F2:%d\n", t2_err );
	ok_num++;
    
    vTaskSuspend(Task2_Handler);
    while( 1 ) {
        
    }
}



// Task3   
#define TASK3_PRIO     8

TaskHandle_t Task3_Handler;
int t3_err = 0;
void task3(void *pvParameters){

    t3_err += task_search();
    ee_printf("N3:%d\n", t3_err );
	ok_num++;
    
    vTaskSuspend(Task3_Handler);
    // while( 1 ) {
    //     taskYIELD();
    // }
}



// Task4   
#define TASK4_PRIO     8

TaskHandle_t Task4_Handler;
int t4_err = 0;
void task4(void *pvParameters){

    t4_err += task_maxs();
	t4_err += task_maxs();
	t4_err += task_maxs();
	t4_err += task_maxs();
	t4_err += task_maxs();

    ee_printf("F4:%d\n", t4_err );
	ok_num++;
    
    vTaskSuspend(Task4_Handler);
    while( 1 ) {
        
    }
}



// Task5   
#define TASK5_PRIO     8

TaskHandle_t Task5_Handler;
int t5_err = 0;
void task5(void *pvParameters){

    t5_err += task_filter(); //not suport moulty task!!!
    ee_printf("F5:%d\n", t5_err );
	ok_num++;
    
    vTaskSuspend(Task5_Handler);
    while( 1 ) {
        
    }
}



// Task6   
#define TASK6_PRIO     8

TaskHandle_t Task6_Handler;
int t6_err = 0;
void task6(void *pvParameters){

    t6_err += task_find();
    ee_printf("N6:%d\n", t6_err );
	ok_num++;
    
    vTaskSuspend(Task6_Handler);
    while( 1 ) {
        
    }
}



// Task7   
#define TASK7_PRIO     8

TaskHandle_t Task7_Handler;
int t7_err = 0;
void task7(void *pvParameters){

    t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();
	t7_err += float_test_error();

    ee_printf("F7:%d\n", t7_err );
	ok_num++;
    
    vTaskSuspend(Task7_Handler);
    while( 1 ) {
        
    }
}





// Task8   
#define TASK8_PRIO     8
SemaphoreHandle_t Mutex_Handler1;
TaskHandle_t Task8_Handler;
int t8_err = 0;
void task8(void *pvParameters){
    
    Mutex_Handler1 = xSemaphoreCreateMutex();		
	if (Mutex_Handler1 == NULL)ee_printf("Mutex Created Failed!!!\r\n");
	//else ee_printf("Mutex Created!\n");	

    // while(1){

    // }

    for( int i = 0; i < 20; ++i ) {
		t8_err += free_main_add();
	}
    ee_printf("V8:%d\n", t8_err );

	ok_num++;
    
    vTaskSuspend(Task8_Handler);
    while( 1 ) {
        
    }
}



// Task9   
#define TASK9_PRIO     8

TaskHandle_t Task9_Handler;
int t9_err = 0;
void task9(void *pvParameters){

    for( int i = 0; i < 20; ++i ) {
		t8_err += free_main_mul();
	}
    ee_printf("V9:%d\n", t9_err );

	ok_num++;
    
    vTaskSuspend(Task9_Handler);
    while( 1 ) {
        
    }
}





// Task10 (check entire err)
#define TASK10_PRIO     3

TaskHandle_t Task10_Handler;

void task10(void *pvParameters){

   if(ok_num==9){
     if( t1_err == 0 && t2_err == 0 && t3_err == 0 && t4_err == 0 && t5_err == 0 && t6_err == 0 && t7_err == 0 && t8_err == 0 && t9_err == 0 ) {
        ee_printf("All Tasks are PASS!!!\n");

        test_pass();
     }
     else {
        ee_printf("Certein Task is FAIL!!!\n");

        test_fail();
     }
   }
   else ee_printf("Shit!\n");
    
    vTaskSuspend(Task10_Handler);
    while( 1 ) {
        
    }
}
















extern bool ee_open;

int main_PressureTest(){


    ee_open = true;
    ee_printf("run in main_PressureTest\n");

    


    
    xTaskCreate((TaskFunction_t )task1,
                    (const char*    )"task1",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK1_PRIO,
                    (TaskHandle_t*  )&Task1_Handler);

      
    xTaskCreate((TaskFunction_t )task2,
                    (const char*    )"task2",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK2_PRIO,
                    (TaskHandle_t*  )&Task2_Handler);

      
    xTaskCreate((TaskFunction_t )task3,
                    (const char*    )"task3",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK3_PRIO,
                    (TaskHandle_t*  )&Task3_Handler);

      
    xTaskCreate((TaskFunction_t )task4,
                    (const char*    )"task4",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK4_PRIO,
                    (TaskHandle_t*  )&Task4_Handler);


    xTaskCreate((TaskFunction_t )task5,
                    (const char*    )"task5",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK5_PRIO,
                    (TaskHandle_t*  )&Task5_Handler);

      
    xTaskCreate((TaskFunction_t )task6,
                    (const char*    )"task6",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK6_PRIO,
                    (TaskHandle_t*  )&Task6_Handler);


      
    xTaskCreate((TaskFunction_t )task7,
                    (const char*    )"task7",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK7_PRIO,
                    (TaskHandle_t*  )&Task7_Handler);


    xTaskCreate((TaskFunction_t )task8,
                    (const char*    )"task8",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK8_PRIO,
                    (TaskHandle_t*  )&Task8_Handler);

      
    xTaskCreate((TaskFunction_t )task9,
                    (const char*    )"task9",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK9_PRIO,
                    (TaskHandle_t*  )&Task9_Handler);




    xTaskCreate((TaskFunction_t )task10,
                    (const char*    )"task10",
                    (uint16_t       )TASK_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK10_PRIO,
                    (TaskHandle_t*  )&Task10_Handler); 

//   Mutex_Handler1 = xSemaphoreCreateMutex();		
// 	if (Mutex_Handler1 == NULL)ee_printf("Mutex Created Failed!!!\r\n");
// 	else ee_printf("Mutex Created!\n");	

    

//    t8_err += free_main_add();
//    ee_printf("t8 is %d",t8_err);


    // while(1){

    // }



    ee_printf("free start 444\n" );

    vTaskStartScheduler();

    

    while(1)
    {
        ee_printf("shouldn't run at here!!\n");
    }
}
