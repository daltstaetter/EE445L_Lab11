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
#include <stdlib.h>
#include "Output.h"
#include "Pll.h"
#include "lm3s1968.h"
#include "sysctl.h"
#include "Dac.h"
#include "LSM9DSO.h"
#include "fixed.h"
#include <math.h>

#define PORTBCS
#define PG2  (*(volatile unsigned long *)0x40026020)
#define MYWAY  // used to define GYRO is enabled

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

unsigned long ulDataTx[SIZE] = {0x00008F00,0x00000A000,0x0000A100,0x0000A200,0x0000A300,0x0000A400};
unsigned long ulConfigTx[SIZE+1] = {0x00008F00,0x000009200,0x0000A000,0x0000A400,0x0000A500,0x0000A600,0x0000A700};

unsigned long ulDataRx[SIZE];
unsigned long ulConfigRx[SIZE+1];

unsigned long DataX[100] = {0};
unsigned long DataY[100] = {0};
unsigned long DataZ[100] = {0};

int angleX = 0;

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

void PortG_Init()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	
	SYSCTL_RCGC2_R |= 0;
	SYSCTL_RCGC2_R |= 0;	
	
	GPIO_PORTG_DIR_R |= 0x05;
	GPIO_PORTG_DEN_R |= 0x05;
	GPIO_PORTG_AFSEL_R &= ~0x05;
	GPIO_PORTG_DATA_R |= 0x05;	
}

void PortB_Init()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	
	SYSCTL_RCGC2_R |= 0;
	SYSCTL_RCGC2_R |= 0;	
	
	GPIO_PORTB_DIR_R |= 0x05;
	GPIO_PORTB_DEN_R |= 0x05;
	GPIO_PORTB_AFSEL_R &= ~0x05;
	GPIO_PORTB_DATA_R = 0x00;	
}
	
int angler = 0;

int main(void)
{
	unsigned long angle1, angle2,angle3,angle4,angle5,angle6;
	unsigned long dt;
	int windUp;
	int swing;
	int followThrough;
	
	windUp = FALSE;
	swing = FALSE;
	followThrough = FALSE;
	DisableInterrupts();
	PLL_Init();
	
	
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ); // 50 MHz Clock
	
	Output_Init();
  Output_Color(15);	
  Delay(100000);           // delay ~1 sec at 12 MHz
	
#ifndef PORTBCS
	PortG_Init();
#else
	PortB_Init();
	PortG_Init();
#endif	
	
#ifdef MYWAY
//	 printf("GYRO%c",NEWLINE);
	//DAC_InitSSI1();
	SPI_Init();
	GetData(XM,7,ulConfigTx,ulConfigRx);

//	GetData(GYRO,6,ulDataTx,ulDataRx);
	begin(G_SCALE_245DPS,A_SCALE_4G,M_SCALE_4GS,G_ODR_760_BW_30,A_ODR_400,M_ODR_25);
	Delay(8000000);
	GetData(XM,1,ulDataTx,ulDataRx);
GetData(XM,7,ulConfigTx,ulConfigRx);
	Delay(25000000);
	printf("%c",NEWLINE);
//GetData(GYRO,1,ulDataTx,ulDataRx);
//	Delay(25000000);
//	printf("%c",NEWLINE);
	
	
	
	
	Output_Clear();
	//GetData(GYRO, 3, ulDataTx,ulDataRx);
	//GetData(GYRO, 1, &ulDataTx[1],&ulDataRx[1]);
	Delay(8000000);
//GetDataXYZ(GYRO);
	Delay(10000000);
	Output_Clear();
#else
	printf("XL/M%c",NEWLINE);
	begin(G_SCALE_245DPS,0x1,M_SCALE_4GS,G_ODR_760_BW_30,A_ODR_400,M_ODR_25);
	GetData(XM, 1, ulDataTx);
	Delay(15000000);
//	Output_Clear();
	GetDataXYZ(XM);
	Delay(20000000);
	//Output_Clear();

