//**************************************************************************
//  Config prog
//  Cencor for TM
//  2024
//**************************************************************************

#ifndef CONFIG
#define CONFIG
/* WORK NRF24 ( mode)										*/
#define TX_MODE  1 											//  
#define RX_MODE  2 											//  
#define RTX_MODE 3  										//  
#define NRF24_BUFFER_MASSIV_SIZE 5 + 1 	// amount byte dor sent to spi

extern xdata int modeRF;                // mode by default

/* MAIN processes												*/
#define END_PROCESS  	999								//  
#define START_PROCESS  0								//  
#define SAVE_PACKET_MASSIV_SIZE 10

//**************************************************************************
//  struct/s for SAVE or SEND
//**************************************************************************
extern struct SAVE_PACKET{  //PACKET vXXXX -> v= value XXX=REGISTR
	unsigned char NAME			[SAVE_PACKET_MASSIV_SIZE];			// 
	unsigned char MODE			[SAVE_PACKET_MASSIV_SIZE];			//
	unsigned char DATA_ADC	[SAVE_PACKET_MASSIV_SIZE];			//	
};

#endif

