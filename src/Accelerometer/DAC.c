// DAC.c
extern unsigned char array[10];

#include <stdio.h>
#include "inc/hw_ssi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "DAC.h"
#include "lm3s1968.h"

extern void Delay(unsigned long ulCount);

unsigned long SSI1WriteByte(enum sensor type, unsigned char subAddress, unsigned char ulDataTx)
{
	unsigned int i;
	unsigned long ulDataRx;	
	unsigned long TxBytes = 0;
	TxBytes |= (0x00FF & ulDataTx);
	TxBytes &= SINGLEWRITE; // sets 2 MSbits to 0
	TxBytes |= ((0x003F & subAddress) << 8); // shift 6-bit address bits into place, i.e. bit13 to bit8;

	// Send the data using the "blocking" put function.  This function
	// will wait until there is room in the send FIFO before returning.
	// This allows you to assure that all the data you send makes it into
	// the send FIFO.
	//
	//if( type == GYRO)
	{
		GPIO_PORTG_DATA_R &= ~0x04;
	}
	//else
	{
		// pick the slave select for the XM
	}
	SSIDataPut(SSI1_BASE, ulDataTx);
	//
	// Wait until SSI1 is done transferring all the data in the transmit FIFO.
	//
	while(SSIBusy(SSI1_BASE))
	{
	}
	//
	// Receive the data using the "blocking" Get function. This function
	// will wait until there is data in the receive FIFO before returning.
	//
	SSIDataGet(SSI1_BASE, &ulDataRx);
	//
	// Since we are using 8-bit data, mask off the MSB that was read full
	// duplex while we were sending our command byte.
	// 
	ulDataRx &= 0x000000FF;
	
	//if( type == GYRO)
	{
		GPIO_PORTG_DATA_R |= 0x04;
	}
	//else
	{
		// pick the slave select for the XM
	}
	return ulDataRx;
}

void GetData1(void)
{
	unsigned int i;
	unsigned long ulDataRx[SIZE];
	//unsigned long ulDataTx[SIZE] = {0x00008F00,0x000009200,0x0000A000,0x0000A400,0x0000A500,0x0000A600};
	// ^^ XL important addresses
	//unsigned long ulDataTx[SIZE] = {0x0000A800,0x00000A900,0x0000AA00,0x0000AB00,0x0000AC00,0x0000AD00};
	// ^^ Gyro XYZ data addresses
	unsigned long ulDataTx[SIZE] = {0x00008F00,0x00000A000,0x0000A100,0x0000A200,0x0000A300,0x0000A400};
	// ^^ Gyro important addresses
	
	// clear debug Rx buffer
	for(i=0;i<SIZE;i++)
	{
		ulDataRx[i] = 0;
	}	

	// Read any residual data from the SSI port.  This makes sure the receive
	// FIFOs are empty, so we don't read any unwanted junk.  This is done here
	// because the SPI SSI mode is full-duplex, which allows you to send and
	// receive at the same time.  The SSIDataGetNonBlocking function returns
	// "true" when data was returned, and "false" when no data was returned.
	// The "non-blocking" function checks if there is any data in the receive
	// FIFO and does not "hang" if there isn't.
	//
	GPIO_PORTG_DATA_R &= ~0x04;
	while(SSIDataGetNonBlocking(SSI1_BASE, &ulDataRx[i]))
	{
	}
	GPIO_PORTG_DATA_R |= 0x04;
	Delay(10000);
	
	// Send the data using the "blocking" put function.  This function
	// will wait until there is room in the send FIFO before returning.
	// This allows you to assure that all the data you send makes it into
	// the send FIFO.
	//
	for(i=0;i<SIZE;i++)
	{
		
		// Send the data using the "blocking" put function.  This function
		// will wait until there is room in the send FIFO before returning.
		// This allows you to assure that all the data you send makes it into
		// the send FIFO.
		//
		GPIO_PORTG_DATA_R &= ~0x04;
		SSIDataPut(SSI1_BASE, ulDataTx[i]);
		//
		// Wait until SSI0 is done transferring all the data in the transmit FIFO.
		//
		while(SSIBusy(SSI1_BASE))
		{
		}
		//
		// Receive the data using the "blocking" Get function. This function
		// will wait until there is data in the receive FIFO before returning.
		//
		SSIDataGet(SSI1_BASE, &ulDataRx[i]);
		//
		// Since we are using 8-bit data, mask off the MSB that was read full
		// duplex while we were sending our command byte.
		// 
		ulDataRx[i] &= 0x000000FF;
		//
		// Display the data that SSI0 received.
		// The datasheet says this value should be 4.
		//
//		UARTprintf("Received: %x\n", ulDataRx);
		GPIO_PORTG_DATA_R |= 0x04;
		if(i < SIZE)
		{
			//array[i] = ulDataRx[i];
			printf("Tx: 0x%04X",(unsigned int)ulDataTx[i]); // prints 4 hex digits with leading zeros
			printf("%c", 0x9);
			printf("Rx: 0x%02X",(unsigned int)ulDataRx[i]); // prints 2 hex digits with leading zeros
			printf("%c", 0xD);
			Delay(4000000);           // delay ~1 sec at 12 MHz
		}
	}
}


