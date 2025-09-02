#include <stdint.h>

#include "../include/uart.h"
#include "../include/utils.h"

#include "my_typedef.h"



int main_PressureTest();

//-------------------------------------------------------

void clint_init(void);

//-------------------------------------------------------

extern bool ee_open;

void entry_init(void)
{
	//extern char _bss_start, _bss_end;
	//size_t bss_size = &_bss_end - &_bss_start;
	//memset(&_bss_start, 0, bss_size);

  //fpu_enable();
  //ee_printf("f5 test:\n" );
  //t1_err = float_test_error();
  //ee_printf("end\n" );

  /*
  clint_init();
  while(1) {
      ee_printf("11\n" );
      ee_printf("22\n" );
      CLINT_misp_write(1);
  }
  */
 
  main_PressureTest();
//	main();
}


//command: make find_chisel chisel=1 vcs_dir50m_core

void plic_irq_vec_handler_1(void)
{
}
void plic_irq_vec_handler_2(void)
{
}
void plic_irq_vec_handler_3(void)
{
}
void plic_irq_vec_handler_4(void)
{
}
void plic_irq_vec_handler_5(void)
{
}
void plic_irq_vec_handler_6(void)
{
}
void plic_irq_vec_handler_7(void)
{
}
void plic_irq_vec_handler_8(void)
{
}
void plic_irq_vec_handler_9(void)
{
}


/*
 *@Note
 task1 and task2 alternate printing
*/
int main(void);
void mydelay_ms(int n){
	int tick=0;
	for(int i=0;i<n;i++){
		for(int j=0;j<1000;j++){
			tick+=1;
		}
	}main();
}

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "croutine.h"


QueueHandle_t Queue_Handler;
#define QUEUE_LENGTH 3
#define QUEUE_SIZE   sizeof(int)
#define QUEUE_WAIT_MAX 10



extern eTaskState Astate,Bstate,Cstate,Dstate;
TaskHandle_t Task1Task_Handler, Task2Task_Handler, Task3Task_Handler, Task4Task_Handler;

extern char InfoBuffer[400];


//Task1 (message queue)
int abc1;
#define TASK1_TASK_PRIO     8
#define TASK1_STK_SIZE      256
TaskHandle_t Task1Task_Handler;
eTaskState Astate;

int info[5]={111,222,333,444,555};
void task1_task(void *pvParameters)
{
	abc1 = 0;
	int send_meg;
	BaseType_t err;

	Queue_Handler = xQueueCreate( QUEUE_LENGTH, QUEUE_SIZE );
	if (Queue_Handler == NULL)ee_printf("Creat queue failed!!!\r\n");
	else ee_printf("Queue Created\n");
	
	for(int i=0;i<5;i++){//send 5 messages
		abc1++;

		send_meg=info[i];
		err=xQueueSend(Queue_Handler,&send_meg,QUEUE_WAIT_MAX);

		while(err!=pdTRUE){//keep sending until successful
			ee_printf("send failed,the queue is full!\n");
			err=xQueueSend(Queue_Handler,&send_meg,QUEUE_WAIT_MAX);
		}

		ee_printf("%d sent\n",send_meg);
	}

	ee_printf("T1 Suspended\n");
	vTaskSuspend(Task1Task_Handler);
	

	while(1) {
		// abc1++;
		//taskYIELD();
		//vTaskDelay(10);
	}
}



//Task2 (message group)
int abc2;
#define TASK2_TASK_PRIO     8
#define TASK2_STK_SIZE      256
TaskHandle_t Task2Task_Handler;
eTaskState Bstate;

void task2_task(void *pvParameters)
{
	abc2 = 0;

	int rec_meg;
	BaseType_t err;

	for(int i=0;i<5;i++){//receive 5 messages
		abc2++;
		err = xQueueReceive(Queue_Handler,&rec_meg,QUEUE_WAIT_MAX);

		while(err!=pdTRUE){//keep recieving until successful
			ee_printf("receive failed,the queue is null!\n");
			err=xQueueReceive(Queue_Handler,&rec_meg,QUEUE_WAIT_MAX);
		}

		ee_printf("%d taken\n",rec_meg);
	}
	
	ee_printf("T2 Suspended\n");
	vTaskSuspend(Task2Task_Handler);

	while(1) {
		//abc2++;

		//vTaskDelay(10);
		//taskYIELD();
		//if(abc2%3000==0)vTaskDelay(15);
	}
}


