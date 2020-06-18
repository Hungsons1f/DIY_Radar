#define SERVOPIN BIT2
#define SERVOMAX 2800
#define SERVOMIN 700
//#define HOLD 10000

void servoinit(int *unit, int *begin);
char servospinup (const int unit, int *now);
char servospindown (const int unit, int *now);

#define LED BIT0
#define TRIG BIT4
#define ECHO BIT5
#define PULSEDUTY 10
#define TIMEOUT 30000

void ledinit();
void seninit();
int senact();

#define SMCLK_F     1000000
#define BAUDRATE    9600
#define UART_RX_INT_EN  1

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char* s);
void uart_put_num (unsigned long val, char dec, unsigned char neg);
char uart_getc();
void uart_gets(char *s);
char uart_data_ready();
