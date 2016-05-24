/*
 * drv8711.c
 *
 *  Created on: 23 mars 2016
 *      Author: ethel
 */

#include "drv8711.h"
#include <msp430.h>
#include <msp430g2102.h>
#include <stdint.h>

// DRV8711 Registers
struct CTRL_Register 	G_CTRL_REG;
struct TORQUE_Register 	G_TORQUE_REG;
struct OFF_Register 	G_OFF_REG;
struct BLANK_Register	G_BLANK_REG;
struct DECAY_Register 	G_DECAY_REG;
struct STALL_Register 	G_STALL_REG;
struct DRIVE_Register 	G_DRIVE_REG;
struct STATUS_Register 	G_STATUS_REG;

/*
 * demande d'ecriture : 0
 * demande de lecteure: 1
 */
#define REGWRITE    0x00
#define REGREAD     0x80
unsigned int G_WRITE_ADDR = 0;//sur 16 bits 0x000(0A1A2A3) //adresse sur 3 bits
unsigned int G_WRITE_DATA = 0;//sur 16 bits 0x0D1D2D3 (premier octet tjrs a 0,12 bits data)
#define CS			BIT6    // P2.3

/* on initialise toutes les valeurs des registres par defaut*/
void drv_init(){

	// Set Default Register Settings
	// CTRL Register
	G_CTRL_REG.Address 	= 0x00;//adresse du registre
	G_CTRL_REG.DTIME 	= 0x03;
	G_CTRL_REG.ISGAIN 	= 0x03;//40 current max 1.36A
	G_CTRL_REG.EXSTALL 	= 0x00;
	G_CTRL_REG.MODE 	= 0x03;
	G_CTRL_REG.RSTEP 	= 0x00;
	G_CTRL_REG.RDIR 	= 0x00;
	G_CTRL_REG.ENBL 	= 0x01;//motor enable

	// TORQUE Register
	G_TORQUE_REG.Address = 0x01;
	G_TORQUE_REG.SIMPLTH = 0x00;
	G_TORQUE_REG.TORQUE  = 0xBA;

	// OFF Register
	G_OFF_REG.Address 	= 0x02;
	G_OFF_REG.PWMMODE 	= 0x00;
	G_OFF_REG.TOFF 		= 0x30;

	// BLANK Register
	G_BLANK_REG.Address = 0x03;
	G_BLANK_REG.ABT 	= 0x01;
	G_BLANK_REG.TBLANK 	= 0x08;

	// DECAY Register.
	G_DECAY_REG.Address = 0x04;
	G_DECAY_REG.DECMOD  = 0x03;
	G_DECAY_REG.TDECAY 	= 0x10;

	// STALL Register
	G_STALL_REG.Address = 0x05;
	G_STALL_REG.VDIV 	= 0x03;
	G_STALL_REG.SDCNT 	= 0x03;
	G_STALL_REG.SDTHR 	= 0x40;

	// DRIVE Register
	G_DRIVE_REG.Address = 0x06;
	G_DRIVE_REG.IDRIVEP = 0x00;
	G_DRIVE_REG.IDRIVEN = 0x00;
	G_DRIVE_REG.TDRIVEP = 0x01;
	G_DRIVE_REG.TDRIVEN = 0x01;
	G_DRIVE_REG.OCPDEG 	= 0x01;
	G_DRIVE_REG.OCPTH 	= 0x01;

	// STATUS Register
	G_STATUS_REG.Address = 0x07;
	G_STATUS_REG.STDLAT  = 0x00;
	G_STATUS_REG.STD     = 0x00;
	G_STATUS_REG.UVLO    = 0x00;
	G_STATUS_REG.BPDF    = 0x00;
	G_STATUS_REG.APDF    = 0x00;
	G_STATUS_REG.BOCP    = 0x00;
	G_STATUS_REG.AOCP    = 0x00;
	G_STATUS_REG.OTS     = 0x00;
	WriteAllRegisters();
}


unsigned int SPI_DRV8711_ReadWrite(unsigned char dataHi, unsigned char dataLo)
{
	unsigned int readData = 0;
	P2OUT |= CS;

	USISR = dataHi;
	USICNT = 8;
	while (!(USICTL1&USIIFG));
	readData |= (USISR << 8);


	USISR  = dataLo;
	USICNT = 8;
	while (!(USICTL1&USIIFG));
	readData |= (USISR << 8);

	P2OUT &= ~CS;
	volatile int i;
	for(i=0;i<100;i++);

	return readData;
}


