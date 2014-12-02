/*
 * Author: Matt Bergstedt
 * Date: Nov 24, 2014
 * Description: Drives the robot in multiple directions and provides for turns
 * Documentation: I used code Dr. Coulston provided for initially testing the outputs to the motors
 */

#include <msp430.h>
//#include "irInfo.h"

#define FULL_TURN 450000

// for use with ir
/*
int32	irPacket = 0;
int8	newIrPacket = FALSE;
int16	packetData[48];
int8	packetIndex = 0;
*/

/*
 * stop function
 * Description: stops the robot
 */
void stop(void){
	// turn off the enables
	P2OUT &= ~(BIT0 | BIT3);
}

/*
* moveForward function
* Description: moves the robot to forward
*/
void moveForward(void){
	// turn on the enables
	P2OUT |= BIT0 | BIT3;

	// left motor ccw
	P2OUT |= BIT1;
	TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

	// right motor cw
	P2OUT &= ~BIT5;
	TA1CCTL2 = OUTMOD_3;					// set TACCTL2 to Set / Reset mode
}

/*
 * moveBackward function
 * Description: moves the robot to backwards
 */
void moveBackward(void){
	// turn on the enables
	P2OUT |= BIT0 | BIT3;

	// left motor cw
	P2OUT &= ~BIT1;
	TA1CCTL1 = OUTMOD_3;

	// right motor ccw
	P2OUT |= BIT5;
	TA1CCTL2 = OUTMOD_7;
}

/*
 * turnLeft function
 * Description: turns the robot to the left
 */
void turnLeft(char turnType){
	// turn on the enables
	P2OUT |= BIT0 | BIT3;

	// left motor cw
	P2OUT &= ~BIT1;
	TA1CCTL1 = OUTMOD_3;

	// right motor cw
	P2OUT &= ~BIT5;
	TA1CCTL2 = OUTMOD_3;

	// long turn as 1, short turn as 0
	if(turnType){
		__delay_cycles(FULL_TURN);
	}
	else{
		__delay_cycles(FULL_TURN/2);
	}
}

/*
 * turnRight function
 * Description: turns the robot to the right
 */
void turnRight(char turnType){
	// turn on the enables
	P2OUT |= BIT0 | BIT3;

	// left motor ccw
	P2OUT |= BIT1;
	TA1CCTL1 = OUTMOD_7;

	// right motor ccw
	P2OUT |= BIT5;
	TA1CCTL2 = OUTMOD_7;

	// long turn as 1, short turn as 0
	if(turnType){
		__delay_cycles(FULL_TURN);
	}
	else{
		__delay_cycles(FULL_TURN/2);
	}
}

/*
 * main.c
 * Description: sets up the ports for use with the motor driver chip, and runs through examples of
 * 			each of the movement functions
 */
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

    // way coded, moves faster as number gets smaller
    TA1CCR1 = 0x0050;
    TA1CCR2 = 0x0050;

    /*  for with ir
    initMSP430();				// Setup MSP to process IR and buttons

    while(1)  {
    	if (packetIndex > 33) {
    		packetIndex = 0;
    	} // end if new IR packet arrived
    	if(newIrPacket){
    		if(irPacket==UP){
    			moveForward();
    		}
    		else if(irPacket==DOWN){
    			moveBackward();
    		}
    		else if(irPacket==RIGHT || irPacket==LEFT){
    			stop();
    		}
    		else if(irPacket==CH_UP){
    			turnRight(1);
    		}
    		else if(irPacket==CH_DW){
    			turnRight(0);
    		}
    		else if(irPacket==VOL_UP){
    			turnLeft(1);
    		}
    		else if(irPacket==VOL_DW){
    			turnLeft(0);
    		}
    		newIrPacket = FALSE;
    	}
    } // end infinite loop
    */
    while (1) {
    	// run through functions to show functionality
    	moveForward();
    	__delay_cycles(3000000);
    	stop();
    	__delay_cycles(3000000);
    	moveBackward();
    	__delay_cycles(3000000);
    	stop();
    	__delay_cycles(3000000);
    	turnLeft(1);
    	stop();
    	__delay_cycles(3000000);
    	turnRight(1);
    	stop();
    	__delay_cycles(3000000);
    	turnLeft(0);
    	stop();
    	__delay_cycles(3000000);
    	turnRight(0);
    	stop();
    	__delay_cycles(3000000);
    } // end loop
} // end main

/*  for with ir
// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

	TA0CCR0 = 0x4000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;		// Use 1:1 presclar off MCLK and enable interrupts

	__enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;

			if(pulseDuration<maxLogic0Pulse && pulseDuration>minLogic0Pulse)
			{
				irPacket=(irPacket<<1)|0;
			}
			if(pulseDuration<maxLogic1Pulse && pulseDuration>minLogic1Pulse)
			{
				irPacket=(irPacket<<1)|1;
			}

			packetData[packetIndex++] = pulseDuration;
			TACTL = 0;
			LOW_2_HIGH; 				// Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			TACTL &= ~TAIFG;
			TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;
			HIGH_2_LOW; 						// Setup pin interrupr on positive edge
			break;
	} // end switch

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

//	TACTL &= ~TAIE;
	packetIndex = 0;
	newIrPacket = TRUE;
	TACTL &= ~TAIFG;
}
*/
