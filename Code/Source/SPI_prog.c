//********************************************************
//  Interface common
//********************************************************
#include <SPI_prog.H>
#include <N76E003.H>

unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  
unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  

int amountByteArrayForSend = BUFFER_SPI_MASSIV_SIZE; 
int counterBit             = 0;   
int counterByte            = 0;
bit FlagInComSPIGlobal     = 0;  

int	valueDelay						 = 0;
bit FlagInComDelay				 = 0;

/* Init timer and start */
void SPI_init_Timer(){
	TIMER0_MODE1_ENABLE;			// Mode timer
	TH0 = FREQ_H;           	// setting freq:
  TL0 = FREQ_L; 						// 
	set_ET0;									// enable Timer0 interrupt
	set_TR0;                  // Timer0 run
}

/* main func(for timer) */
void SPI_exchange_start(void){
	SPI_exchange_do( 
		SPI_Data_Convert_Bit(
			&valueBufferArrayTx[counterByte]),	// buf  for TX
		&valueBufferArrayRx[counterByte]);		// buf  for RX
}

/* exchange bitwise operation */
void SPI_exchange_do(unsigned char valueMosi, unsigned char *outSideBuffer){ 
	if(counterBit) {                      // CPOL 
		PIN_CLK_SPI =~PIN_CLK_SPI;          // clocking
	}  								
	if( PIN_CLK_SPI == 0) {       				// CPHA
/******************************/
		switch(counterBit){		 							// Start
			// End bit packet
			case BUFFER_SPI:          				
				PIN_MOSI_SPI = 1;
				//PIN_MISO_SPI = 1;
				PIN_CLK_SPI  = 0;	
				counterBit   = 0;
				SPI_exchange_end();       // continue?
				SPI_Data_Convert_Byte();  // next packet
				break;
			// process going
			default: 
				PIN_CS_SPI   = 0;
				if(valueMosi){PIN_MOSI_SPI = 1;}// send MoSi
				else{ 				PIN_MOSI_SPI = 0;}
				
				if ( PIN_MISO_SPI == 1){      	// send MiSo 	
					outSideBuffer = outSideBuffer +
					SPI_Data_Convert_Bit(outSideBuffer);
				}	
				
				counterBit++; 									// next bit
				break;
		}
	}
/******************************/	
}

/* In the latest BYTE exchange SPI*/
void SPI_exchange_end(void){	
	#if SPI_DATA_BYTE == 1
		if(counterByte == 0){ 
			SPI_End();  // SPI_MSbyte
		}
	#else
		if(counterByte == amountByteArrayForSend - 1){ 
			SPI_End();  // SPI_LSbyte
		}
	#endif
}

/* In the end exchange SPI*/
void SPI_End(void){
	PIN_CS_SPI  = 1;
	FlagInComSPIGlobal = 0; 
}


/* Convert DATA BYTE*/
void SPI_Data_Convert_Byte(){	
	#if SPI_DATA_BYTE == 1
		counterByte--;     // SPI_MSbyte 
	#else
		counterByte++;     // SPI_LSbyte
	#endif
}

/*Convert DATA BIT*/
unsigned char SPI_Data_Convert_Bit(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  
	#if SPI_DATA_BIT == 1
		buf = buf & 0x01 << (BUFFER_SPI - counterBit - 1);// SPI_MSB
	#else
		buf = buf & 0x01 << counterBit; 									// SPI_LSB
	#endif
	return buf;
}

/*Delay = 1 timer cycle timer*/
void SPI_Delay(){
	if(valueDelay){	valueDelay--;	}
	else{	
		FlagInComDelay = 0;	
		FlagInComSPIGlobal = 0;
	}
}

/*SET Delay = 1 timer cycle timer*/
void SPI_Delay_Set(int delay){
	valueDelay = delay;
	FlagInComDelay = 1;
	FlagInComSPIGlobal = 1;
}

/* init clk */
void SPI_CLK_init(bit value){ PIN_CLK_SPI = value;}
/* Start exchange spi */
void SPI_Start(void){
	#if SPI_DATA_BYTE == 1
		counterByte = amountByteArrayForSend -1;
	#else
		counterByte = 0;
	#endif
	FlagInComSPIGlobal = 1;
}
