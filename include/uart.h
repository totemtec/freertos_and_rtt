#ifndef UART_H__
#define UART_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define UART0_BASE 0x80001000
#define UART_RX_REG 0
#define UART_TX_REG 4
#define UART_STATUS_REG 8

#define UART_STATUS_RX_EMPTY 1
#define UART_STATUS_TX_FULL 2

#define UART_EOF -1

#define UART_IRQ_NUM 16
#define UART_IRQ (1 << UART_IRQ_NUM)
#define DEFAULT_UART UART_FROM_BASE_ADDR(UART0_BASE)

#define DEV_WRITE(addr, val) (*((volatile uint32_t *)(addr)) = val)
#define DEV_READ(addr) (*((volatile uint32_t *)(addr)))

typedef void* uart_t;

#define UART_FROM_BASE_ADDR(addr)((uart_t)(addr))

void enable_interrupts(uint32_t enable_interrupts);
void set_global_interrupt_enable(uint32_t enable);

int install_exception_handler(uint32_t vector_num, void(*handler_fn)(void));

void uart_enable_rx_int(void);
int uart_in(uart_t uart);
void uart_out(uart_t uart, char c);
void uart_init(void);

int putchar(int c);
int getchar(void);
int puts(const char *str);
void puthex(uint32_t h);

#define	_CR_CRLF		0	/* 1: Convert \n ==> \r\n in the output char */

typedef size_t ee_size_t;
int ee_printf(const char *fmt, ...);
int ee_printf_a(const char *fmt, ...);
int ee_printf_info(const char *fmt, ...);
// void xprintf (const char* fmt, ...);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

void my_putchar( char c );

#endif // UART_H__