//**************************************************************************
//  Config prog
//  Cencor for TM
//  2024
//**************************************************************************

#ifndef CONFIG
#define CONFIG

//**************************************************************************
/* Configuration									*/
//**************************************************************************


//**************************************************************************
/* WORK NRF24 ( mode)										*/
//**************************************************************************
#define TX_MODE  0 											//  
#define RX_MODE  1 											//  
#define NRF24_BUFFER_MASSIV_SIZE 5 + 1 	// amount byte dor sent to spi

//**************************************************************************
/* MAIN processes												*/
//**************************************************************************
#define END_PROCESS  	999								//  
#define START_PROCESS  0								//  
#define DATA_PACKET_MASSIV_SIZE 10

//**************************************************************************
//  struct/s for SAVE or SEND for NRF and FRAM
//**************************************************************************
extern struct DATA_PACKET_SAVE{  													//
	unsigned char NAME			[DATA_PACKET_MASSIV_SIZE];			// 
	unsigned char MODE			[DATA_PACKET_MASSIV_SIZE];			//
	unsigned char DATA_ADC	[DATA_PACKET_MASSIV_SIZE];			//	
};

extern struct DATA_PACKET_SEND{ 													//
	unsigned char NAME			[DATA_PACKET_MASSIV_SIZE];			// 
	unsigned char MODE			[DATA_PACKET_MASSIV_SIZE];			//
	unsigned char DATA_ADC	[DATA_PACKET_MASSIV_SIZE];			//	
};

#endif

