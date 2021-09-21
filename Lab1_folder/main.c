#include "TM4C123GH6PM.h"

// Defining the bits responsible for LED color in GPIOF
#define RED 0x2
#define BLUE 0x4
#define GREEN 0x8

// 1 second
#define TIME_PERIOD 1000
#define INTERVAL 250
#define TIME_DIVIDER 4

// Old function for delay
void delayMs(int n);

// New improved and implemented function for delay
void delay(int n);

// Blink responsible for the blinking of LEDs every 500ms
int blink(int toggle, int color);

int main(void)
{
	// LED
	// 0 -> RED
	// 1 -> BLUE
	// 2 -> GREEN
	int led = 0;
	
	// Used to change the state of LED without pausing the blinking process
	int toggle = 0;
	
	// Counter used to check for a sustained change in the state of
	// sw1 for more than 2 seconds
	int change_couter_sw1 = 0;
	
	// Counter used to check for a sustained change in the state of
	// sw2 for more than 2 seconds
	int change_couter_sw2 = 0;
	
	// To record the state of sw1
	unsigned int state_sw1;
	
	// To record the state of sw1
	unsigned int state_sw2;
	
	// To check whether the blinking should be on or off
	// 0 -> OFF
	// 1 -> ON
	int power = 0;
	
	// Enable clock to GPIOF
	SYSCTL->RCGCGPIO |= 0x20;
	
	// Unlock GPIOCR register
	GPIOF->LOCK = 0x4C4F434B;
	
	// Enable GPIOPUR register to commit
	GPIOF->CR  = 0x01;
	
	// Enable Pull-up resistor for PF0 and PF4
	GPIOF->PUR = 0x11;
	
	// Set PF1, PF2 and PF3 as output
	// And set PF0 and PF4 as input
	GPIOF->DIR = 0x0e;
	
	// Enable all pins as digital GPIO pins
	GPIOF->DEN = 0x1f;
	
	while(1)
	{
		if (power%2 == 0)
		{
			if (led % 3 == 0)
			{
				toggle = blink(toggle, RED);
			}

			else if (led % 3 == 1)
			{
				toggle = blink(toggle, BLUE);
			}
			else 
			{
				toggle = blink(toggle, GREEN);
			}
		}
		else
		{
			delay(250);
		}
			
		state_sw1 = GPIOF->DATA & 0x01;
		if (!state_sw1)
		{		
			change_couter_sw1++;
			if (change_couter_sw1%8 == 0)
			{
				led++;
				toggle = 0;
			}
		}
		else 
		{
			change_couter_sw1 = 0;
		}
		
		state_sw2 = GPIOF->DATA & 0x10;
		if (!state_sw2)
		{	
			change_couter_sw2++;
			if (change_couter_sw2%8 == 0)
			{
				power++;
				if (power%2 == 1)
				{
					GPIOF->DATA = 0;
					toggle = 0;
				}
			}
		}
		else 
		{
			change_couter_sw2 = 0;
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
	/*Enable the timer and choose sysclk */
	SysTick->CTRL = 0x5; 
	/*wait until the Count flag is set */
	while((SysTick->CTRL & 0x10000) == 0) 
	{ }
	/*Stop the timer (Enable = 0) */
	SysTick->CTRL = 0; 
}

int blink(int toggle, int color)
{
	if(toggle%4  == 0 || toggle%4 == 1)
	{
		GPIOF->DATA = color;
		delay(INTERVAL);
		toggle++;
	}
	else if(toggle%4 == 2 || toggle%4 == 3)
	{
		GPIOF->DATA = 0; 
		delay(INTERVAL);
		toggle++;
	}
	return toggle;
}
