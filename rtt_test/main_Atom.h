

void plic_irq_base_handler(void) {}
void plic_irq_vec_handler_0(void) {}
void plic_irq_vec_handler_1(void) {}
void plic_irq_vec_handler_2(void) {}
void plic_irq_vec_handler_3(void) {}
void plic_irq_vec_handler_4(void) {}
void plic_irq_vec_handler_5(void) {}
void plic_irq_vec_handler_6(void) {}
void plic_irq_vec_handler_7(void) {}
void plic_irq_vec_handler_8(void) {}
void plic_irq_vec_handler_9(void) {}

int cas(int* p_val, int old_cmp, int new);
int amoadd_ins(int* p_val, int data1 );

int my_tick = 0;

int cmp_tick = 0;

//线程任务1
rt_uint8_t thread1_stack[2024];             //线程栈
struct rt_thread thread1_pcb;            //线程控制块
void thread1(void *param)
{
	for( int i = 0; i < 1000; i++) {

		//my_tick = my_tick + 1;

		amoadd_ins( &my_tick, 2 );

		/*
		int a, b;
		do {
			a = my_tick;
    		b = a + 1;
		}
		while( cas( &my_tick, a, b ) );
		*/

	}
	cmp_tick++;
    while( 1 ) {}
}

//线程任务2
rt_uint8_t thread2_stack[2024];             //线程栈
struct rt_thread thread2_pcb;            //线程控制块
void thread2(void *param)
{
	for( int i = 0; i < 1000; i++) {
		
		//my_tick = my_tick + 1;

		//amoadd_ins( &my_tick, 3 );

	/*
		int a, b;
		do {
			a = my_tick;
    		b = a + 1;
		}
		while( cas( &my_tick, a, b ) );
	*/

	}
	cmp_tick++;
    while( 1 ) {}
}

//线程任务main
void thread_m(void)
{
	while(1) {
		ee_printf_a("my_tick: %d\n", my_tick );
		//delay_ms(5);

		if( cmp_tick == 2 ) {
			//test_delay(10);
			//test_pass();
		}
	}
}

int main()
{
    uart_init();

    ee_open = 2;
    test_delay(1);
    ee_printf_a("bosc 555\n" );

    fpu_enable();

    //线程handle
    //线程名称
    //线程入口函数
    //线程入口参数
    //线程栈地址
    //线程栈大小
    //线程优先级
    //线程时间片

    rt_thread_init(&thread1_pcb, "static_thread", thread1, RT_NULL, &thread1_stack[0], sizeof(thread1_stack), 2, 1);
    rt_thread_startup(&thread1_pcb);

	rt_thread_init(&thread2_pcb, "static_thread", thread2, RT_NULL, &thread2_stack[0], sizeof(thread2_stack), 2, 1);
    rt_thread_startup(&thread2_pcb);

    clint_init();
    
    thread_m();
}