void test2(unsigned int data)
{
	P2OUT |= CS;

	USISR = data;
	USICNT = 16;
	while (!(USICTL1&USIIFG));


	P2OUT &= ~CS;

}



void WriteAllRegisters()
{
	unsigned char dataHi = 0x00;
	unsigned char dataLo = 0x00;

	// Write CTRL Register
	dataHi = REGWRITE | (G_CTRL_REG.Address << 4) | (G_CTRL_REG.DTIME << 2) | (G_CTRL_REG.ISGAIN);
	dataLo = (G_CTRL_REG.EXSTALL << 7) | (G_CTRL_REG.MODE << 3) | (G_CTRL_REG.RSTEP << 2) | (G_CTRL_REG.RDIR << 1) | (G_CTRL_REG.ENBL);
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	// Write TORQUE Register
	dataHi = REGWRITE | (G_TORQUE_REG.Address << 4) | (G_TORQUE_REG.SIMPLTH);
	dataLo = G_TORQUE_REG.TORQUE;
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	// Write OFF Register
	dataHi = REGWRITE | (G_OFF_REG.Address << 4) | (G_OFF_REG.PWMMODE);
	dataLo = G_OFF_REG.TOFF;
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	// Write BLANK Register
	dataHi = REGWRITE | (G_BLANK_REG.Address << 4) | (G_BLANK_REG.ABT);
	dataLo = G_BLANK_REG.TBLANK;
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	/// Write DECAY Register
	dataHi = REGWRITE | (G_DECAY_REG.Address << 4) | (G_DECAY_REG.DECMOD);
	dataLo = G_DECAY_REG.TDECAY;
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	// Write STALL Register
	dataHi = REGWRITE | (G_STALL_REG.Address << 4) | (G_STALL_REG.VDIV << 2) | (G_STALL_REG.SDCNT);
	dataLo = G_STALL_REG.SDTHR;
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	// Write DRIVE Register
	dataHi = REGWRITE | (G_DRIVE_REG.Address << 4) | (G_DRIVE_REG.IDRIVEP << 2) | (G_DRIVE_REG.IDRIVEN);
	dataLo = (G_DRIVE_REG.TDRIVEP << 6) | (G_DRIVE_REG.TDRIVEN << 4) | (G_DRIVE_REG.OCPDEG << 2) | (G_DRIVE_REG.OCPTH);
	SPI_DRV8711_ReadWrite(dataHi, dataLo);

	// Write STATUS Register
	dataHi = REGWRITE | (G_STATUS_REG.Address << 4);
	dataLo = (G_STATUS_REG.STDLAT << 7) | (G_STATUS_REG.STD << 6) | (G_STATUS_REG.UVLO << 5) | (G_STATUS_REG.BPDF << 4) | (G_STATUS_REG.APDF << 3) | (G_STATUS_REG.BOCP << 2) | (G_STATUS_REG.AOCP << 1) | (G_STATUS_REG.OTS);
	SPI_DRV8711_ReadWrite(dataHi, dataLo);
}

