#include "msp430.h"
#include "header.h"

void main()
{
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    WDTCTL = WDTPW + WDTHOLD;

    int unit, now, distance;
    char s = 0;

    servoinit(&unit, &now);
    uart_init();
    ledinit();
    seninit();

    while (1)
    {
        switch (s)
        {
        case 0:
            uart_put_num(servospinup(unit, &now),0,0);
            uart_puts(",");
            if (now > SERVOMAX) s = 1;
            distance = senact();
            uart_put_num(distance,0,0);
            uart_puts("\n");
            break;
        case 1:
            uart_put_num(servospindown(unit, &now),0,0);
            uart_puts(",");
            if (now < SERVOMIN) s = 0;
            distance = senact();
            uart_put_num(distance,0,0);
            uart_puts("\n");
            break;
        }

    }

}
