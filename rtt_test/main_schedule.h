
//线程任务1
int t1_err = -1;
rt_uint8_t thread1_stack[1024];             //线程栈
struct rt_thread thread1_pcb;            //线程控制块
void thread1(void *param)
{
   //note: this function can't called by main!!!!
    fpu_enable();
    t1_err = float_test_error();
    while( 1 ) {
        //ee_printf("t1:%d\n",t1_err);
        rt_thread_yield();
    }
}


//线程任务2
int t2_err = -1;

int t_last = 0;
void add_step( int n )
{
    if( t2_err > 0 ) {
        return;
    }
    if( t_last + 1 != n ) {
        t2_err = 1;
        return;
    }
    t_last = n;
}
rt_uint8_t thread2_stack[1024];             //线程栈
struct rt_thread thread2_pcb;            //线程控制块
void thread2(void *param)
{
    ee_printf("t2-1\n");
    add_step( 1 );
    rt_thread_yield();
    ee_printf("t2-3\n");
    add_step( 3 );
    rt_thread_yield();
    ee_printf("t2-7\n");
    add_step( 7 );
    rt_thread_delay(2);
    ee_printf("t2-10\n");
    add_step( 10 );

    if( t_last == 10 ) {
        t2_err = 0;
    }
    while(1) {
        //ee_printf("t2:%d\n",t2_err);
        rt_thread_yield();
    }
}
//线程任务3
rt_uint8_t thread3_stack[2000];             //线程栈
struct rt_thread thread3_pcb;            //线程控制块
void thread3(void *param)
{
    ee_printf("t3-2\n");
    add_step( 2 );
    rt_thread_yield();
    rt_thread_suspend( &thread2_pcb );
    ee_printf("t3-4\n");
    add_step( 4 );
    rt_thread_yield();
    ee_printf("t3-5\n");
    add_step( 5 );
    rt_thread_yield();
    ee_printf("t3-6\n");
    add_step( 6 );
    rt_thread_resume( &thread2_pcb );
    rt_thread_yield();
    ee_printf("t3-8\n");
    add_step( 8 );
    rt_thread_yield();
    ee_printf("t3-9\n");
    add_step( 9 );

    while(1) {
        rt_thread_yield();
    }
}
struct rt_timer timer1;
void timeout1(void *parameter)
{
    //ee_printf("*\n");
}
//线程任务main
void thread_m(void)
{
    while( 1 ) {

        if( t1_err != -1 && t2_err != -1 ) {
            
            //ee_open = true;
            ee_printf("e1:%d\n",t1_err);
            ee_printf("e2:%d\n",t2_err);
            ee_printf("test finish\n");
            ee_printf("test finish\n");
            ee_printf("test finish\n");
            ee_printf("test finish\n");
            if( t1_err != 0 || t2_err != 0 ) {
                test_fail();
            }
            else {
                test_pass();
            }
        }
        //ee_printf("m:%d\n",timer_num1);
        rt_thread_yield();
    }
}
void test_float(void);

int main()
{
    ee_open = false;
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
