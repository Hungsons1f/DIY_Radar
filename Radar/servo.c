#include  <msp430.h>

#define SERVOPIN BIT2
#define SERVOMAX 2800
#define SERVOMIN 700
#define HOLD 35000

/*int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    int unit, now;
    SERVOINIT(&unit,&now);

    int s = 0;
    while (1)
    {
        switch (s)
        {
        case 0:
            SERVOWSPIN(unit, &now);
            if (now > SERVOMAX) s = 1;
            break;
        case 1:
            SERVOCWSPIN(unit, &now);
            if (now < SERVOMIN) s = 0;
            break;
        }
    }
}
*/

void servoinit(int *unit, int *begin)
{
    P2DIR |= SERVOPIN;                          // P2.2/TA1.1 is used for PWM
    P2OUT &=~ SERVOPIN;
    P2SEL |= SERVOPIN;                          // P2.2 select TA1.1 option

    TA1CCR0 = 20000-1;                           // PWM Period TA1.1
    TA1CCR1 = SERVOMIN;                            // CCR1 PWM duty cycle
    TA1CCTL1 = OUTMOD_7;                       // CCR1 reset/set
    TA1CTL   = TASSEL_2 + MC_1;                // SMCLK, up mode

    *unit = (SERVOMAX - SERVOMIN)/180;
    *begin = SERVOMIN;
}

char servospinup (const int unit, int *now)
{
    //__delay_cycles(HOLD);
    char angle;
    *now +=  unit;
    angle = (char) ((*now - SERVOMIN)/unit);
    if (angle <0) angle = 0;
    if (angle > 180) angle = 180;
    TA1CCR1 = *now;
    return angle;
}

char servospindown (const int unit, int *now)
{
    //__delay_cycles(HOLD);
    char angle;
    *now -=  unit;
    angle = (char) ((*now - SERVOMIN)/unit);
    if (angle <0) angle = 0;
    if (angle > 180) angle = 180;
    TA1CCR1 = *now;
    return angle;
}