void GetDataXYZ1(void)
{
	unsigned int i;
	unsigned long ulDataRx[SIZE];
	//unsigned long ulDataTx[SIZE] = {0x00008F00,0x000009200,0x0000A000,0x0000A400,0x0000A500,0x0000A600};
	unsigned long ulDataTx[SIZE] = {0x0000A800,0x00000A900,0x0000AA00,0x0000AB00,0x0000AC00,0x0000AD00};
	//unsigned long ulDataTx[SIZE] = {0x00008F00,0x00000A000,0x0000A100,0x0000A200,0x0000A300,0x0000A400};

	
	i = 0;
	for(i=0;i<SIZE;i++)
	{
		ulDataRx[i] = 0;
	}	

	// Read any residual data from the SSI port.  This makes sure the receive
	// FIFOs are empty, so we don't read any unwanted junk.  This is done here
	// because the SPI SSI mode is full-duplex, which allows you to send and
	// receive at the same time.  The SSIDataGetNonBlocking function returns
	// "true" when data was returned, and "false" when no data was returned.
	// The "non-blocking" function checks if there is any data in the receive
	// FIFO and does not "hang" if there isn't.
	//
	GPIO_PORTG_DATA_R &= ~0x04;
	while(SSIDataGetNonBlocking(SSI1_BASE, &ulDataRx[i]))
	{
	}
	GPIO_PORTG_DATA_R |= 0x04;
	Delay(10000);

	// Send the data using the "blocking" put function.  This function
	// will wait until there is room in the send FIFO before returning.
	// This allows you to assure that all the data you send makes it into
	// the send FIFO.
	//
	for(i=0;i<SIZE;i++)
	{
		//
		// Send the data using the "blocking" put function.  This function
		// will wait until there is room in the send FIFO before returning.
		// This allows you to assure that all the data you send makes it into
		// the send FIFO.
		//
		GPIO_PORTG_DATA_R &= ~0x04;
		SSIDataPut(SSI1_BASE, ulDataTx[i]);

		//
		// Wait until SSI1 is done transferring all the data in the transmit FIFO.
		//
		
		//ulDataRx[i] = SSI1WriteByte(GYRO,0x20,ulDataTx[i]);
		while(SSIBusy(SSI1_BASE))
		{
		}
		//
		// Receive the data using the "blocking" Get function. This function
		// will wait until there is data in the receive FIFO before returning.
		//
		SSIDataGet(SSI1_BASE, &ulDataRx[i]);
		//
		// Since we are using 8-bit data, mask off the MSB that was read full
		// duplex while we were sending our command byte.
		// 
		ulDataRx[i] &= 0x000000FF;
		//
		// Display the data that SSI1 received.
		// The datasheet says this value should be 4.
		//

		GPIO_PORTG_DATA_R |= 0x04;

		if(i < SIZE)
		{
			//array[i] = ulDataRx[i];
			printf("Tx: 0x%04X",(unsigned int)ulDataTx[i]); // prints 4 hex digits with leading zeros
			printf("%c", 0x9); // 0x9 == TAB character
			printf("Rx: 0x%02X",(unsigned int)ulDataRx[i]); // prints 2 hex digits with leading zeros
			printf("%c", 0xD); // 0xD == NEWLINE character
			Delay(4000000);           // delay ~1 sec at 12 MHz
		}
	}
}