void ReadAllRegisters()
{
	unsigned char dataHi = 0x00;
	const unsigned char dataLo = 0x00;
	unsigned int readData = 0x00;

	// Read CTRL Register
	dataHi = REGREAD | (G_CTRL_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	/*G_CTRL_REG.DTIME        = ((readData >> 10) & 0x0003);
	G_CTRL_REG.ISGAIN       = ((readData >> 8) & 0x0003);
	G_CTRL_REG.EXSTALL      = ((readData >> 7) & 0x0001);
	G_CTRL_REG.MODE         = ((readData >> 3) & 0x000F);
	G_CTRL_REG.RSTEP        = ((readData >> 2) & 0x0001);
	G_CTRL_REG.RDIR         = ((readData >> 1) & 0x0001);
	 */
	G_CTRL_REG.ENBL         = ((readData >> 0) & 0x0001);

	/*
	// Read TORQUE Register
	dataHi = REGREAD | (G_TORQUE_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_TORQUE_REG.SIMPLTH    = ((readData >> 8) & 0x0007);
	G_TORQUE_REG.TORQUE     = ((readData >> 0) & 0x00FF);

	// Read OFF Register
	dataHi = REGREAD | (G_OFF_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_OFF_REG.PWMMODE       = ((readData >> 8) & 0x0001);
	G_OFF_REG.TOFF          = ((readData >> 0) & 0x00FF);

	// Read BLANK Register
	dataHi = REGREAD | (G_BLANK_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_BLANK_REG.ABT         = ((readData >> 8) & 0x0001);
	G_BLANK_REG.TBLANK      = ((readData >> 0) & 0x00FF);

	// Read DECAY Register
	dataHi = REGREAD | (G_DECAY_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_DECAY_REG.DECMOD      = ((readData >> 8) & 0x0007);
	G_DECAY_REG.TDECAY      = ((readData >> 0) & 0x00FF);

	// Read STALL Register
	dataHi = REGREAD | (G_STALL_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_STALL_REG.VDIV        = ((readData >> 10) & 0x0003);
	G_STALL_REG.SDCNT       = ((readData >> 8) & 0x0003);
	G_STALL_REG.SDTHR       = ((readData >> 0) & 0x00FF);

	// Read DRIVE Register
	dataHi = REGREAD | (G_DRIVE_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_DRIVE_REG.IDRIVEP     = ((readData >> 10) & 0x0003);
	G_DRIVE_REG.IDRIVEN     = ((readData >> 8) & 0x0003);
	G_DRIVE_REG.TDRIVEP     = ((readData >> 6) & 0x0003);
	G_DRIVE_REG.TDRIVEN     = ((readData >> 4) & 0x0003);
	G_DRIVE_REG.OCPDEG      = ((readData >> 2) & 0x0003);
	G_DRIVE_REG.OCPTH       = ((readData >> 0) & 0x0003);

	// Read STATUS Register
	dataHi = REGREAD | (G_STATUS_REG.Address << 4);
	readData = SPI_DRV8711_ReadWrite(dataHi, dataLo);
	G_STATUS_REG.STDLAT     = ((readData >> 7) & 0x0001);
	G_STATUS_REG.STD        = ((readData >> 6) & 0x0001);
	G_STATUS_REG.UVLO       = ((readData >> 5) & 0x0001);
	G_STATUS_REG.BPDF       = ((readData >> 4) & 0x0001);
	G_STATUS_REG.APDF       = ((readData >> 3) & 0x0001);
	G_STATUS_REG.BOCP       = ((readData >> 2) & 0x0001);
	G_STATUS_REG.AOCP       = ((readData >> 1) & 0x0001);
	G_STATUS_REG.OTS        = ((readData >> 0) & 0x0001);
	 */
}

void init_cs(){
	P2DIR |= CS;
	P2SEL=0;
	P2OUT &= ~CS;
}

void updateStep(){
	if(!(P1IN & BIT0)){
		G_CTRL_REG.MODE 	= 0x04;//1/16 step

	}
	if(!(P1IN & BIT1)){
		G_CTRL_REG.MODE 	= 0x06;//1/64 step

	}
	if(!(P1IN & BIT2)){
		G_CTRL_REG.MODE 	= 0x07;//1/128 step

	}
	if(!(P1IN & BIT3)){
		G_CTRL_REG.MODE 	= 0x00;//full step

	}
	WriteAllRegisters();
}






void test(){
	unsigned char dataHi = 0x00;
	unsigned char dataLo = 0x00;
	G_STATUS_REG.Address = 1;
	G_STATUS_REG.STDLAT  = 0;
	G_STATUS_REG.STD     = 0;
	G_STATUS_REG.UVLO    = 1;
	G_STATUS_REG.BPDF    = 1;
	G_STATUS_REG.APDF    = 1;
	G_STATUS_REG.BOCP    = 0;
	G_STATUS_REG.AOCP    = 0;
	G_STATUS_REG.OTS     = 1;
	dataHi = REGWRITE | (G_STATUS_REG.Address << 4);
	dataLo = (G_STATUS_REG.STDLAT << 7) | (G_STATUS_REG.STD << 6) | (G_STATUS_REG.UVLO << 5) | (G_STATUS_REG.BPDF << 4) | (G_STATUS_REG.APDF << 3) | (G_STATUS_REG.BOCP << 2) | (G_STATUS_REG.AOCP << 1) | (G_STATUS_REG.OTS);
	SPI_DRV8711_ReadWrite(dataHi, dataLo);
}
