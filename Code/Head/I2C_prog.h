//********************************************************
//  I2C_prog
//********************************************************
#ifndef I2C_prog
#define I2C_prog

#include <CONFIG.h>
#include <GPIO.h>

/* Settings */
#define I2C_CPOL				0					// for slk
#define I2C_CPHA				0					// for slk

#define I2C_DATA_BIT 		1 				// or use I2C_LSB = 0 ; I2C_MSB = 1
#define I2C_DATA_BYTE 	0 				// or use I2C_LSbyte = 0 ; I2C_MSbyte = 1

#define I2C_SIZE_ADR 		7					// size adress
#define I2C_SIZE_DATA		8					// size 1 packet

#define I2C_SIZE_PACKET_BUF	32+1	// max size 1 packet (2 int + 1 / +1->this end process)
#define I2C_AMOUNT_PACKETS		 6	// max size Packets

enum I2cMode{ SEND = 0, READ = 1 }; // For func I2C_exchange_do

static xdata int sizeADRI2c;			//
static xdata int sizeDataI2c;			//
static xdata int sizeBufI2c;			// <=I2C_SIZE_BUF
static xdata int sizeByteSendI2C; //

static xdata int counterBitI2c; 	// current bit in packet(byte)
//static xdata int counterByteI2c; 	// current packet(byte)

static xdata unsigned int bufferLowValueRX;				
static xdata unsigned int bufferHighValueRX;

static xdata unsigned int bufferLowValueTX;				
static xdata unsigned int bufferHighValueTX;

extern bit FlagI2cStart;
extern bit FlagI2cDelay;

/*	Pins	*/
#define I2C_SLK 		ADSK						//
#define	I2C_SDA			ADOUT 					//	

/*	Func	*/
long I2C_Read_Buf();
void I2C_Write_Buf(long value);

void I2C_change_size_ADR (int value);					//
void I2C_change_size_DATA(int value);					//
void I2C_change_size_BUF (int value);					//
void I2C_change_size_BYTE(int value);					//

void I2C_init_SLK(bit value);									//

void I2C_exchange_start(enum I2cMode mode);		//				
void I2C_exchange_do(enum I2cMode mode);			//
void I2C_exchange_end(void);									//

void I2C_delay(void);
void I2C_start();

static bit I2C_Get_Bit_Buf_TX(void);
static void I2C_Write_Bit_Buf_RX(void);
	
#endif /*	I2C_prog	*/




