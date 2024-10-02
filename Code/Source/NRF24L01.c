//***********************************************************************************************************
//  NRF24L01 
//***********************************************************************************************************
#include <GPIO.h>
#include <Interface_com.H>
#include <NRF24L01.H>
#include <N76E003.h>

//**************************************************************************
//  struct for send mode RX
//**************************************************************************
xdata struct PACKET packetRX = {
	/*reg:						,value:   */
	{W_REG|NRF_CONFIG	,0x03},		//	CONFIG
	{W_REG|EN_AA			,0x00},		//	EN_AA
	{W_REG|SETUP_AW		,0x03},		//	SETUP_AW
	{W_REG|RF_CH			,0x6E},		//	RF_CH		
	{W_REG|RF_SETUP		,0x06},		//	RF_SETUP
	{W_REG|EN_RXADDR	,0x01},		//	EN_RXADDR		
	{W_REG|RX_PW_P0		,0x01},		//	RX_PW_P0
	//{0x01,0x02,0x03,0x04,0x05,0x05}								//	RX_PW_P1
	{W_REG|TX_ADDR		,'V','V','V','V','t'},				//	TX_ADDR	
	{W_REG|RX_ADDR_P0	,'V','V','V','V','r'},				//	RX_ADDR0
	//{0x01,0x02,0x03,0x04,0x05,0x05}								//	RX_ADDR1
	{FLUSH_RX					,NOP}			//	FLUSH		
};

//**************************************************************************
//  struct for send mode TX
//**************************************************************************
xdata struct PACKET packetTX = {
	/*reg:						,value:*/
	{W_REG|NRF_CONFIG	,0x02,0x00,0x04,0x05,0x05},		//	CONFIG
	{W_REG|EN_AA			,0x0B,0x0C,0x0D,0x0E,0x05},		//	EN_AA
	{W_REG|SETUP_AW		,0x02,0x03,0x04,0x05,0x05},		//	SETUP_AW
	{W_REG|RF_CH			,0x02,0x03,0x04,0x05,0x05},		//	RF_CH		
	{W_REG|RF_SETUP		,0x02,0x03,0x04,0x05,0x05},		//	RF_SETUP
	{W_REG|EN_RXADDR	,0x02,0x03,0x04,0x05,0x05},		//	EN_RXADDR		
	{W_REG|RX_ADDR_P0	,0x02,0x03,0x04,0x05,0x05},		//	RX_PW_P0
	//{0x01,0x02,0x03,0x04,0x05,0x05}								//	RX_PW_P1
	{W_REG|TX_ADDR		,'V','V','V','V','t'},				//	TX_ADDR	
	{W_REG|RX_ADDR_P0	,'V','V','V','V','r'},				//	RX_ADDR0
	//{0x01,0x02,0x03,0x04,0x05,0x05}								//	RX_ADDR1
	{FLUSH_RX					,NOP,0x03,0x04,0x05,0x05}			//	FLUSH		
};

//**************************************************************************
// NRF24L01
//**************************************************************************

int currentProgress = 0;
xdata unsigned char readBuf[BUFFER_MASSIV_SIZE]={0x00}; 

/*initialization as receiver*/
bit NRF_init(struct PACKET *packet){
	switch(currentProgress){
		case START_PROGRESS: NRF_CE = 0;						break;
		case 1:	Timer3_Delay100ms(START_DELAY);			break;
		case 2: NRF_send( packet->vCONFIG,		2 );	break;
		case 3: NRF_send( packet->vEN_AA,			2 );	break;	
		case 4:	NRF_send( packet->vSETUP_AW,	2 );	break;	
		case 5:	NRF_send( packet->vRF_CH, 		2 );	break;	
		case 6:	NRF_send( packet->vRF_SETUP, 	2 );	break;	
		case 7:	NRF_send( packet->vEN_RXADDR, 2 );	break;	
		case 8:	NRF_send( packet->vRX_PW_P0, 	2 );	break;	
		case 9:	NRF_send( packet->vTX_ADDR, 	6 );	break;	
		case 10:NRF_send( packet->vRX_ADDR0,	6 );	break;	
		case 11:NRF_CE = 1;													break;
		case 12:currentProgress = END_PROGRESS;			break;
		default: break;
	}
	if(currentProgress == END_PROGRESS){ 
		currentProgress = 0;
		return 1;
	}
	else {
		currentProgress++;
		return 0;
	}
}

/*send info for SPI*/
void NRF_send(unsigned char *message,int amountMessage){
	int i;
	for(i = 0; i <= amountMessage; i++){
		valueBufferArrayTx[i] = *(message + i);
	}		
	amountByteArrayForSend = amountMessage;
	InCom_SPI_start();      //start exchange
}

/* clear all flags*/		
void NRF_clear_IRQ(void){
	unsigned char array[BUFFER_MASSIV_SIZE] = 
		{W_REG|STATUS,0x70};
	NRF_send(array,		2);		// Clear RX_DR, TX_DS, MAX_RT flags
}

/* Clear all FIFO*/		
void NRF_clear_FIFO(void){
	unsigned char array[BUFFER_MASSIV_SIZE] = 
		{W_REG|STATUS,0x70};
	NRF_send(array,		2);		// Clear RX_DR, TX_DS, MAX_RT flags
}

/* ask the status*/	
void NRF_ack_status(void){
	unsigned char array[BUFFER_MASSIV_SIZE] =
		{STATUS};
	NRF_send( array, 1 ); 	// read Status
}	

/* read last answer for spi*/	
void NRF_get_value(void){
	int i;
	for(i = 0; i < BUFFER_MASSIV_SIZE;i++){
		readBuf[i] = valueBufferArrayRx[i];
	}
}
	
	
	
	
	