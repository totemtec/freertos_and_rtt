#include "../include/uart.h"
#include "../include/htif.h"

#include "global.h"

long hw_interrupt_disable();
long hw_interrupt_enable(long);

static char *outptr;

void enable_interrupts(uint32_t enable_mask){
    asm volatile("csrs mie, %0\n" : : "r"(enable_mask));
}

void set_global_interrupt_enable(uint32_t enable){
    if(enable) {
        asm volatile("csrs mstatus, %0\n" : : "r"(1<<3));
    } else {
        asm volatile("csrc mstatus, %0\n" : : "r"(1<<3));
    }
}

extern uint32_t _vectors_start;
volatile uint32_t* exc_vectors = &_vectors_start;

int install_exception_handler(uint32_t vector_num, void(*handler_fn)(void)) {
  if (vector_num >= 32)
    return 1;

  volatile uint32_t* handler_jmp_loc = exc_vectors + vector_num;
  int32_t offset = (uint32_t)handler_fn - (uint32_t)handler_jmp_loc;

  if ((offset  >= (1 << 19)) || (offset  < -(1 << 19))) {
    return 2;
  }

  uint32_t offset_uimm = offset;

  uint32_t jmp_ins =
    ((offset_uimm & 0x7fe) << 20) | // imm[10:1] -> 21
    ((offset_uimm & 0x800) << 9) | // imm[11] -> 20
    (offset_uimm & 0xff000) | // imm[19:12] -> 12
    ((offset_uimm & 0x100000) << 11) | // imm[20] -> 31
    0x6f; // J opcode

  *handler_jmp_loc = jmp_ins;

  __asm__ volatile("fence.i;");

  return 0;
}

void uart_enable_rx_int(void) {
  enable_interrupts(UART_IRQ);
  set_global_interrupt_enable(1);
}

int uart_in(uart_t uart) {
  int res = UART_EOF;

  if (!(DEV_READ(uart + UART_STATUS_REG) & UART_STATUS_RX_EMPTY)) {
    res = DEV_READ(uart + UART_RX_REG);
  }

  return res;
}

//#include "rtthread.h"


//static struct rt_mutex mutex;

void uart_init(void)
{
    //rt_mutex_init(&mutex, "smutex", RT_IPC_FLAG_FIFO);
}


volatile uint32_t FastuartPort __attribute__((section(".uartif")));
volatile uint32_t FastuartPortIdx = 0x100;


void uart_out(uart_t uart, char c) {

  //rt_err_t result = rt_mutex_take(&mutex, RT_WAITING_FOREVER);

//rt_base_t level = rt_hw_interrupt_disable();

#if IS_VCS_NOT_SPIKE

	while(DEV_READ(uart + UART_STATUS_REG) & UART_STATUS_TX_FULL);
    DEV_WRITE(uart + UART_TX_REG, c);

	//FastuartPortIdx += 0x100;
	//FastuartPort = FastuartPortIdx + c;
	


#else
	htif_console_putchar(c);
#endif



//rt_hw_interrupt_enable(level);

  //rt_mutex_release(&mutex);

}

int putchar(int c) {
    if(c == '\n') {
        uart_out(DEFAULT_UART,'\r');
    }
    uart_out(DEFAULT_UART,c);
    return c;
}

int getchar(void) {
    return uart_in(DEFAULT_UART);
}

int puts(const char *str) {
    while (*str) {
        putchar(*str++);
    }
    return 0;
}

void puthex(uint32_t h) {
  int cur_digit;
  // Iterate through h taking top 4 bits each time and outputting ASCII of hex
  // digit for those 4 bits
  for (int i = 0; i < 8; i++) {
    cur_digit = h >> 28;

    if (cur_digit < 10)
      putchar('0' + cur_digit);
    else
      putchar('A' - 10 + cur_digit);

    h <<= 4;
  }
}

#define ZEROPAD (1 << 0)   /* Pad with zero */
#define SIGN (1 << 1)      /* Unsigned/signed long */
#define PLUS (1 << 2)      /* Show plus */
#define SPACE (1 << 3)     /* Spacer */
#define LEFT (1 << 4)      /* Left justified */
#define HEX_PREP (1 << 5)  /* 0x */
#define UPPERCASE (1 << 6) /* 'ABCDEF' */

#define is_digit(c) ((c) >= '0' && (c) <= '9')

static char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static char *upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static ee_size_t strnlen(const char *s, ee_size_t count);

static ee_size_t strnlen(const char *s, ee_size_t count) {
  const char *sc;
  for (sc = s; *sc != '\0' && count--; ++sc)
    ;
  return sc - s;
}

static int skip_atoi(const char **s) {
  int i = 0;
  while (is_digit(**s))
    i = i * 10 + *((*s)++) - '0';
  return i;
}

