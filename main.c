#include <msp430.h> 
#include <msp430g2102.h>
#include "drv8711.h"
/*
 * main.c
 */


#define SCLK		BIT5    // SCLK
#define SDATO		BIT6    // MISO
#define SDATI		BIT7    // MOSI


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//while(1);

	// Set DCO to 16MHz
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	// Set SMCLK to 2MHz
	BCSCTL2 |= DIVS_3;
	// ACLK = VLO
	BCSCTL3 |= LFXT1S_2;

	// setup spi

	// 1)
	////USICTL0 = USISWRST;

	// 2) configurer le chip select (voir quelle pin c'est)
	/*
	P2DIR |= CS;
	P2OUT &= ~CS;

	 */
	init_cs();
	//P1REN |= 0x20;
	//P1OUT =  0x20;
	P1SEL = 0xE0;


	// 3) 3-pin, 8-bit S
	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIMST + USIOE; // Port, SPI master, output enable
	//USICTL0  &= ~USILSB; //MSB first
	//USICTL1 |= USICKPH;
	//	USICTL1 &= ~USII2C;// on desactive le mode i2c


	USICKCTL = USIDIV_4 + USISSEL_2 + USICKPL;      // /16 SMCLK
	USICTL0 &= ~USISWRST;// release for operation
	//USICNT = 8;
	USICNT |= USI16B + USICNT4;
	volatile unsigned char data;
	volatile long i;
	for(i=0;i<100000;i++);


	/*
	 * P1.0 à P1.4 en entrée avec P1.4 pour validation
	 *
	 *
	 */
	   P1DIR &= ~ 0x1F;

	/*
	while(1)
	{
		P2OUT |= BIT6;
		//USISRL = 0x0C;
		//USISRH = 0x19;
		USICNT |= 16;

		USISR = 0x0C19;
		while(!(USICTL1&USIIFG));

		P2OUT &= ~BIT6;

		for(i = 0xFFF; i > 0; i--);
	}

	unsigned int data2= 0x0C19;
	 */

	/*
	//USISRL = 0x03;
	//USICNT = 8;
	while(1){
	//drv_init();
	test2(data2);
	//ReadAllRegisters();

	}
	//drv_init();


	 */

	/*
	while (1);
	{
		// Update Functions

	}
	 */
	drv_init();
	WriteAllRegisters();//a tester


	//while(1){

		if(!(P1IN & BIT4)){
			updateStep();
		}




	/*
	 * 0011: 1/8 step
	   0100: 1/16 step
	   0110: 1/64 step
	   0111: 1/128 step
	   1000: 1/256 step
	 */



}

