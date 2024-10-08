//********************************************************
//  Interface common
//********************************************************
#include <Interface_com.H>
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
void InCom_SPI_init_Timer(){
	TIMER0_MODE1_ENABLE;			// Mode timer
	TH0 = FREQ_H;           	// setting freq:
  TL0 = FREQ_L; 						// 
	set_ET0;									// enable Timer0 interrupt
	set_TR0;                  // Timer0 run
}

/* main func(for timer) */
void InCom_SPI_exchange(void){
	InCom_SPI( 
	InCom_SPI_Output_in_buffer(
		&valueBufferArrayTx[counterByte]),   // buf  for Tx
	&valueBufferArrayRx[counterByte]);	   // buf  for Rx
}

/* exchange bitwise operation */
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer){ 
	if(counterBit) {                      // CPOL 
		PIN_CLK_SPI =~PIN_CLK_SPI;          // clocking
	}  								
	if( PIN_CLK_SPI == 0) {       				// CPHA
		switch(counterBit){		 							// Start
			/******************************/
			case BUFFER_SPI:          				// End packet
				PIN_MOSI_SPI = 1;
				PIN_MISO_SPI = 1;
				PIN_CLK_SPI  = 0;	
				counterBit = 0;
				if(counterByte == amountByteArrayForSend - 1){ // end exchange
					PIN_CS_SPI  = 1;
					counterByte = 0;
					FlagInComSPIGlobal = 0; 
				}
				else counterByte++;                 // next packet
				break;
			/******************************/
			default:                  				// process going
				PIN_CS_SPI   = 0;
				PIN_MOSI_SPI = valueMosi; 			// send MoSi
				if ( PIN_MISO_SPI == 1){      // send MiSo 	
					InCom_SPI_Input_in_buffer(outSideBuffer);
				}	
				counterBit++; 									// next bit
				break;
		}
	}
}

/* save reading bit in buf */
void InCom_SPI_Input_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;
	if(counterBit == 0){         // start bit
		if(SPI_MSB){
			*outSideBuffer = 0x80;   
			return;
		}
	}
	if(counterBit < BUFFER_SPI){ // next bit
		if(SPI_MSB)	buf = buf + (0x01 << counterBit -1);
		else 				buf = buf + (0x01 << counterBit);
		*outSideBuffer = buf;	
	}
}


/* read buf and send bit*/
bit InCom_SPI_Output_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  
	if (counterBit == 0){	 									// start bit
		if(SPI_MSB){ buf = buf & 0x01; } 		 	// mask MSB
		else       { buf = buf & 0x80; } 			// mask LSB
	}
	if( counterBit < 8 && counterBit != 0){	// next bit 
		buf = buf & (0x01 << counterBit);
	}
	if(buf) { return 1; } else { return 0; } 
}







/*Delay = 1 timer cycle timer*/
void InCom_Delay(){
	if(valueDelay){	valueDelay--;	}
	else{	
		FlagInComDelay = 0;	
		FlagInComSPIGlobal = 0;
	}
}

/*SET Delay = 1 timer cycle timer*/
void InCom_Set_Delay(int delay){
	valueDelay = delay;
	FlagInComDelay = 1;
	FlagInComSPIGlobal = 1;
}

/* init clk */
void InCom_SPI_CLK_init(bit init){ PIN_CLK_SPI = init;}

void InCom_SPI_start(void){FlagInComSPIGlobal = 1;}
