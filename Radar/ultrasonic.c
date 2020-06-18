#include <msp430.h>

#define LED BIT0
#define TRIG BIT4
#define ECHO BIT5
#define PULSEDUTY 10
#define TIMEOUT 30000

int miliseconds;
int distance;
long sensor;

/*void main(void)
{
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;                     // submainclock 1mhz
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  seninit();
  ledinit();
  while(1){
     senact();
 }
}*/

void ledinit()
{
    P1SEL &=~ LED;
    P1SEL2 &=~ LED;
    P1DIR |= LED;                            // P1.0 as output for LED
    P1OUT &= ~LED;
}

void seninit()
{
    CCTL0 = CCIE;                             // CCR0 interrupt enabled
    CCR0 = 1000;
    TACTL = TASSEL_2 + MC_1;
    P1IFG  &=~ ECHO;
    _BIS_SR(GIE);
}

int senact()
{
    P1DIR |= TRIG;          // trigger pin 1.4 as output
    P1OUT |= TRIG;
    __delay_cycles(PULSEDUTY);             // for 10us
    P1OUT &= ~TRIG;

    P1DIR &= ~ECHO;         // make pin P1.5 input (ECHO)
    P1IFG &=~ ECHO ;
    P1IE |= ECHO;
    P1IES &= ~ECHO;
    __delay_cycles(TIMEOUT);

    distance = sensor/58;           // converting ECHO lenght into cm
    if(distance < 20 && distance != 0) P1OUT |= LED;  //turning LED on if distance is less than 20cm and if distance isn't 0.
    else P1OUT &= ~LED;

    return distance;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG&ECHO)  //is there interrupt pending?
        {
          if(!(P1IES&ECHO)) // is this the rising edge?
          {
            TACTL|=TACLR;
            miliseconds = 0;
            P1IES |= ECHO;
          }
          else
          {
            sensor = (long)miliseconds*1000 + (long)TAR;

          }
    P1IFG &= ~ECHO;
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  miliseconds++;
}



