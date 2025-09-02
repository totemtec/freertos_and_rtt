
//线程任务1
rt_uint8_t thread1_stack[2000];             //线程栈
struct rt_thread thread1_pcb;            //线程控制块
void thread1(void *param)
{
    while( 1 ) {
        ee_printf("t1-1\n");
        ee_printf("t1-2\n");
        ee_printf("t1-3\n");
        ee_printf("t1-4\n");
    }

}
//线程任务2
rt_uint8_t thread2_stack[2000];             //线程栈
struct rt_thread thread2_pcb;            //线程控制块
void thread2(void *param)
{
    while(1) {
        ee_printf("t2-1\n");
        ee_printf("t2-2\n");
    }
}
//线程任务3
rt_uint8_t thread3_stack[2000];             //线程栈
struct rt_thread thread3_pcb;            //线程控制块
void thread3(void *param)
{
    while(1) {
        ee_printf("t3-A\n");
        ee_printf("t3-B\n");
        ee_printf("t3-C\n");
        ee_printf("t3-D\n");
    }
}
int tick;
struct rt_timer timer1;
void timeout1(void *parameter)
{
    tick++;
}

//线程任务main
void thread_m(void)
{
    while( 1 ) {
        ee_printf("m-X\n");
        ee_printf("m-Y\n");
        ee_printf("m-T%d\n", tick);
    }
}

int main()
{
    ee_open = true;
    ee_printf("rt 333\n" );

    //test_pid();

    rt_thread_init(&thread1_pcb,                          //线程handle
                                             "static_thread",           //线程名称
                                             thread1,              //线程入口函数
                                             RT_NULL,                   //线程入口参数
                                             &thread1_stack[0],         //线程栈地址
                                             sizeof(thread1_stack),     //线程栈大小
                                             2,                        //线程优先级
                                             1);                        //线程时间片
    rt_thread_startup(&thread1_pcb);                      //线程进入就绪态
    
    rt_thread_init(&thread2_pcb,                          //线程handle
                                             "static_thread",           //线程名称
                                             thread2,              //线程入口函数
                                             RT_NULL,                   //线程入口参数
                                             &thread2_stack[0],         //线程栈地址
                                             sizeof(thread2_stack),     //线程栈大小
                                             2,                        //线程优先级
                                             1);                        //线程时间片
    rt_thread_startup(&thread2_pcb);                      //线程进入就绪态

    rt_thread_init(&thread3_pcb,                          //线程handle
                                             "static_thread",           //线程名称
                                             thread3,              //线程入口函数
                                             RT_NULL,                   //线程入口参数
                                             &thread3_stack[0],         //线程栈地址
                                             sizeof(thread3_stack),     //线程栈大小
                                             2,                        //线程优先级
                                             1);                        //线程时间片
    rt_thread_startup(&thread3_pcb);                      //线程进入就绪态

    rt_timer_init( &timer1, "timer1", timeout1, RT_NULL, 1, RT_TIMER_FLAG_PERIODIC );
    rt_timer_start( &timer1 );
    
    clint_init();
    
    thread_m();
}
