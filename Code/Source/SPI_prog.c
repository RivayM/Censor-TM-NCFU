//********************************************************
//  PROG_SPI 
//********************************************************
#include <SPI_prog.H>
#include <N76E003.H>
 
pdata unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE] = 0;  
pdata unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE] = 0;

int amountByteForSendSPI 	= BUFFER_SPI_MASSIV_SIZE; 
int counterBitSPI        	= 0;   
int counterByteSPI       	= 0;
int valueDelay						= 0;

bit FlagSPIGlobal					= 0;
bit FlagExchangeSPIStart	= 0;
bit FlagSPIDelay				 	= 0;

void SPI_write_amount_byte(int value){amountByteForSendSPI = value;}

unsigned char *SPI_get_RX_buf(){return &valueBufferArrayRx;}

void SPI_write_TX_buf(unsigned char *buf){
	int  i;
	for( i = 0; i <= amountByteForSendSPI; i++ ){
		valueBufferArrayTx[i] = *( buf + i );
	}
}

//********************************************************
//			_____			 _____				->	CLK
//			|		|			 |	 |
//______|		|______|	 |______
//  f()	 f()	f()		f()	 f()		->	SPI_exchange_start()
//********************************************************
/* main func(for timer) */
void SPI_exchange_start(void){
	/*every clk launch*/
	SPI_exchange_do( 
		SPI_Data_Convert_Bit(
			&valueBufferArrayTx[counterByteSPI]),	// buf  for TX
		&valueBufferArrayRx[counterByteSPI]);		// buf  for RX
}

/* exchange bitwise operation */
void SPI_exchange_do(unsigned char valueMosi, unsigned char *outSideBuffer){ 
/******************************/
	if(counterBitSPI){			
		PIN_CLK_SPI =~ PIN_CLK_SPI;
	}
	else{	/*counterBit=0*/ 						// what do when start packet
		if(PIN_CLK_SPI == SPI_CPOL ){				// check setings
			/*don't change state*/
		}
		else{ 
			PIN_CLK_SPI =~ PIN_CLK_SPI; 	
		}		
	}
	if( PIN_CLK_SPI == SPI_CPHA) {       	// CPHA - when next bit	
/******************************/
		switch(counterBitSPI){		 					// Start
/*End bit packet	*/
			case BUFFER_SPI	/*	max bit	*/:          				
				PIN_MOSI_SPI 		= 1;
				//PIN_MISO_SPI 	= 1;
				PIN_CLK_SPI 		= SPI_CPOL;	
				counterBitSPI		= 0;
				SPI_exchange_end();       			// continue?
				SPI_Data_Convert_Byte();  			// next packet
				break;
/* process going	*/
			default  /* 0 -> max bit - 1*/: 
				PIN_CS_SPI   = 0;
/* send MoSi	*/
				if(valueMosi){PIN_MOSI_SPI = 1;}
				else{ 				PIN_MOSI_SPI = 0;}
/* send MiSo	*/
				if ( PIN_MISO_SPI == 1){      	 	
					outSideBuffer += SPI_Data_Convert_Bit(outSideBuffer);
				}	
/* next bit		*/
				counterBitSPI++; 									
				break;
		}
	}
/******************************/	
	if(counterBitSPI==0 && SPI_CPHA !=PIN_CLK_SPI){
		PIN_CLK_SPI =~ PIN_CLK_SPI; 
	}
/******************************/
}

/* In the latest BYTE exchange SPI*/
void SPI_exchange_end(void){	
	#if SPI_DATA_BYTE == SPI_MS_BYTE
		if(counterByteSPI == 0){ 
			SPI_End();  
		}
	#else
		if(counterByteSPI == amountByteForSendSPI - 1){ 
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
	counterByteSPI--;     // SPI_MSbyte 
#else
	counterByteSPI++;     // SPI_LSbyte
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
	
	buf &= (counterBitSPI >= BUFFER_SPI - 1) ? 
		0x01 : 
		0x01 << (BUFFER_SPI - counterBitSPI - 1) ;

#else	/*SPI_DATA_BIT == SPI_LSB*/
	
	buf &= (counterBitSPI >= BUFFER_SPI - 1) ? 
		0x01 << BUFFER_SPI - 1 : 
		0x01 << counterBitSPI ;
	
#endif /*SPI_DATA_BIT */
	return buf;
}

/*Delay = 1 timer cycle timer*/
void SPI_Delay(){
	if(valueDelay){	
		valueDelay--;	
	}
	else{	
		FlagSPIDelay 	= 0;	
		FlagSPIGlobal = 0;
	}
}

/*SET Delay = 1 timer cycle timer*/
void SPI_Delay_Set(int delay){
	valueDelay 		= delay;
	FlagSPIDelay 	= 1;
	FlagSPIGlobal = 1;
}

/* init clk */
void SPI_CLK_init(bit value){ 
	PIN_CLK_SPI = value;
}

/* Start exchange spi */
void SPI_Start(void){
#if SPI_DATA_BYTE == SPI_MS_BYTE
	counterByteSPI = amountByteForSendSPI - 1;
#else
	counterByteSPI = 0;
#endif
	FlagSPIGlobal  = 1;
}
