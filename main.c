/* This program blinks the red LED on the
* TI Tiva LaunchPad. The connections are:
* PF1 - red LED
* PF2 - blue LED
* PF3 - green LED
* They are high active (a '1' turns on the LED).
*/
#include "TM4C123GH6PM.h"

#define RED 0x2
#define BLUE 0x4
#define GREEN 0x8

void delayMs(int n);
void delay(int n);

int main(void)
{
	int count = 0;
	int toggle = 0;
	int change_couter_sw1 = 0;
	int change_couter_sw2 = 0;
	unsigned int state_sw1;
	unsigned int state_sw2;
	int power = 0;
	
	SYSCTL->RCGCGPIO |= 0x20;
	GPIOF->LOCK = 0x4C4F434B;
	GPIOF->CR  = 0x01;
	GPIOF->PUR = 0x11;
	GPIOF->DIR = 0x0e;
	GPIOF->DEN = 0x1f;
	
	while(1)
	{
		if (power%2 == 0)
		{
			if (count % 3 == 0)
			{
				if(toggle%4 == 0 || toggle%4 == 1)
				{
					GPIOF->DATA = RED;
					delay(250);
					toggle++;
				}
				if(toggle%4 == 2 || toggle%4 == 3)
				{
					GPIOF->DATA = 0; 
					delay(250);
					toggle++;
				}
			}

			else if (count % 3 == 1)
			{
				if(toggle%4 == 0 || toggle%4 == 1)
				{
					GPIOF->DATA = BLUE;
					delay(250);
					toggle++;
				}
				if(toggle%4 == 2 || toggle%4 == 3)
				{
					GPIOF->DATA = 0; 
					delay(250);
					toggle++;
				}
			}
			else 
			{
				if(toggle%4 == 0 || toggle%4 == 1)
				{
					GPIOF->DATA = GREEN;
					delay(250);
					toggle++;
				}
				if(toggle%4 == 2 || toggle%4 == 3)
				{
					GPIOF->DATA = 0; 
					delay(250);
					toggle++;
				}
			}
		}
		else
		{
			delay(250);
		}
			
		state_sw1 = GPIOF->DATA & 0x01;
		if (!state_sw1){	
			
			change_couter_sw1++;
			
			if (change_couter_sw1%8 == 0){
				count++;
			}
		}
		
		state_sw2 = GPIOF->DATA & 0x10;
		if (!state_sw2){	
			
			change_couter_sw2++;
			
			if (change_couter_sw2%8 == 0){
				power++;
				if (power%2 == 1)
				{
					GPIOF->DATA = 0;
					toggle = 0;
				}
			}
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
