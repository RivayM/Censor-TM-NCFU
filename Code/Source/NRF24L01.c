//***********************************************************************************************************
//  NRF24L01 
//***********************************************************************************************************
#include <GPIO.h>
#include <Interface_com.H>
#include <NRF24L01.H>
#include <N76E003.h>

// размер адреса
// номер канала
// 

unsigned char CONFIG_SEND[BUFFER_MASSIV_SIZE] ={0x01,0x02,0x03,0x04,0x05};
unsigned char CONFIG_SENDE[BUFFER_MASSIV_SIZE] ={0x09,0x12,0x13,0x14,0x05};


unsigned char TX_ADDRESS[BUFFER_MASSIV_SIZE] = 
	{0x01,0x02,0x03,0x04,0x05};
unsigned char RX_ADDRESS[BUFFER_MASSIV_SIZE] =
	{0x01,0x02,0x03,0x04,0x05};

int currentProgress = 0;

void NRF_init(){
	NRF_CE = 0;
	// init IRQ 
}


void NRF_init_RX(){
	NRF_CE = 0;
	switch(currentProgress){
		default:FlagInComSPIGlobal = 0; break;
		case 0: NRF_send( &CONFIG_SEND  , 4 ); break;
		case 1: NRF_send( &CONFIG_SENDE , 4 ); break;
		
	}
}

void NRF_init_TX(){
	NRF_CE = 0;
}
	
/*send info for SPI*/
void NRF_send(unsigned char *message,int amountMessage){
	int i;
	for(i = 0; i <= amountMessage; i++){
		valueBufferArrayTx[i] = *(message + i);
	}		
	amountByteArrayForSend = amountMessage;
	FlagInComSPIGlobal = 1;
}

/*get info for SPI*/
unsigned char *NRF_read(){
	return 0x00;
}
	
void NRF_clear_IRQ(void){}
void NRF_get_status(void){}
	
	
	
	
	
	
	