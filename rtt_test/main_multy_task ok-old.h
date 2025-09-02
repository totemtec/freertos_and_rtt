#define PLIC_NUM_INTERRUPTS     256
#define PLIC_NUM_PRIORITIES     8

#define PLIC_PRIORITY_BASE          0x00000000
#define PLIC_PENDING_BASE           0x00001000
#define PLIC_ENABLE_BASE            0x00002000
#define PLIC_PRIORITY_THRESHOLD     0x00200000
#define PLIC_CLAIM_COMPLETED        0x00200004

#define PLIC_BASE   0x01000000

#define reg32 *(volatile unsigned int *)

// func def
void open_plic_ie(void);



volatile int32 g_tick = 0;
struct rt_timer timer1;
void timeout1(void *parameter)
{
    //note: hear mast mast run pass
    putchar('*');
    g_tick++;
    //if( tick == 100 ) {
    //    test_pass();
    //}
}

uint32_t tshow;
struct rt_timer timer2;
void timeout2(void *parameter)
{
    //tshow = read_csr(0x7D8);
	//ee_printf_a("+%d\n", tshow );
}

int ok_num = 0;

//线程任务1
rt_uint8_t thread1_stack[2024];             //线程栈
struct rt_thread thread1_pcb;            //线程控制块
int t1_err = 0;
void thread1(void *param)
{
    t1_err += task_big();
    ee_printf_a("N1:%d\n", t1_err );
	ok_num++;
    while( 1 ) {
        //ee_printf("t1 *%d\n", tick);
        rt_thread_yield();
    }
}
//线程任务2
rt_uint8_t thread2_stack[2000];             //线程栈
struct rt_thread thread2_pcb;            //线程控制块
int t2_err = 0;
void thread2(void *param)
{
    t2_err += BubSort_main();
	t2_err += BubSort_main();
	t2_err += BubSort_main();
	t2_err += BubSort_main();
	t2_err += BubSort_main();
    ee_printf_a("F2:%d\n", t2_err );
	ok_num++;
    while(1) {
        //ee_printf("t2 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务3
rt_uint8_t thread3_stack[2000];             //线程栈
struct rt_thread thread3_pcb;            //线程控制块
int t3_err = 0;
void thread3(void *param)
{
    t3_err += task_search();
    ee_printf_a("N3:%d\n", t3_err );
	ok_num++;
    while(1) {
        //ee_printf("t3 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务4
rt_uint8_t thread4_stack[2024];             //线程栈
struct rt_thread thread4_pcb;            //线程控制块
int t4_err = 0;
void thread4(void *param)
{
    t4_err += task_maxs();
	t4_err += task_maxs();
	t4_err += task_maxs();
	t4_err += task_maxs();
	t4_err += task_maxs();
    ee_printf_a("F4:%d\n", t4_err );
	ok_num++;
    while(1) {
        //ee_printf("t4 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务5
rt_uint8_t thread5_stack[2024];             //线程栈
struct rt_thread thread5_pcb;            //线程控制块
int t5_err = 0;
void thread5(void *param)
{
    t5_err += task_filter(); //not suport moulty task!!!
    ee_printf_a("F5:%d\n", t5_err );
	ok_num++;
    while(1) {
        //ee_printf("t5 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务6
rt_uint8_t thread6_stack[2024];             //线程栈
struct rt_thread thread6_pcb;            //线程控制块
int t6_err = 0;
void thread6(void *param)
{
    t6_err += task_find();
    ee_printf_a("N6:%d\n", t6_err );
	ok_num++;
    while( 1 ) {
        //ee_printf("t6 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务7
rt_uint8_t thread7_stack[2024];             //线程栈
struct rt_thread thread7_pcb;            //线程控制块
int t7_err = 0;
void thread7(void *param)
{
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
    ee_printf_a("F7:%d\n", t7_err );
	ok_num++;
    while( 1 ) {
        //ee_printf("t7 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务8
rt_uint8_t thread8_stack[2024];             //线程栈
struct rt_thread thread8_pcb;            //线程控制块
int t8_err = 0;
void thread8(void *param)
{
	for( int i = 0; i < 20; ++i ) {
		t8_err += rvv_main_add();
	}
    ee_printf_a("V8:%d\n", t8_err );

	ok_num++;
    while( 1 ) {
        //ee_printf("t8 *%d\n",tick);
        rt_thread_yield();
    }
}
//线程任务9
rt_uint8_t thread9_stack[2024];             //线程栈
struct rt_thread thread9_pcb;            //线程控制块
int t9_err = 0;
void thread9(void *param)
{
    for( int i = 0; i < 20; ++i ) {
		t8_err += rvv_main_mul();
	}
    ee_printf_a("V9:%d\n", t9_err );

	ok_num++;
    while( 1 ) {
        //ee_printf("t9 *%d\n",tick);
        rt_thread_yield();
    }
}


void open_ie(void)
{
	unsigned int csr_tmp;
	csr_tmp = read_csr(mstatus);
    write_csr(mstatus,(csr_tmp | 0x8));
}
void close_ie(void)
{
	unsigned int csr_tmp;
	unsigned int csr_mask;
	csr_tmp = read_csr(mstatus);
	csr_mask = 0x8;
    write_csr(mstatus,csr_tmp & ~csr_mask);
}
void clear_mie(void)
{
	write_csr(mie, 0 );
}
void set_mmode(void)
{
	asm volatile( "li t0, 0x00001800" );
  	asm volatile( "csrs mstatus, t0" );
}

void plic_init(void)
{
	PLIC_init();

    PLIC_set_threshold(0);

    //context:0x0,num:0x1,priority:0x2,enable:0x1
	
    PLIC_sourceX_cfg(0x0,0x1,0x0,0x1);
    PLIC_sourceX_cfg(0x0,0x2,0x1,0x1);
    PLIC_sourceX_cfg(0x0,0x3,0x2,0x1);
    PLIC_sourceX_cfg(0x0,0x4,0x5,0x1);
	PLIC_sourceX_cfg(0x0,0x5,0x4,0x1);
    PLIC_sourceX_cfg(0x0,0x6,0x4,0x1);
    PLIC_sourceX_cfg(0x0,0x7,0x4,0x1);
    PLIC_sourceX_cfg(0x0,0x8,0x5,0x1);
	PLIC_sourceX_cfg(0x0,0x9,0x6,0x1);
    PLIC_sourceX_cfg(0x0,0x10,0x7,0x1);

	open_plic_ie();
}
void open_plic_ie(void)
{
	unsigned int csr_tmp;
	csr_tmp = read_csr(mie);
    write_csr(mie, csr_tmp | 0x00000800);//OPEN EXT TIMER
}
void close_plic_ie(void)
{
	unsigned int csr_tmp;
	unsigned int csr_mask;
	csr_tmp = read_csr(mie);
	csr_mask = 0x00000800;
	write_csr(mie, csr_tmp & ~csr_mask );//CLOSE EXT TIMER
}
int ptick = 0;
int ptick_t = 0;

uint32_t plic_irq1_mie;
uint32_t plic_irq1_mepc;
void plic_irq_vec_handler_1(void)
{
	plic_irq1_mie = read_csr(mie);
	plic_irq1_mepc = read_csr(mepc);
	uint32 id = PLIC_get_claim();
	
	ptick += 1;
	
	clear_mie();
	open_plic_ie();
	open_ie();

	ee_printf_a("{1" );
	//test_delay(25);
	ee_printf_a("}" );
	
	close_ie();
	write_csr( mepc, plic_irq1_mepc );
	write_csr( mie, plic_irq1_mie );
	set_mmode();
}
uint32_t plic_irq2_mie;
uint32_t plic_irq2_mepc;
uint32_t plic_irq2_CP;
uint32_t plic_irq2_thr;
void plic_irq_vec_handler_2(void)
{
	plic_irq2_mie = read_csr(mie);
	plic_irq2_mepc = read_csr(mepc);
	//plic_irq2_CP = read_csr(0x7D8);
	plic_irq2_thr = PLIC_get_threshold();

	//uint32 id = PLIC_get_claim();
	
	ptick_t += 1;

	putchar( '{' );
	putchar( '2' );
	//ee_printf_a("%d", plic_irq2_CP );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( '}' );

	plic_irq2_CP = 0;
	//write_csr( 0x7D8, plic_irq2_CP );
	reg32(PLIC_BASE + PLIC_CLAIM_COMPLETED) = 2;
	write_csr( mepc, plic_irq2_mepc );
	write_csr( mie, plic_irq2_mie );
	set_mmode();
}
uint32_t plic_irq3_mie;
uint32_t plic_irq3_mepc;
uint32_t plic_irq3_CP;
uint32_t plic_irq3_thr;
void plic_irq_vec_handler_3(void)
{
	plic_irq3_mie = read_csr(mie);
	plic_irq3_mepc = read_csr(mepc);
	//plic_irq3_CP = read_csr(0x7D8);
	plic_irq3_thr = PLIC_get_threshold();
	PLIC_set_threshold( 2 );
	uint32 id = PLIC_get_claim();
	
	ptick += 100;

	putchar( '[' );
	putchar( '3' );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	
	//test_delay(25);

	
	close_ie();

	putchar( ']' );

	reg32(PLIC_BASE + PLIC_CLAIM_COMPLETED) = 3;
	PLIC_set_threshold( plic_irq3_thr );
	//write_csr( 0x7D8, plic_irq3_CP>>16 );
	write_csr( mepc, plic_irq3_mepc );
	write_csr( mie, plic_irq3_mie );
	set_mmode();
}
uint32_t plic_irq4_mie;
uint32_t plic_irq4_mepc;
uint32_t plic_irq4_CP;
uint32_t plic_irq4_thr;
void plic_irq_vec_handler_4(void)
{
	plic_irq4_mie = read_csr(mie);
	plic_irq4_mepc = read_csr(mepc);
	//plic_irq4_CP = read_csr(0x7D8);
	plic_irq4_thr = PLIC_get_threshold();
	PLIC_set_threshold( 5 );
	uint32 id = PLIC_get_claim();
	
	ptick += 100;

	putchar( '(' );
	putchar( '4' );
	putchar( '0' + plic_irq4_thr );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( ')' );

	reg32(PLIC_BASE + PLIC_CLAIM_COMPLETED) = 4;
	PLIC_set_threshold( plic_irq4_thr );
	//write_csr( 0x7D8, plic_irq4_CP>>16 );
	write_csr( mepc, plic_irq4_mepc );
	write_csr( mie, plic_irq4_mie );
	set_mmode();
}
uint32_t plic_irq5_mie;
uint32_t plic_irq5_mepc;
uint32_t plic_irq5_CP;
uint32_t plic_irq5_thr;
void plic_irq_vec_handler_5(void)
{
	plic_irq5_mie = read_csr(mie);
	plic_irq5_mepc = read_csr(mepc);
	//plic_irq5_CP = read_csr(0x7D8);
	plic_irq5_thr = PLIC_get_threshold();
	PLIC_set_threshold( 4 );

	ptick += 100;

	putchar( '<' );
	putchar( '5' );
	putchar( '0' + plic_irq5_thr );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( '>' );
	
	reg32(PLIC_BASE + PLIC_CLAIM_COMPLETED) = 5;
	PLIC_set_threshold( plic_irq5_thr );
	//write_csr( 0x7D8, plic_irq5_CP>>16 );
	write_csr( mepc, plic_irq5_mepc );
	write_csr( mie, plic_irq5_mie );
	set_mmode();
}
uint32_t plic_irq6_mie;
uint32_t plic_irq6_mepc;
void plic_irq_vec_handler_6(void)
{
	plic_irq6_mie = read_csr(mie);
	plic_irq6_mepc = read_csr(mepc);
	
	ptick += 100;
	
	putchar( '<' );
	putchar( '6' );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( '>' );

	write_csr( mepc, plic_irq6_mepc );
	write_csr( mie, plic_irq6_mie );
	set_mmode();
}
uint32_t plic_irq7_mie;
uint32_t plic_irq7_mepc;
void plic_irq_vec_handler_7(void)
{
	plic_irq7_mie = read_csr(mie);
	plic_irq7_mepc = read_csr(mepc);
	
	ptick += 100;
	
	putchar( '<' );
	putchar( '7' );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( '>' );

	write_csr( mepc, plic_irq7_mepc );
	write_csr( mie, plic_irq7_mie );
	set_mmode();
}
uint32_t plic_irq8_mie;
uint32_t plic_irq8_mepc;
void plic_irq_vec_handler_8(void)
{
	plic_irq8_mie = read_csr(mie);
	plic_irq8_mepc = read_csr(mepc);
	
	ptick += 100;
	
	putchar( '<' );
	putchar( '8' );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( '>' );

	write_csr( mepc, plic_irq8_mepc );
	write_csr( mie, plic_irq8_mie );
	set_mmode();
}
uint32_t plic_irq9_mie;
uint32_t plic_irq9_mepc;
void plic_irq_vec_handler_9(void)
{
	plic_irq9_mie = read_csr(mie);
	plic_irq9_mepc = read_csr(mepc);
	
	ptick += 100;
	
	putchar( '<' );
	putchar( '9' );
	
	clear_mie();
	open_plic_ie();
	open_ie();

	//test_delay(25);
	
	close_ie();

	putchar( '>' );

	write_csr( mepc, plic_irq9_mepc );
	write_csr( mie, plic_irq9_mie );
	set_mmode();
}
uint32_t plic_irq10_mie;
uint32_t plic_irq10_mepc;
void plic_irq_vec_handler_10(void)
{
	plic_irq10_mie = read_csr(mie);
	plic_irq10_mepc = read_csr(mepc);

	ptick += 100;
	
	clear_mie();
	open_plic_ie();
	open_ie();

	ee_printf_a("<10" );
	//test_delay(25);
	ee_printf_a(">" );
	
	close_ie();
	write_csr( mepc, plic_irq10_mepc );
	write_csr( mie, plic_irq10_mie );
	set_mmode();
}

//线程任务main
void thread_m(void)
{
    while( 1 ) {

        if( ok_num == 9 ) {
			
			ee_printf_a("T:%d\n",g_tick);
			ee_printf_a("P:%d\n",ptick_t);

            if( t1_err == 0 && t2_err == 0 && t3_err == 0 && t4_err == 0 && t5_err == 0 && t6_err == 0 && t7_err == 0 && t8_err == 0 && t9_err == 0 ) { //&& ptick == 1111
                ee_printf_a("R PASS\n");
                test_delay(25);
				clint_disable();
                test_pass();
            }
            else {
                ee_printf_a("R FAIL\n");
                test_delay(25);
				clint_disable();
                test_fail();
            }
        }
        //g_tick+=100;
        //ee_printf_a("mm *%d\n",g_tick);
        rt_thread_yield();
    }
}

int main()
{
    //int i = 20; 
    //ERROR! array overflow
    
    //test_array[20] = 12;
    //test_array[i] = 34;
    //test_array[IDX] = 56;


    uart_init();

    ee_open = 2;
    test_delay(1);

	//this, and link.ld, and clint.c-timer
	ee_printf_a("rtt core 19\n" );
    //ee_printf_a("hello ahbl 2\n" );

    fpu_enable();



	//clint_init();
	//for( int i = 0; i < 100; ++i ) {
	//	ee_printf_a("test hello %d   time: %d \n", i, timer_num1 );
	//	delay_ms( 100 );
	//}
	//test_pass();
	//while(1) {}



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

	rt_timer_init( &timer2, "timer2", timeout2, RT_NULL, 5, RT_TIMER_FLAG_PERIODIC );
    rt_timer_start( &timer2 );

    rt_thread_init(&thread1_pcb, "static_thread", thread1, RT_NULL, &thread1_stack[0], sizeof(thread1_stack), 2, 1);
    rt_thread_startup(&thread1_pcb);

    rt_thread_init(&thread2_pcb, "static_thread", thread2, RT_NULL, &thread2_stack[0], sizeof(thread2_stack), 2, 1);
    rt_thread_startup(&thread2_pcb);

    rt_thread_init(&thread3_pcb, "static_thread", thread3, RT_NULL, &thread3_stack[0], sizeof(thread3_stack), 2, 1);
    rt_thread_startup(&thread3_pcb);

    rt_thread_init(&thread4_pcb, "static_thread", thread4, RT_NULL, &thread4_stack[0], sizeof(thread4_stack), 2, 1);
    rt_thread_startup(&thread4_pcb);

    rt_thread_init(&thread5_pcb, "static_thread", thread5, RT_NULL, &thread5_stack[0], sizeof(thread5_stack), 2, 1);
    rt_thread_startup(&thread5_pcb);

    rt_thread_init(&thread6_pcb, "static_thread", thread6, RT_NULL, &thread6_stack[0], sizeof(thread6_stack), 2, 1);
    rt_thread_startup(&thread6_pcb);

    rt_thread_init(&thread7_pcb, "static_thread", thread7, RT_NULL, &thread7_stack[0], sizeof(thread7_stack), 2, 1);
    rt_thread_startup(&thread7_pcb);

	rvv_init();

	rt_thread_init(&thread8_pcb, "static_thread", thread8, RT_NULL, &thread8_stack[0], sizeof(thread8_stack), 2, 1);
    rt_thread_startup(&thread8_pcb);
	
	rt_thread_init(&thread9_pcb, "static_thread", thread9, RT_NULL, &thread9_stack[0], sizeof(thread9_stack), 2, 1);
    rt_thread_startup(&thread9_pcb);

    clint_init();

	plic_init();
    
    thread_m();
}

