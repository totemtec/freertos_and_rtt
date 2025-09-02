

volatile int32_t g_tick = 0;
struct rt_timer timer1;
void timeout1(void *parameter)
{
    //putchar('*');
    g_tick++;

}

volatile int32_t g_tick1 = 0;
//线程任务1
rt_uint8_t thread1_stack[2024];             //线程栈
struct rt_thread thread1_pcb;            //线程控制块
void thread1(void *param)
{
    while( 1 ) {
        mc_ctrl();
		rt_thread_delay( 7000 );
		ee_printf_a("g_tick: %d\n",g_tick);
		g_tick1++;
    }
}

volatile int32_t g_tick2 = 0;
//线程任务2
rt_uint8_t thread2_stack[2024];             //线程栈
struct rt_thread thread2_pcb;            //线程控制块
void thread2(void *param)
{
    while( 1 ) {
        foc_update();
		rt_thread_delay( 100 );
		//ee_printf_a("thread2\n");
		g_tick2++;
    }
}

//线程任务main
void thread_m(void)
{
	thread_motor_main();
}

int main()
{
    uart_init();

    ee_open = 2;
    test_delay(1);
    ee_printf_a("111 hello\n" );

    fpu_enable();

    //线程handle
    //线程名称
    //线程入口函数
    //线程入口参数
    //线程栈地址
    //线程栈大小
    //线程优先级
    //线程时间片

    rt_timer_init( &timer1, "timer1", timeout1, RT_NULL, 2, RT_TIMER_FLAG_PERIODIC );
    rt_timer_start( &timer1 );

    rt_thread_init(&thread1_pcb, "static_thread", thread1, RT_NULL, &thread1_stack[0], sizeof(thread1_stack), 2, 1);
    rt_thread_startup(&thread1_pcb);

	rt_thread_init(&thread2_pcb, "static_thread", thread2, RT_NULL, &thread2_stack[0], sizeof(thread2_stack), 2, 1);
    rt_thread_startup(&thread2_pcb);

    clint_init();
    
    thread_m();
}