// it doesn't do anything with the compass stuff
//	begin(G_SCALE_500DPS,A_SCALE_8G,M_SCALE_4GS,G_ODR_95_BW_125,A_ODR_400,M_ODR_25);
	
	
//	//Ignore the gyro if our angular velocity does not meet our threshold
//  if (gyroRate >= rotationThreshold || gyroRate <= -rotationThreshold) 
//	{
//    currentAngle += gyroRate;
//  }
	
#endif
//angle1 = 0;
//angle2 = 0;
//angle3 = 0;
//angle4 = 0;
//angle5 = 0;
//angle6 = 0;
	while(1)
	{
		//#ifdef MYWAY
//GetDataXYZ(GYRO);
		//printf("%c",NEWLINE);
		//Delay(4820000); // with these settings the sampling is at 10 Hz
		
		//Output_Clear();
		//#else
		//printf("XL Data(in g's):%c",NEWLINE,NEWLINE);
		GetDataXYZ(XM);
		Delay(1);
		Output_Clear();
		
		if(XLangleZ < 2800) // Z-angle < 28.00 degrees
		{
			windUp = TRUE;
		}
		while(windUp == TRUE)
		{
			GetDataXYZ(XM);
			Delay(1);
			Output_Clear();
			
			if(XLangleZ > 4600)
			{    // Z-angle goes to 9000 then back down again
				swing = TRUE; // started the swing
			}
			while(swing == TRUE)
			{
				GetDataXYZ(XM);
				Delay(1);
				Output_Clear();
				
				if(XLangleZ < 6400) // Z-angle < 62.00 degrees
				{
					Output_Clear();
					printf("vectX: "); Fixed_sDecOut3(vectX); printf("%c",NEWLINE);
					printf("vectY: "); Fixed_sDecOut3(vectY); printf("%c",NEWLINE);
					printf("%cThis gets sent to the LCD module!%c",NEWLINE,NEWLINE);
					Delay(50000000);
					
					// reset for new turn;
					swing = FALSE;
					windUp = FALSE;
					break;
				}
				break;
			}
			break;
		}
		
		
		
		
//		angle1 = ((98*(angle1+(gy*125)/1000)+2*XLangleX+50)/100)%36000;
//		angle2 = ((98*(angle2+(gz*125)/1000)+2*XLangleX+50)/100)%36000;
//		angle3 = ((98*(angle3+(gx*125)/1000)+2*XLangleY+50)/100)%36000;
//		angle4 = ((98*(angle4+(gz*125)/1000)+2*XLangleY+50)/100)%36000;
//		angle5 = ((98*(angle5+(gx*125)/1000)+2*XLangleZ+50)/100)%36000;
//		angle6 = ((98*(angle6+(gy*125)/1000)+2*XLangleZ+50)/100)%36000;
//		
//		Fixed_uDecOut2(angle1);printf("%c",TAB);	Fixed_uDecOut2(angle2);printf("%c",NEWLINE);
//		Fixed_uDecOut2(angle3);printf("%c",TAB);	Fixed_uDecOut2(angle4);printf("%c",NEWLINE);
//		Fixed_uDecOut2(angle5);printf("%c",TAB);	Fixed_uDecOut2(angle6);printf("%c",NEWLINE);
//		
		
		//#endif
	}
	
/*  printf("Hello, world.");
//  printf("%c", NEWLINE);
//  Delay(4000000);           // delay ~1 sec at 12 MHz
//  Output_Color(8);
//  printf("A really long string should go to the next line.\r");
//  printf("Oxxx(:::::::::::::::>%c", NEWLINE);
//  Delay(4000000);           // delay ~1 sec at 12 MHz
//  Output_Color(15);
//  printf("Color Table:%c", NEWLINE);
//  Delay(4000000);           // delay ~1 sec at 12 MHz
//  Output_Color(8);
//  printf("<:::::::::::::::)xxxO%c", NEWLINE);
//  for(i=15; i>=1; i=i-2){
//    Delay(4000000);         // delay ~1 sec at 12 MHz
//    Output_Color(i);
//    printf("Color: %u%c", i, TAB);
//    Output_Color(i-1);
//    printf("Color: %u%c", i-1, NEWLINE);
//  }
//  Delay(4000000);           // delay ~1 sec at 12 MHz
//  Output_Clear();
*/
}


