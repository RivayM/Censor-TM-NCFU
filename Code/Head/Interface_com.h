//********************************************************
//  Interface common (For prog SPI and I2c)
//********************************************************
#ifndef Interface_common
#define Interface_common
#include "CONFIG.h"
//********************************************************
#define FREQ_120KHZ_H 0xFF
#define FREQ_120KHZ_L 0xFB
#define FREQ_480KHZ_H 0xFF
#define FREQ_480KHZ_L 0xFD
//********************************************************
/*USE FREQ IN void init_CLK*/
#define FREQ_H FREQ_120KHZ_H         // setting freq
#define FREQ_L FREQ_120KHZ_L

//********************************************************
//  USE:
/* 
void ISR_Timer0() interrupt 1 {
if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}
*/
//********************************************************
//  for NRF24L01
//********************************************************
#define BUFFER_SPI 8   // amount bit in packet(byte)
#define BUFFER_SPI_MASSIV_SIZE NRF24_BUFFER_MASSIV_SIZE // amount packet(byte)
#define SPI_MSB 1      // or use SPI_LSB = 0
extern unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  
extern unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  

extern int counterBit;  // current bit in packet(byte)
extern int counterByte; // current packet(byte)
extern int amountByteArrayForSend; // current packet(byte) for SEND
extern bit FlagInComSPIGlobal;  	 // Permission (exchange)

/* USE GPIO.h FOR SPI*/
#define PIN_CLK_SPI    P10
#define PIN_MOSI_SPI   P00
#define PIN_MISO_SPI   P01
#define PIN_CS_SPI     P15

void InCom_SPI_init_Timer(void);
bit InCom_SPI_Output_in_buffer(unsigned char *outSideBuffer);
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer );
void InCom_SPI_CLK_init(bit init);
void InCom_SPI_Input_in_buffer(unsigned char *outSideBuffer);
void InCom_SPI_exchange(void);    
//********************************************************

#endif