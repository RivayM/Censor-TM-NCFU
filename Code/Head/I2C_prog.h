//********************************************************
//  I2C_prog
//********************************************************
#ifndef I2C_prog
#define I2C_prog

#include <CONFIG.h>
#include <GPIO.h>

/* Settings */
#define I2C_CPOL				0				// for slk
#define I2C_CPHA				0				// for slk

#define I2C_DATA_BIT 		1 			// or use I2C_LSB = 0 ; I2C_MSB = 1
#define I2C_DATA_BYTE 	0 			// or use I2C_LSbyte = 0 ; I2C_MSbyte = 1

#define I2C_SIZE_ADR 		7				// size adress
#define I2C_SIZE_DATA		8				// size 1 packet

static xdata int sizeADR;
static xdata int sizeData;

//extern unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  
//extern unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE]; 

extern bit FlagI2cStart;
extern bit FlagI2cDelay;

//extern int counterBit; 			// current bit in packet(byte)
//extern int counterByte; 		// current packet(byte)

/*	Pins	*/
#define I2C_SLK 	ADSK					//
#define	I2C_SDA		ADOUT 				//	INT0
#define	I2C_ON		PIN_ADC_ON 		// 		

/*	Func	*/
void I2C_change_size_ADR	(int *value);
void I2C_change_size_DATA	(int *value);

void I2C_start();

void I2C_exchange_start();
void I2C_exchange_do();
void I2C_exchange_end();
	
void I2C_delay();
	
	
#endif /*	I2C_prog	*/




