#include <stdint.h>

#include "../include/uart.h"
#include "../include/utils.h"

#include "rtthread.h"


struct rt_thread *my_main_thread;
extern struct rt_thread main_thread;

extern unsigned int timer_num1;

extern unsigned char ee_open;
int entry();
void entry_init(void)
{
  /*
  extern char _bss_start, _bss_end;
  size_t bss_size = &_bss_end - &_bss_start;
  memset(&_bss_start, 0, bss_size);
  */

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
 
  entry();
}

//---------------------------------
//#include "main_schedule.h"
//#include "main_thread.h"


//this is the test multy task
#include "my_typedef.h"
#include "main_multy_task.h"

//this is the test motor
//#include "main_motor.h"

//this is the test Atom
//#include "main_Atom.h"

