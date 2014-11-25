#include <msp430.h>
#include "irInfo.h"

//#define TRUE 1
//#define FALSE 0

//char enable = FALSE;
// stop function
void stop(void){
	// turn off the enables
//	enable = FALSE;
	P2OUT &= ~(BIT0 & BIT3);
}

// moveForward function
void moveForward(void){
//	enable = TRUE;
	// turn on the enables
	P2OUT |= BIT0 & BIT3;

	P2OUT |= BIT1;
//    TA1CCR1 = 0x0050;
	TA1CCTL1 = OUTMOD_3;

	P2OUT &= ~BIT5;
//    TA1CCR2 = 0x0050;
	TA1CCTL2 = OUTMOD_7;
}

// moveBackward function
void moveBackward(void){
	// turn on the enables
	P2OUT |= BIT0 & BIT3;

	P2OUT &= ~BIT1;
//    TA1CCR1 = 0x0050;
	TA1CCTL1 = OUTMOD_3;

	P2OUT |= BIT5;
//    TA1CCR2 = 0x0050;
	TA1CCTL2 = OUTMOD_7;
}

// turnLeft function
void turnLeft(unsigned int turnTime){
	do{
		P2DIR &= ~BIT2;
		TA1CCTL1 = OUTMOD_3;

		P2DIR |= BIT4;
		TA1CCTL2 = OUTMOD_7;

		turnTime--;
	}while(turnTime>0);
}

// turnRight function
void turnRight(unsigned int turnTime){
	do{
		P2DIR |= BIT2;
		TA1CCTL1 = OUTMOD_7;

		P2DIR &= ~BIT4;
		TA1CCTL2 = OUTMOD_3;

		turnTime--;
	}while(turnTime>0);
}


void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    // use P2.0 as the enable for left motor, start with it off
    P2DIR |= BIT0;
    P2OUT |= BIT0;

    // use P2.1 as direction for the left motor, 0 goes cw, 1 goes ccw
    P2DIR |= BIT1;
    P2OUT &= ~BIT1;

    // P2.2 as the pwm for the left motor
    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    // use P2.3 as the enable for right motor, start with it off
    P2DIR |= BIT3;
    P2OUT |= BIT3;

    // P2.4 as the pwm for the right motor
    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

    // use P2.5 as direction for the left motor, 0 goes cw, 1 goes ccw
    P2DIR |= BIT5;
    P2OUT &= ~BIT5;

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

    TA1CCR1 = 0x0050;
//    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 0x0050;
//    TA1CCTL2 = OUTMOD_3;					// set TACCTL2 to Set / Reset mode

    while (1) {
/*
    	while((P1IN & BIT3) != 0);			// Wait for a button press
    	while((P1IN & BIT3) == 0);			// and release

        TA1CCR1 = (TA1CCR1 + 0x010) & 0xFF;	// increase duty cycle
        TA1CCR2 = (TA1CCR2 + 0x010) & 0xFF;	// decrease duty cycle
*/
//    	moveForward();
    	moveBackward();
    } // end loop
} // end main