static char *number(char *str, long num, int base, int size, int precision,
                    int type) {
  char c, sign, tmp[66];
  char *dig = digits;
  int i;

  if (type & UPPERCASE)
    dig = upper_digits;
  if (type & LEFT)
    type &= ~ZEROPAD;
  if (base < 2 || base > 36)
    return 0;

  c = (type & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (type & SIGN) {
    if (num < 0) {
      sign = '-';
      num = -num;
      size--;
    } else if (type & PLUS) {
      sign = '+';
      size--;
    } else if (type & SPACE) {
      sign = ' ';
      size--;
    }
  }

  if (type & HEX_PREP) {
    if (base == 16)
      size -= 2;
    else if (base == 8)
      size--;
  }

  i = 0;

  if (num == 0)
    tmp[i++] = '0';
  else {
    while (num != 0) {
      tmp[i++] = dig[((unsigned long)num) % (unsigned)base];
      num = ((unsigned long)num) / (unsigned)base;
    }
  }

  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type & (ZEROPAD | LEFT)))
    while (size-- > 0)
      *str++ = ' ';
  if (sign)
    *str++ = sign;

  if (type & HEX_PREP) {
    if (base == 8)
      *str++ = '0';
    else if (base == 16) {
      *str++ = '0';
      *str++ = digits[33];
    }
  }

  if (!(type & LEFT))
    while (size-- > 0)
      *str++ = c;
  while (i < precision--)
    *str++ = '0';
  while (i-- > 0)
    *str++ = tmp[i];
  while (size-- > 0)
    *str++ = ' ';

  return str;
}

static char *eaddr(char *str, unsigned char *addr, int size, int precision,
                   int type) {
  char tmp[24];
  char *dig = digits;
  int i, len;

  if (type & UPPERCASE)
    dig = upper_digits;
  len = 0;
  for (i = 0; i < 6; i++) {
    if (i != 0)
      tmp[len++] = ':';
    tmp[len++] = dig[addr[i] >> 4];
    tmp[len++] = dig[addr[i] & 0x0F];
  }

  if (!(type & LEFT))
    while (len < size--)
      *str++ = ' ';
  for (i = 0; i < len; ++i)
    *str++ = tmp[i];
  while (len < size--)
    *str++ = ' ';

  return str;
}

static char *iaddr(char *str, unsigned char *addr, int size, int precision,
                   int type) {
  char tmp[24];
  int i, n, len;

  len = 0;
  for (i = 0; i < 4; i++) {
    if (i != 0)
      tmp[len++] = '.';
    n = addr[i];

    if (n == 0)
      tmp[len++] = digits[0];
    else {
      if (n >= 100) {
        tmp[len++] = digits[n / 100];
        n = n % 100;
        tmp[len++] = digits[n / 10];
        n = n % 10;
      } else if (n >= 10) {
        tmp[len++] = digits[n / 10];
        n = n % 10;
      }

      tmp[len++] = digits[n];
    }
  }

  if (!(type & LEFT))
    while (len < size--)
      *str++ = ' ';
  for (i = 0; i < len; ++i)
    *str++ = tmp[i];
  while (len < size--)
    *str++ = ' ';

  return str;
}

#ifdef HAS_FLOAT

char *ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
char *fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
static void ee_bufcpy(char *d, char *s, int count);

void ee_bufcpy(char *pd, char *ps, int count) {
  char *pe = ps + count;
  while (ps != pe)
    *pd++ = *ps++;
}

static void parse_float(double value, char *buffer, char fmt, int precision) {
  int decpt, sign, exp, pos;
  char *digits = NULL;
  char cvtbuf[80];
  int capexp = 0;
  int magnitude;

  if (fmt == 'G' || fmt == 'E') {
    capexp = 1;
    fmt += 'a' - 'A';
  }

  if (fmt == 'g') {
    digits = ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
    magnitude = decpt - 1;
    if (magnitude < -4 || magnitude > precision - 1) {
      fmt = 'e';
      precision -= 1;
    } else {
      fmt = 'f';
      precision -= decpt;
    }
  }

  if (fmt == 'e') {
    digits = ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);

    if (sign)
      *buffer++ = '-';
    *buffer++ = *digits;
    if (precision > 0)
      *buffer++ = '.';
    ee_bufcpy(buffer, digits + 1, precision);
    buffer += precision;
    *buffer++ = capexp ? 'E' : 'e';

    if (decpt == 0) {
      if (value == 0.0)
        exp = 0;
      else
        exp = -1;
    } else
      exp = decpt - 1;

    if (exp < 0) {
      *buffer++ = '-';
      exp = -exp;
    } else
      *buffer++ = '+';

    buffer[2] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[1] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[0] = (exp % 10) + '0';
    buffer += 3;
  } else if (fmt == 'f') {
    digits = fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
    if (sign)
      *buffer++ = '-';
    if (*digits) {
      if (decpt <= 0) {
        *buffer++ = '0';
        *buffer++ = '.';
        for (pos = 0; pos < -decpt; pos++)
          *buffer++ = '0';
        while (*digits)
          *buffer++ = *digits++;
      } else {
        pos = 0;
        while (*digits) {
          if (pos++ == decpt)
            *buffer++ = '.';
          *buffer++ = *digits++;
        }
      }
    } else {
      *buffer++ = '0';
      if (precision > 0) {
        *buffer++ = '.';
        for (pos = 0; pos < precision; pos++)
          *buffer++ = '0';
      }
    }
  }

  *buffer = '\0';
}

