/* This program blinks the red LED on the
* TI Tiva LaunchPad. The connections are:
* PF1 - red LED
* PF2 - blue LED
* PF3 - green LED
* They are high active (a '1' turns on the LED).
*/
#include "TM4C123GH6PM.h"
void delayMs(int n);
void delay(int n);
int main(void)
{
	int count =0;
	unsigned int state;
 // enable clock to GPIOF at clock gating control register
	SYSCTL->RCGCGPIO |= 0x20;
 // enable the GPIO pins for the LED (PF3, 2 1) as output
	
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR  = 0x01;
	GPIOF->PUR = 0x10;
	GPIOF->DIR = 0x0e;
	GPIOF->DEN = 0x1f;
	
	while(1)
	{
		if (count % 3 == 0)
			{
				GPIOF->DATA = 0x2;
				delay(500);
				GPIOF->DATA = 0; 
				delay(500);
			}

			else if (count % 3 == 1){
				GPIOF->DATA = 0x4;
				delay(500);
				GPIOF->DATA = 0; 
				delay(500);
			}
			else {
				GPIOF->DATA = 0x8;
				delay(500);
				GPIOF->DATA = 0; 
				delay(500);
			}
		state = GPIOF->DATA & 0x10;
		if (!state){	
			count++;
		}
	}
}


// delay in milliseconds (16 MHz CPU clock)
void delayMs(int n)
{
 int i, j;
 for(i = 0 ; i < n; i++)
 for(j = 0; j < 3180; j++)
 {} // do nothing for 1 ms
}


void delay(int n)
{
	SysTick->LOAD = 15999*n;
	SysTick->CTRL = 0x5; /*Enable the timer and choose sysclk */
	while((SysTick->CTRL & 0x10000) == 0) /*wait until the Count flag is set */
	{ }
	SysTick->CTRL = 0; /*Stop the timer (Enable = 0) */
}