void DAC_InitSSI1(void)
{
	//
	// Now set up the SPI port to talk to the LSM9DS0
  //
	
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  GPIOPinConfigure(GPIO_PE0_SSI1CLK);
  GPIOPinConfigure(GPIO_PE1_SSI1FSS);
  GPIOPinConfigure(GPIO_PE2_SSI1RX);
  GPIOPinConfigure(GPIO_PE3_SSI1TX);
  GPIOPinTypeSSI(GPIO_PORTE_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);
	
	//
  // LSM9DS0 SPI Specs
  // Max SPI Clock : 10 MHz
  // Data Order : MSB transmitted first
  // Clock Polarity: high when idle => SPO = 1;
  // Clock Phase : sample on rising edge => SPH = 1
  //
  // We read and write 8 bits to the LSM9DS0 but we need the Stellaris to drive the clock.  
	// We send 16 bits, MSB are specifiers(R/W,Inc,Address respectively), LSB is data (send MSbit 1st)
	// R/W - bit 15, Read = 1, Write = 0;
	// Inc - bit 14, (Auto Increment Address) Inc = 1
	// Address - bit 13 to bit 8, MOSI address to Tx data to
	// data - bit 7 to bit 0, data to store at said address
	// When we do a write, all 16 bits are written.
  // When we do a read, we take the command byte and shift left 8 bits, writing 0's as the last eight bits.
  // Since this mode is full duplex, we'll get 16 bits back but mask off the top 8, leaving only the read data we are
  
  SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 5000000, 16);
  SSIEnable(SSI1_BASE);
}