static void decimal_point(char *buffer) {
  while (*buffer) {
    if (*buffer == '.')
      return;
    if (*buffer == 'e' || *buffer == 'E')
      break;
    buffer++;
  }

  if (*buffer) {
    int n = strnlen(buffer, 256);
    while (n > 0) {
      buffer[n + 1] = buffer[n];
      n--;
    }

    *buffer = '.';
  } else {
    *buffer++ = '.';
    *buffer = '\0';
  }
}

static void cropzeros(char *buffer) {
  char *stop;

  while (*buffer && *buffer != '.')
    buffer++;
  if (*buffer++) {
    while (*buffer && *buffer != 'e' && *buffer != 'E')
      buffer++;
    stop = buffer--;
    while (*buffer == '0')
      buffer--;
    if (*buffer == '.')
      buffer--;
    while (buffer != stop)
      *++buffer = 0;
  }
}

static char *flt(char *str, double num, int size, int precision, char fmt,
                 int flags) {
  char tmp[80];
  char c, sign;
  int n, i;

  // Left align means no zero padding
  if (flags & LEFT)
    flags &= ~ZEROPAD;

  // Determine padding and sign char
  c = (flags & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (flags & SIGN) {
    if (num < 0.0) {
      sign = '-';
      num = -num;
      size--;
    } else if (flags & PLUS) {
      sign = '+';
      size--;
    } else if (flags & SPACE) {
      sign = ' ';
      size--;
    }
  }

  // Compute the precision value
  if (precision < 0)
    precision = 6;  // Default precision: 6

  // Convert floating point number to text
  parse_float(num, tmp, fmt, precision);

  if ((flags & HEX_PREP) && precision == 0)
    decimal_point(tmp);
  if (fmt == 'g' && !(flags & HEX_PREP))
    cropzeros(tmp);

  n = strnlen(tmp, 256);

  // Output number with alignment and padding
  size -= n;
  if (!(flags & (ZEROPAD | LEFT)))
    while (size-- > 0)
      *str++ = ' ';
  if (sign)
    *str++ = sign;
  if (!(flags & LEFT))
    while (size-- > 0)
      *str++ = c;
  for (i = 0; i < n; i++)
    *str++ = tmp[i];
  while (size-- > 0)
    *str++ = ' ';

  return str;
}

#endif

static int ee_vsprintf(char *buf, const char *fmt, va_list args) {



  //rt_base_t level = rt_hw_interrupt_disable();



  int len;
  unsigned long num;
  int i, base;
  char *str;
  char *s;

  int flags;  // Flags to number()

  int field_width;  // Width of output field
  int precision;  // Min. # of digits for integers; max number of chars for from
                  // string
  int qualifier;  // 'h', 'l', or 'L' for integer fields

  for (str = buf; *fmt; fmt++) {
    if (*fmt != '%') {
      *str++ = *fmt;
      continue;
    }

    // Process flags
    flags = 0;
  repeat:
    fmt++;  // This also skips first '%'
    switch (*fmt) {
      case '-':
        flags |= LEFT;
        goto repeat;
      case '+':
        flags |= PLUS;
        goto repeat;
      case ' ':
        flags |= SPACE;
        goto repeat;
      case '#':
        flags |= HEX_PREP;
        goto repeat;
      case '0':
        flags |= ZEROPAD;
        goto repeat;
    }

    // Get field width
    field_width = -1;
    if (is_digit(*fmt))
      field_width = skip_atoi(&fmt);
    else if (*fmt == '*') {
      fmt++;
      field_width = va_arg(args, int);
      if (field_width < 0) {
        field_width = -field_width;
        flags |= LEFT;
      }
    }

    // Get the precision
    precision = -1;
    if (*fmt == '.') {
      ++fmt;
      if (is_digit(*fmt))
        precision = skip_atoi(&fmt);
      else if (*fmt == '*') {
        ++fmt;
        precision = va_arg(args, int);
      }
      if (precision < 0)
        precision = 0;
    }

    // Get the conversion qualifier
    qualifier = -1;
    if (*fmt == 'l' || *fmt == 'L') {
      qualifier = *fmt;
      fmt++;
    }

    // Default base
    base = 10;

    switch (*fmt) {
      case 'c':
        if (!(flags & LEFT))
          while (--field_width > 0)
            *str++ = ' ';
        *str++ = (unsigned char)va_arg(args, int);
        while (--field_width > 0)
          *str++ = ' ';
        continue;

      case 's':
        s = va_arg(args, char *);
        if (!s)
          s = "<NULL>";
        len = strnlen(s, precision);
        if (!(flags & LEFT))
          while (len < field_width--)
            *str++ = ' ';
        for (i = 0; i < len; ++i)
          *str++ = *s++;
        while (len < field_width--)
          *str++ = ' ';
        continue;

      case 'p':
        if (field_width == -1) {
          field_width = 2 * sizeof(void *);
          flags |= ZEROPAD;
        }
        str = number(str, (unsigned long)va_arg(args, void *), 16, field_width,
                     precision, flags);
        continue;

      case 'A':
        flags |= UPPERCASE;

      case 'a':
        if (qualifier == 'l')
          str = eaddr(str, va_arg(args, unsigned char *), field_width,
                      precision, flags);
        else
          str = iaddr(str, va_arg(args, unsigned char *), field_width,
                      precision, flags);
        continue;

      // Integer number formats - set up the flags and "break"
      case 'o':
        base = 8;
        break;

      case 'X':
        flags |= UPPERCASE;

      case 'x':
        base = 16;
        break;

      case 'd':
      case 'i':
        flags |= SIGN;

      case 'u':
        break;

#ifdef HAS_FLOAT

      case 'f':
        str = flt(str, va_arg(args, double), field_width, precision, *fmt,
                  flags | SIGN);
        continue;

#endif

      default:
        if (*fmt != '%')
          *str++ = '%';
        if (*fmt)
          *str++ = *fmt;
        else
          --fmt;
        continue;
    }

    if (qualifier == 'l')
      num = va_arg(args, unsigned long);
    else if (flags & SIGN)
      num = va_arg(args, int);
    else
      num = va_arg(args, unsigned int);

    str = number(str, num, base, field_width, precision, flags);
  }

  *str = '\0';


//rt_hw_interrupt_enable(level);


  return str - buf;
}

volatile int ttt;


#include "my_typedef.h"
int ee_open = 1;

#define mylen 1000
char my_buffer[mylen];
int my_idx = 0;

void my_putchar( char c )
{
    if( ee_open != 1 ) {
      return;
    }
    putchar( c );
}
void my_putchar_a( char c )
{
    if( ee_open == 0 ) {
      return;
    }
    putchar( c );
}
void my_show(void)
{
   char *p = my_buffer;
   while (*p) {
      putchar(*p);
      p++;
  }
    my_idx = 0;
}



int ee_printf(const char *fmt, ...)
{
  char buf[100], *p;
  va_list args;
  int n = 0;

  if( ee_open != 1 ) {
    return 0;
  }

//t_err_t result = rt_mutex_take(&mutex, RT_WAITING_FOREVER);
long level = hw_interrupt_disable();

  va_start(args, fmt);
  ee_vsprintf(buf, fmt, args);
  va_end(args);
  p = buf;
  while (*p) {

    putchar(*p);
    //my_putchar(*p);

    //for(int i = 0;i < 400; i++) {
    //    ttt += 1;
    //}

    n++;
    p++;
  }

hw_interrupt_enable(level);
//rt_mutex_release(&mutex);

  return n;
}

int ee_printf_a(const char *fmt, ...)
{
  char buf[100], *p;
  va_list args;
  int n = 0;

  if( ee_open == 0 ) {
    return 0;
  }

//rt_err_t result = rt_mutex_take(&mutex, RT_WAITING_FOREVER);
long level = hw_interrupt_disable();

  va_start(args, fmt);
  ee_vsprintf(buf, fmt, args);
  va_end(args);
  p = buf;
  while (*p) {

    putchar(*p);
    //my_putchar(*p);

    //for(int i = 0;i < 100; i++) {
    //    ttt += 1;
    //}

    n++;
    p++;
  }

hw_interrupt_enable(level);
//rt_mutex_release(&mutex);

  return n;
}


int ee_printf_info(const char *fmt, ...)
{
  char buf[400], *p;
  va_list args;
  int n = 0;

  if( ee_open != 1 ) {
    return 0;
  }

//t_err_t result = rt_mutex_take(&mutex, RT_WAITING_FOREVER);
long level = hw_interrupt_disable();

  va_start(args, fmt);
  ee_vsprintf(buf, fmt, args);
  va_end(args);
  p = buf;
  while (*p) {

    putchar(*p);
    //my_putchar(*p);

    //for(int i = 0;i < 400; i++) {
    //    ttt += 1;
    //}

    n++;
    p++;
  }

hw_interrupt_enable(level);
//rt_mutex_release(&mutex);

  return n;
}