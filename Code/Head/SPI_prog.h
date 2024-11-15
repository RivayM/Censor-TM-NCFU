//********************************************************
//  PROG_SPI 
//********************************************************
#ifndef SPI_prog
#define SPI_prog

#include "CONFIG.h"
#include "GPIO.h"

/* PINS -> USE GPIO.h FOR SPI*/
#define PIN_CLK_SPI   SPI_SCK 			// P10
#define PIN_MOSI_SPI  SPI_MOSI 			// P00
#define PIN_MISO_SPI  SPI_MISO 			// P01
#define PIN_CS_SPI    SPI_CSRF			// P15

/* Settings */
#define SPI_CPOL				0						// See void SPI_exchange_do
#define SPI_CPHA				0						// See void SPI_exchange_do

#define SPI_LSB					0
#define SPI_MSB					1
#define SPI_LS_BYTE			0
#define SPI_MS_BYTE			1

#define BUFFER_SPI_MASSIV_SIZE NRF24_BUFFER_MASSIV_SIZE // amount packet(byte)
#define BUFFER_SPI 	 		8 					// amount bit in packet(byte)
#define SPI_DATA_BIT 		SPI_MSB			// or use SPI_LSB = 0 ; SPI_MSB = 1
#define SPI_DATA_BYTE 	SPI_LS_BYTE // or use SPI_LSbyte = 0 ; SPI_MSbyte = 1

extern unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  
extern unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  

extern int counterBit; 							// current bit in packet(byte)
extern int counterByte; 						// current packet(byte)
extern int amountByteArrayForSend; 	// current packet(byte) for SEND
extern int valueDelay;             	// delay values 1 = 1 timer cycle timer ->TactTimer0

extern bit FlagSPIGlobal;  	 				// Permission global(all)
extern bit FlagSPIExchange;  	 			// Permission exchange)
extern bit FlagSPIDelay;        	 	// Permission delay

/* FUNC*/
void SPI_write_amount_byte(int value);
void SPI_write_TX_buf(unsigned char *buf);
unsigned char *SPI_get_RX_buf(void);

void SPI_CLK_init(bit value);
	
void SPI_Start(void);
void SPI_End(void);
	
void SPI_exchange_start(void);
void SPI_exchange_do(unsigned char valueMosi, unsigned char *outSideBuffer);
void SPI_exchange_end(void);

void SPI_Data_Convert_Byte(void);
unsigned char SPI_Data_Convert_Bit(unsigned char *outSideBuffer);

void SPI_Delay(void);
void SPI_Delay_Set(int delay);

#endif /*SPI_prog*/