volatile int test_num=0;

SemaphoreHandle_t Mutex_Handler;

//task3 mutex
int abc3;extern int abc4;
#define TASK3_TASK_PRIO     7
#define TASK3_STK_SIZE      256
TaskHandle_t Task3Task_Handler;
eTaskState Cstate;

void task3_task(void *pvParameters)
{
	abc3 = 0;

	Mutex_Handler = xSemaphoreCreateMutex();		
	if (Mutex_Handler == NULL)ee_printf("Mutex Created Failed!!!\r\n");
	else ee_printf("Mutex Created!\n");	
	

	for(;abc3<2000;abc3++){
		//xSemaphoreTake(Mutex_Handler,QUEUE_WAIT_MAX);
		if(xSemaphoreTake(Mutex_Handler,QUEUE_WAIT_MAX)==pdFALSE)ee_printf("mutex take failed!\n");//if the mutex return false , the code will excute continutely , instead of blocking

		test_num+=1;

		xSemaphoreGive(Mutex_Handler);
	}

	if(abc3==2000&&abc4==3000){
		if(test_num==5000)ee_printf("test_num is %d,correct!\n",test_num);

		else ee_printf("test_num is %d,wrong answer!\n",test_num);
	}

	ee_printf("T3 Suspended\n");
	vTaskSuspend(Task3Task_Handler);

	while(1) {
		
		//abc3++;
		
		//taskYIELD();
		//vTaskDelayAstate= eTaskGetState(Task1Task_Handler);(1);
	}
}


//task4 mutex 
int abc4;
#define TASK4_TASK_PRIO     7
#define TASK4_STK_SIZE      256
TaskHandle_t Task4Task_Handler;

eTaskState Dstate;
void task4_task(void *pvParameters)
{
	abc4 = 0;

	for(;abc4<3000;abc4++){
		if(xSemaphoreTake(Mutex_Handler,QUEUE_WAIT_MAX)==pdFALSE)ee_printf("mutex take failed!\n");
		//xSemaphoreTake(Mutex_Handler,QUEUE_WAIT_MAX);

		test_num+=1;

		xSemaphoreGive(Mutex_Handler);
	}

	if(abc3==2000&&abc4==3000){
		if(test_num==5000)ee_printf("test_num is %d,correct!\n",test_num);

		else ee_printf("test_num is %d,wrong answer!\n",test_num);
	}

	ee_printf("T4 Suspended\n");
	vTaskSuspend(Task4Task_Handler);


	while(1) {
		
		//abc4++;
		//taskYIELD();
		//vTaskDelay(3);
	}
}



#define TIMER1_PERIOD   6
#define TIMER1_ID       1

TimerHandle_t Timer1_Handler;

void Timer1_CallBack(TimerHandle_t xTimer)
{
	static int count1 = 0;
	count1++;
	ee_printf("timer1 Call Back %d\r\n",count1);

	if(count1==5){
		ee_printf("timer1 stop\n");
		xTimerStop(Timer1_Handler, 10 );
	}
}


#define TIMER2_PERIOD   8
#define TIMER2_ID       2

TimerHandle_t Timer2_Handler;
char InfoBuffer[400];

// extern TaskHandle_t pxCurrentTCB;
// extern struct tskTaskControlBlock;

void Timer2_CallBack(TimerHandle_t xTimer)
{
	static int count2 = 0;
	count2++;
	ee_printf("timer2 Call Back %d\r\n",count2);

	if(count2==5){
		ee_printf("timer2 stop\n");
		xTimerStop(Timer2_Handler, 10 );

		// TaskHandle_t tmp_handle = xTaskGetCurrentTaskHandle();//to prove soft timer is in "tmr_svc" task
		// char* str;
		// str=pcTaskGetName(tmp_handle);
		// ee_printf("timer2 in %s\n", str);

		vTaskList(InfoBuffer);
		ee_printf("taskName\ttaskState\ttaskPrio\ttaskStack\ttaskNum\r\n");
		ee_printf_info("%s",InfoBuffer);
	}
}


