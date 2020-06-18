#include "msp430.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

/*int main(void)
{
    uart_init();

    char str[100];
    char s[100];

    while(1)
    {
        uart_puts("hello world");
        uart_gets(s);
        uart_puts("\n\r");
        sprintf(str, "HELLO WORLD");
        uart_puts(str);
        uart_puts("\n\r");
        __delay_cycles(1000000);
    }

}*/

void uart_init(void)
{
    unsigned int tempfactor;
    P1SEL |= BIT1 + BIT2 ;                     // Set P1.1 as RXD
    P1SEL2 |= BIT1 + BIT2 ;                    // Set P1.2 as TXD

    UCA0CTL0 |= UCMODE_0;                      // Select UART mode
    UCA0CTL1 |= UCSSEL_2;                     // Select clock source USCI

    tempfactor = SMCLK_F/BAUDRATE;
    UCA0BR0 = (unsigned char) tempfactor&0x00FF;
    tempfactor >>= 8;
    UCA0BR1 = (unsigned char) (tempfactor&0x00FF);


    UCA0MCTL |= UCBRF_0 + UCBRS_0;                        // Configure modulator
    UCA0CTL1 &= ~UCSWRST;                     // **Release USCI to operate**
    if (UART_RX_INT_EN)
        IE2 |= UCA0RXIE;
}
void uart_putc(char c)
{
    while(!(IFG2&UCA0TXIFG));   // Wait until USCI_A0 TX buffer empty
        UCA0TXBUF = c;          // assign character to TX buffer
}


void uart_puts(const char* s)
{
    while(*s != '\0'){
        uart_putc(*s);
        s++;
    }
}


void uart_put_num (unsigned long val, char dec, unsigned char neg)
{
    char i, j, digit,k;
    long total;
    long temp;
    for (i = 0, total = val; total > 0;i++) // count number
        total /= 10;
        total = i;

    if (neg !=0 )
      uart_putc ('-');

    if ((total - dec) <=0)  // if total < dec put 0.xxx
        uart_putc('0');
    else
    {

      for (i=0; i< (total-dec); i++)
      {
        temp = 1;
        for (j=1;j<(total-i);j++)
            temp *=10;
        digit = (val/temp)%10;
        uart_putc(digit + 0x30);
      }
    }
    if (dec > 0)
    {
      uart_putc('.');
      if ( (dec - total)> 0)    // ex: val = 55; dec = 3 put 0.055
      {
        for ( i = 0; i < (dec-total);i++)
          uart_putc('0');
        k = 0;
      }
      else
        k = total - dec;
      for (i=k; i< total; i++)
      {
       temp = 1;
       for (j=1;j<(total-i);j++)
           temp *=10;
       digit = (val/temp)%10;
       uart_putc(digit + 0x30);
      }
    }
}



char uart_getc()
{
    while(!(IFG2&UCA0RXIFG));   // Wait until USCI_A0 RX receive a complete character
        return UCA0RXBUF; // assign RX buffer to function 's name
}


void uart_gets(char *s)
{
    *s = uart_getc();
    while(*s!='\0')
    {
        s++;
        *s = uart_getc();
    }
}


char uart_data_ready()
{
    if(UCA0RXIFG) return 1;
    else return 0;
}



#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
   //uart_putc(UCA0RXBUF); // echo the received character
   //
       //...
   //
}