// from the book, Sec. 7-5 pg 371
void DAC_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // activate PortE
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI1;  // activate SSI1
		
	delay = SYSCTL_RCGC2_R;  // allow time to finish activating
	delay = SYSCTL_RCGC2_R;  // allow time to finish activating
	
	GPIO_PORTE_DIR_R = 0x0B;    // PE0,1,3 outputs; PE2 input
	GPIO_PORTE_AFSEL_R |= 0x0F; // enable alt func on PE0-3
	GPIO_PORTE_DEN_R |= 0x0F;   // enable digital IO on PA2,3,5
	
	SSI1_CR1_R &= ~SSI_CR1_SSE; // disable SSI
	SSI1_CR1_R &= ~SSI_CR1_MS;  // master mode
	
	//SSI1_CPSR_R = (SSI0_CPSR_R & ~SSI_CPSR_CPSDVSR_M) + 2; // 3MHz
	SSI1_CPSR_R = 5; // f_ssi = 10 MHz with SCR=0
	SSI1_CR0_R &= ~(SSI_CR0_SCR_M |   // SCR = 0;
									SSI_CR0_SPH   |   // SPH = 0; latch data on rising edge of CLK
									SSI_CR0_SPO);     // SPO = 0; I change this in the next line of code
	
	SSI1_CR0_R |= SSI_CR0_SPO;        // SPO = 1; start transmission on falling edge of CLK
	
	
	SSI1_CR0_R = (SSI1_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO; // Freescale
	SSI1_CR0_R = (SSI1_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_16; // 16-bit data
	SSI1_CR1_R |= SSI_CR1_SSE; // enable SSI
}


void DAC_Init0(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate PortE
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;  // activate SSI1
		
	delay = SYSCTL_RCGC2_R;  // allow time to finish activating
	delay = SYSCTL_RCGC2_R;  // allow time to finish activating
	
	GPIO_PORTA_DIR_R = 0x2C;    // PE0,1,3 outputs; PE2 input
	GPIO_PORTA_AFSEL_R |= 0x3C; // enable alt func on PE0-3
	GPIO_PORTA_DEN_R |= 0x3C;   // enable digital IO on PA2,3,5
	
	SSI0_CR1_R &= ~SSI_CR1_SSE; // disable SSI
	SSI0_CR1_R &= ~SSI_CR1_MS;  // master mode
	
	//SSI0_CPSR_R = (SSI0_CPSR_R & ~SSI_CPSR_CPSDVSR_M) + 2; // 3MHz
	SSI0_CPSR_R = 10; // f_ssi = 10 MHz with SCR=0
	SSI0_CR0_R &= ~(SSI_CR0_SCR_M |   // SCR = 0;
									SSI_CR0_SPH   |   // SPH = 0; latch data on rising edge of CLK
									SSI_CR0_SPO);     // SPO = 0; I change this in the next line of code
	
	SSI0_CR0_R |= SSI_CR0_SPO;        // SPO = 1; start transmission on falling edge of CLK
	SSI0_CR0_R |= SSI_CR0_SPH;        // latch data on rising clock edge

	SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO; // Freescale
	SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_16; // 16-bit data
	SSI0_CR1_R |= SSI_CR1_SSE; // enable SSI
	
	return;
}



// from the book, Sec. 7-5 pg 372
// send the 16-bit data to the SSI, return a reply
void DAC_OutSSI1(unsigned char data, unsigned char subAddress, unsigned char csPin)
{	
	unsigned short TxBytes = 0;
	TxBytes |= (0x00FF & data);
	TxBytes &= SINGLEWRITE; // sets 2 MSB to 0
	TxBytes |= ((0x003F & subAddress) << 8); // shift 6-bit address bits into place, i.e. bit13 to bit8;
	
	// first write the address then the data
	/////////////////////////////////////
	// I need to add a specifier for the chip select bit
	// to differentiate among gyro, accel, & magnetometer
	/////////////////////////////////////
	GPIO_PORTG_DATA_R &= ~0x04;// Open communication
	// write the MS_bit first
	// If write, bit 0 (MS) should be 0
	// If single write, bit 1 should be 0
	while(SSI1_SR_TNF == 0) 
	{}; // wait until room in FIFO
	SSI1_DR_R = TxBytes;  // data out
	Delay(60); // for some reason it needs this delay of >= 48, it doesn't get
						 // stored in the XL/G if not
	GPIO_PORTG_DATA_R |= 0x04; // Close communication
}

void DAC_Out0(unsigned char data, unsigned char subAddress, unsigned char csPin)
{	
	unsigned short TxBytes = 0;
	TxBytes |= (0x00FF & data);
	TxBytes &= SINGLEWRITE; // sets 2 MSB to 0
	TxBytes |= ((0x003F & subAddress) << 8); // shift 6-bit address bits into place, i.e. bit13 to bit8;
	
	// first write the address then the data
	/////////////////////////////////////
	// I need to add a specifier for the chip select bit
	// to differentiate among gyro, accel, & magnetometer
	/////////////////////////////////////
	
	// digitalWrite(csPin,Low);
	
	// write the MSB first
	// If write, bit 0 (MSB) should be 0
	// If single write, bit 1 should be 0
	GPIO_PORTG_DATA_R &= ~0x04;
	while(SSI0_SR_TNF == 0) 
	{}; // wait until room in FIFO
	SSI0_DR_R = TxBytes;  // data out
	GPIO_PORTG_DATA_R |= 0x04;
//	digitalWrite(csPin, HIGH); // Close communication
}


unsigned char DAC_In(unsigned char subAddress, unsigned char csPin)
{	
	unsigned long ulDataRx;
	// I might need to change this to 8 bit data
	unsigned short RxBytes = 0;
	RxBytes |= ((0x3F & subAddress) << 8); // shift 6-bit address bits into place, i.e. bit13 to bit8;

	RxBytes |= READ; // set MSB to a read
	RxBytes &= SINGLE; // Single read bit14=0
	
	// digitalWrite(csPin,Low);
	GPIO_PORTG_DATA_R &= ~0x04;
	while(SSI1_SR_TNF == 0) 
	{};
	//SSI1_DR_R = RxBytes;  // choose address to read
	SSIDataPut(SSI1_BASE, RxBytes);
		//Delay(4);
//	while(SSI1_SR_RNE == 0) 
//	{};
	SSIDataGet(SSI1_BASE, &ulDataRx);	
		
	GPIO_PORTG_DATA_R |= 0x04;
	return (unsigned char) ulDataRx;	
}


unsigned char DAC_In0(unsigned char subAddress, unsigned char csPin)
{	
	unsigned long RxData;
	// I might need to change this to 8 bit data
	static unsigned short RxBytes = 0;
	RxBytes = 0;
	RxBytes |= ((0x3F & subAddress) << 8); // shift 6-bit address bits into place, i.e. bit13 to bit8;

	RxBytes |= READ; // set MSB to a read
	RxBytes &= SINGLE; // Single read bit14=0
	
	// digitalWrite(csPin,Low);
	GPIO_PORTG_DATA_R &= ~0x04;
	while(SSI0_SR_TNF == 0) 
	{};
	SSI0_DR_R = RxBytes;  // choose address to read
	while(SSI0_SR_RNE == 0) 
	{};
	RxData = SSI0_DR_R;
	GPIO_PORTG_DATA_R |= 0x04;
	return (unsigned char) RxData;	
}

