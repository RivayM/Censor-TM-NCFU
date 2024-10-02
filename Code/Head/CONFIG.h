//**************************************************************************
//  Config prog
//  Cencor for TM
//  2024
//**************************************************************************

#ifndef CONFIG
#define CONFIG
// WORK NRF24 ( mode)
#define TX_mode  1 					//  
#define RX_mode  2 					//  
#define RTX_mode 3  				//  
#define NRF24_BUFFER_MASSIV_SIZE 5 + 1 // amount byte dor sent to spi
// MAIN processes
#define END_PROGRESS  	999	//  
#define START_PROGRESS  0		//  
#define SAVE_PACKET_MASSIV_SIZE 10

//**************************************************************************
//  struct for SAVE 
//**************************************************************************
extern struct SAVE_PACKET{  //PACKET vXXXX -> v= value XXX=REGISTR
	unsigned char NAME			[SAVE_PACKET_MASSIV_SIZE];			// 
	unsigned char MODE			[SAVE_PACKET_MASSIV_SIZE];			//
	unsigned char DATA_ADC	[SAVE_PACKET_MASSIV_SIZE];			//	
};

#endif

