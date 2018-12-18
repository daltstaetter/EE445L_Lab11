// OLEDTestMain.c
// Runs on LM3S1968
// Test Output.c by sending various characters and strings to
// the OLED display and verifying that the output is correct.
// Daniel Valvano
// July 28, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Section 3.4.5

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdio.h>
#include "driverlib/gpio.h"
#include "Output.h"
#include "Pll.h"
#include "lm3s1968.h"
#include "sysctl.h"
#include "DAC.h"
#include "Ball.h"


char Map [84][6]; 

//********Ball Image****************
const char Ball [] = {
0xF0, 0xF0, 0xF0, 0xF0
};

//********White Space***************
const char ClearBall [] = {
0x00, 0x00, 0x00, 0x00
};





void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

unsigned char array[10];

int ballPosX = 75, ballPosY = 4; //starting location for the ball

// delay function for testing from sysctl.c
// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(unsigned long ulCount){
	__asm (	"    subs    r0, #1\n"
			"    bne     Delay\n"
			"    bx      lr\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

#endif
	
void PortG_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	
	delay = SYSCTL_RCGC2_R;
	
	GPIO_PORTG_DIR_R |= GPIO_PIN_2;
	GPIO_PORTG_DEN_R |= GPIO_PIN_2;
	GPIO_PORTG_AFSEL_R &= ~GPIO_PIN_2;
	GPIO_PORTG_DATA_R |= GPIO_PIN_2;	
	Delay(100);
	GPIO_PORTG_DATA_R &= ~GPIO_PIN_2;
}

int main(void)
{
	int level = 0, done = 0;
	int x, y; //calculated swin values
	int i, j, k = 0;
	//unsigned short count = 0;
	DisableInterrupts();
	PLL_Init();
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ); // 50 MHz Clock
	PortG_Init();	
 // Output_Init();
 // Output_Color(15);	
 // printf("Hello, world.");
 // printf("%c", NEWLINE);
 // Delay(4000000);           // delay ~1 sec at 12 MHz
	
	Nokia_LCD_InitSSI1();
	Delay(10000000);

  //****************LCD levels**************************************************
  //****************************************************************************
  //****************************************************************************
  //I'll put this into it's own file later
  while(level <= 1) {
		
		//initialize starting position
		ballPosX = 75;
		ballPosY = 4;
		done = 0;
		
		//intialize map
		for(i = 0; i < 6; i++) {
			for(j = 0; j < 84; j++) {
				Map[j][i] = 0;
			}
		}
		
		//ball starting position
		Map[75][4] = Map[75][4] | 0xF0;
		Map[74][4] = Map[74][4] | 0xF0;
		Map[73][4] = Map[73][4] | 0xF0;
		Map[72][4] = Map[72][4] | 0xF0;
		Nokia5110_DrawMap(Map, level);
		//Nokia5110_DrawBall(ballPosX, ballPosY, Ball, Map);
		
		
		while(done == 0) {
			done = MoveBall(x, y, &ballPosX, &ballPosY, &level, Map); 	//x and y are the balls vector components, ballPosx and 
																																	//ballPoxY are globals holding balls position

		}
	}

	Nokia5110_DrawMap(Map, level);
	while(1) {}
  //***********end of level code****************************************************
  //********************************************************************************
  //********************************************************************************



	
	while(1)
	{
		GPIO_PORTG_DATA_R ^= GPIO_PIN_2;	
		Delay(4000000);
	}
	

}