//task5 software timer *2
int abc5;
#define TASK5_TASK_PRIO     10
#define TASK5_STK_SIZE      256
TaskHandle_t Task5Task_Handler;
eTaskState Estate;

void task5_task(void *pvParameters)
{
	abc5 = 0;

	BaseType_t err;
	 
	Timer1_Handler = xTimerCreate((char *)"Timer1",
									(TickType_t )TIMER1_PERIOD,
									(UBaseType_t) pdTRUE,
									(void *)TIMER1_ID,
									(TimerCallbackFunction_t) Timer1_CallBack);
	
	err = xTimerStart(Timer1_Handler,10);
	if(err==pdTRUE)ee_printf("Timer1 Created\n");
	else ee_printf("Timer1 Create Failed!\n");
		



	Timer2_Handler = xTimerCreate((char *)"Timer2",
									(TickType_t )TIMER2_PERIOD,
									(UBaseType_t) pdTRUE,
									(void *)TIMER2_ID,
									(TimerCallbackFunction_t) Timer2_CallBack);
	
	err = xTimerStart(Timer2_Handler,10);
	if(err==pdTRUE)ee_printf("Timer2 Created\n");
	else ee_printf("Timer2 Create Failed!\n");
		
	ee_printf("T5 Suspended\n");
	vTaskSuspend(Task5Task_Handler);

	while(1) {
		
		//abc4++;
		//taskYIELD();
		//vTaskDelay(100);
	}
}



//CoRoutine
void vCoRoutineFunction( CoRoutineHandle_t xHandle,
                              UBaseType_t uxIndex )
    {
        crSTART( xHandle );

        for( ;; )
        {
            ee_printf("CoRoutine %d running\n",(int)uxIndex);

			crDELAY( xHandle, 1 );

        }

        crEND();
    }



int main(void)
{
	//ee_printf("rest space is %d\n",xPortGetFreeHeapSize());
	ee_printf("run in main\n");

	int a=xTaskCreate((TaskFunction_t )task1_task,
                    (const char*    )"task1",
                    (uint16_t       )TASK1_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK1_TASK_PRIO,
                    (TaskHandle_t*  )&Task1Task_Handler);

	//ee_printf("rest space is %d\n",xPortGetFreeHeapSize());
	
	
	int b=xTaskCreate((TaskFunction_t )task2_task,
                    (const char*    )"task2",
                    (uint16_t       )TASK2_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK2_TASK_PRIO,
                	(TaskHandle_t*  )&Task2Task_Handler);
	


	int c=xTaskCreate((TaskFunction_t )task3_task,
                    (const char*    )"task3",
                    (uint16_t       )TASK3_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK3_TASK_PRIO,
                    (TaskHandle_t*  )&Task3Task_Handler);

	int d=xTaskCreate((TaskFunction_t )task4_task,
                    (const char*    )"task4",
                    (uint16_t       )TASK4_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK4_TASK_PRIO,
                    (TaskHandle_t*  )&Task4Task_Handler);

	int e=xTaskCreate((TaskFunction_t )task5_task,
                    (const char*    )"task5",
                    (uint16_t       )TASK5_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK5_TASK_PRIO,
                    (TaskHandle_t*  )&Task5Task_Handler);

	

	

	// CoRoutine
	xCoRoutineCreate( vCoRoutineFunction, 0, 1 );
	xCoRoutineCreate( vCoRoutineFunction, 0, 2 );


	// ee_printf("a=%d b=%d c=%d d=%d\n",a,b,c,d);

	ee_open = true;

    ee_printf("free start\n" );

	//clint_init();

	


	vTaskStartScheduler();
	
    while(1)
    {
		// abc2 += 10000;
        ee_printf("shouldn't run at here!!\n");
    }
}



// int main(){

// 	ee_printf("start\n");

// 	clint_init();

// 	while(1){

// 	}

// }




//Astate Bstate Cstate tm1 abc1 abc2 abc3 abc4