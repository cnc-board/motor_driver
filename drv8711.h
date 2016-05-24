/*
 * drv8711.h
 *
 *  Created on: 23 mars 2016
 *      Author: ethel
 */

#ifndef DRV8711_H_
#define DRV8711_H_

// CTRL Register
struct CTRL_Register
{
	unsigned char Address;	// bits 14-12
	unsigned char DTIME;		// bits 11-10
	unsigned char ISGAIN;	// bits 9-8
	unsigned char EXSTALL;	// bit 7
	unsigned char MODE;		// bits 6-3
	unsigned char RSTEP;		// bit 2
	unsigned char RDIR;		// bit 1
	unsigned char ENBL;		// bit 0
};

// TORQUE Register
struct TORQUE_Register
{
	unsigned char Address;	// bits 14-12
	/* Reserved */ 			// bit 11
	unsigned char SIMPLTH;  	// bits 10-8
	unsigned char TORQUE;	// bits 7-0
};

// OFF Register
struct OFF_Register
{
	unsigned char Address;	// bits 14-12
	/* Reserved */ 			// bits 11-9
	unsigned char PWMMODE;  	// bit 8
	unsigned char TOFF;		// bits 7-0
};

// BLANK Register
struct BLANK_Register
{
	unsigned char Address;	// bits 14-12
	/* Reserved */ 			// bits 11-9
	unsigned char ABT;  		// bit 8
	unsigned char TBLANK;	// bits 7-0
};

// DECAY Register
struct DECAY_Register
{
	unsigned char Address;	// bits 14-12
	/* Reserved */ 			// bit 11
	unsigned char DECMOD;  	// bits 10-8
	unsigned char TDECAY;	// bits 7-0
};

// STALL Register
struct STALL_Register
{
	unsigned char Address;	// bits 14-12
	unsigned char VDIV;  	// bits 11-10
	unsigned char SDCNT;		// bits 9-8
	unsigned char SDTHR;		// bits 7-0
};

// DRIVE Register
struct DRIVE_Register
{
	unsigned char Address;	// bits 14-12
	unsigned char IDRIVEP;  	// bits 11-10
	unsigned char IDRIVEN;	// bits 9-8
	unsigned char TDRIVEP;	// bits 7-6
	unsigned char TDRIVEN;	// bits 5-4
	unsigned char OCPDEG;	// bits 3-2
	unsigned char OCPTH;		// bits 1-0
};

// STATUS Register
struct STATUS_Register
{
	unsigned char Address;	// bits 14-12
	/* Reserved */			// bits 11-8
	unsigned char STDLAT;  	// bit 7
	unsigned char STD;		// bit 6
	unsigned char UVLO;		// bit 5
	unsigned char BPDF;		// bit 4
	unsigned char APDF;		// bit 3
	unsigned char BOCP;		// bit 2
	unsigned char AOCP;		// bit 1
	unsigned char OTS;		// bit 0
};


void drv_init();
void test2(unsigned int data);

void WriteAllRegisters();
void ReadAllRegisters();
unsigned int SPI_DRV8711_ReadWrite(unsigned char dataHi, unsigned char dataLo);
void init_cs();
void test();

void updateStep();

#endif /* DRV8711_H_ */
