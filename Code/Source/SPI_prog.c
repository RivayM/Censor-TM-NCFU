//********************************************************
//  PROG_SPI 
//********************************************************
#include <SPI_prog.H>
#include <N76E003.H>

unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE]; 
unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  

int amountByteArrayForSend 	= BUFFER_SPI_MASSIV_SIZE; 
int counterBit            	= 0;   
int counterByte           	= 0;

bit FlagSPIGlobal						= 0;
bit FlagExchangeSPIStart		= 0;

int	valueDelay						 	= 0;
bit FlagSPIDelay				 		= 0;

void SPI_write_amount_byte(int value){amountByteArrayForSend = value;}

unsigned char *SPI_get_RX_buf(){return &valueBufferArrayRx;}

void SPI_write_TX_buf(unsigned char *buf){
	int  i;
	for( i = 0; i <= amountByteArrayForSend; i++ ){
		valueBufferArrayTx[i] = *( buf + i );
	//	valueBufferArrayTx[i] = 0x33;
	}
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
	if(counterBit ||	SPI_CPOL) {         // CPOL - when start clk
		PIN_CLK_SPI =~PIN_CLK_SPI;          // clocking
	}  		
	if( PIN_CLK_SPI == SPI_CPHA) {       	// CPHA - when next bit
/******************************/
		switch(counterBit){		 							// Start
/*End bit packet	*/
			case BUFFER_SPI:          				
				PIN_MOSI_SPI = 1;
				//PIN_MISO_SPI = 1;
				PIN_CLK_SPI  = SPI_CPOL;	
				counterBit   = 0;
				SPI_exchange_end();       // continue?
				SPI_Data_Convert_Byte();  // next packet
				break;
/* process going	*/
			default  /* 0 - 7 */: 
				PIN_CS_SPI   = 0;
/* send MoSi	*/
				if(valueMosi){PIN_MOSI_SPI = 1;}
				else{ 				PIN_MOSI_SPI = 0;}
/* send MiSo	*/
				if ( PIN_MISO_SPI == 1){      	 	
					outSideBuffer = outSideBuffer +
					SPI_Data_Convert_Bit(outSideBuffer);
				}	
/* next bit		*/
				counterBit++; 									
				break;
		}
	}
/******************************/	
}

/* In the latest BYTE exchange SPI*/
void SPI_exchange_end(void){	
	#if SPI_DATA_BYTE == SPI_MS_BYTE
		if(counterByte == 0){ 
			SPI_End();  
		}
	#else
		if(counterByte == amountByteArrayForSend - 1){ 
			SPI_End();  
		}
	#endif
}

/* In the end exchange SPI*/
void SPI_End(void){
	PIN_CS_SPI  = 1;
	FlagSPIGlobal = 0; 
}


/* Convert DATA BYTE*/
void SPI_Data_Convert_Byte(){	
#if SPI_DATA_BYTE == SPI_MS_BYTE
	counterByte--;     // SPI_MSbyte 
#else
	counterByte++;     // SPI_LSbyte
#endif
}

/*Convert DATA BIT*/
unsigned char SPI_Data_Convert_Bit(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  
	/*
	in Nuvaton, shift operations by 0, we go to -1 
	and force the register to go around in a circle 
	(the whole cycle) - long operation
	*/
#if SPI_DATA_BIT == SPI_MSB
	if(counterBit >= BUFFER_SPI - 1){ 
		buf = buf & 0x01; 
	}
	else {
		buf = buf & 0x01 << (BUFFER_SPI - counterBit - 1);// SPI_MSB
	}
#else
	if(counterBit >= BUFFER_SPI - 1){
		buf = buf & 0x01 << BUFFER_SPI - 1;
	}
	else{
		buf = buf & 0x01 << counterBit; 									// SPI_LSB
	}
#endif
	return buf;
}

/*Delay = 1 timer cycle timer*/
void SPI_Delay(){
	if(valueDelay){	
		valueDelay--;	
	}
	else{	
		FlagSPIDelay = 0;	
		FlagSPIGlobal = 0;
	}
}

/*SET Delay = 1 timer cycle timer*/
void SPI_Delay_Set(int delay){
	valueDelay = delay;
	FlagSPIDelay = 1;
	FlagSPIGlobal = 1;
}

/* init clk */
void SPI_CLK_init(bit value){ 
	PIN_CLK_SPI = value;
}

/* Start exchange spi */
void SPI_Start(void){
#if SPI_DATA_BYTE == SPI_MS_BYTE
	counterByte = amountByteArrayForSend -1;
#else
	counterByte = 0;
#endif
	FlagSPIGlobal = 1;
}
