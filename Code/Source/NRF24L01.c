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
xdata struct NRF_PACKET_SPI packetRX = {
	/*reg:						,value:   */
	{W_REG|NRF_CONFIG	,0x0F},		//	CONFIG
	{W_REG|EN_AA			,0x3f},		//	EN_AA
	{W_REG|SETUP_AW		,0x03},		//	SETUP_AW
	{W_REG|RF_CH			,0x4C},		//	RF_CH		
	{W_REG|RF_SETUP		,0x06},		//	RF_SETUP
	{W_REG|EN_RXADDR	,0x02},		//	EN_RXADDR		
	
	{W_REG|RX_PW_P0		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P1		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P2		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P3		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P4		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P5		,0x01},		//	RX_PW_P0
	
	{W_REG|TX_ADDR		,'V','V','V','V','t'},	//	TX_ADDR	
	
	{W_REG|RX_ADDR_P0	,'V','V','V','V','t'},	//	RX_ADDR0
	{W_REG|RX_ADDR_P1	,'V','V','V','V','r'},	//	RX_ADDR0
	{W_REG|RX_ADDR_P2	,'r'},									//	RX_ADDR0
	{W_REG|RX_ADDR_P3	,'y'},									//	RX_ADDR0
	{W_REG|RX_ADDR_P4	,'u'},									//	RX_ADDR0
	{W_REG|RX_ADDR_P5	,'i'},									//	RX_ADDR0
	
	{FLUSH_RX					,NOP}			//	FLUSH		
};

//**************************************************************************
//  struct for send mode TX
//**************************************************************************
xdata struct NRF_PACKET_SPI packetTX = {
	
};

//**************************************************************************
// NRF24L01
//**************************************************************************

int currentProgress = 0;
xdata unsigned char readBuf[NRF_MASSIV_SIZE]={0x00}; 

/*initialization RF*/
bit NRF_init(struct NRF_PACKET_SPI *packet){
	switch(currentProgress){
		case START_PROCESS: NRF_CE = 0;									break;
		case 1:	Timer3_Delay100ms(START_DELAY);					break;
		case 2: Send_SPI_NRF( packet->vCONFIG,		2 );	break;
		case 3: Send_SPI_NRF( packet->vEN_AA,			2 );	break;	
		case 4:	Send_SPI_NRF( packet->vSETUP_AW,	2 );	break;	
		case 5:	Send_SPI_NRF( packet->vRF_CH, 		2 );	break;	
		case 6:	Send_SPI_NRF( packet->vRF_SETUP, 	2 );	break;	
		case 7:	Send_SPI_NRF( packet->vEN_RXADDR, 2 );	break;	
		case 8:	Send_SPI_NRF( packet->vRX_PW_P0, 	2 );	break;	
		case 9:	Send_SPI_NRF( packet->vTX_ADDR, 	6 );	break;	
		case 10:Send_SPI_NRF( packet->vRX_ADDR0,	6 );	break;	
		case 11:NRF_CE = 1;															break;
		case 12:currentProgress = END_PROCESS;					break;
		default: break;
	}
	if(currentProgress == END_PROCESS){ 
		currentProgress = 0;
		return 1;
	}
	else {
		currentProgress++;
		return 0;
	}
}

/*NRF send(radio) */
bit NRF_send(/*struct DATA_PACKET_SEND *packet*/){
	switch(currentProgress){
		case START_PROCESS: NRF_CE = 0;									break;
		default: break;
	
	}
	//процесс очистки.
	// дергать сe ( как ардуино гдето 200микросекунд или более)
	//или попробовать ждать прерывание об окончании отправки пакета
	// и се= 0
	if(currentProgress == END_PROCESS){ 
		currentProgress = 0;
		return 1;
	}
	else {
		currentProgress++;
		return 0;
	}
}

/*NRF get(radio) */
bit NRF_get(/*struct DATA_PACKET_SAVE *packet*/){
	switch(currentProgress){
		case START_PROCESS: NRF_CE = 0;				break;
		case 1: NRF_ack_status();							break; // ждать или прочитать статус
		case 2:	NRF_read_value();							break;
		case 3: 
			if( readBuf[0] & FLAG_RX_DR ){
				Send_SPI_NRF( R_RX_PL, 	NRF_MASSIV_SIZE );
			}
			else{
				currentProgress = END_PROCESS;
			}
			break;
		case 4:	NRF_read_value();							break;	
		case 5: 
			if( NRF_clear_FIFO() ){             // until the process is completed
				break;
			}
			else {
				return 0;
			}
		case 6: currentProgress = END_PROCESS;break;
		default: break;
	}
	if(currentProgress == END_PROCESS){ 
		currentProgress = 0;
		return 1;
	}
	else {
		currentProgress++;
		return 0;
	}

}	

/*send info for SPI*/
void Send_SPI_NRF(unsigned char *message,int amountMessage){
	int i;
	for(i = 0; i <= amountMessage; i++){
		valueBufferArrayTx[i] = *(message + i);
	}		
	amountByteArrayForSend = amountMessage;
	InCom_SPI_start();      			//start exchange
}

/* clear all flags*/		
void NRF_clear_IRQ(void){
	unsigned char array[NRF_MASSIV_SIZE] = 
		{W_REG|STATUS,0x70};
	Send_SPI_NRF(&array,		2);		// Clear RX_DR, TX_DS, MAX_RT flags
}

/* Clear all FIFO*/		
bit NRF_clear_FIFO(void){
	unsigned char arrayRX[NRF_MASSIV_SIZE] = 
		{W_REG|FLUSH_RX,NOP};
	unsigned char	arrayTX[NRF_MASSIV_SIZE] = 
		{W_REG|FLUSH_TX,NOP};
	switch(currentProgress){
		case START_PROCESS: NRF_CE = 0;						break;
		case 1:	Send_SPI_NRF(&arrayRX,		2);			break;
		case 2: Send_SPI_NRF(&arrayTX,		2);			break;
		case 3: NRF_CE = 1;												break;	
		case 4: currentProgress = END_PROCESS;		break;
		default: break;
	}
	if(currentProgress == END_PROCESS){ 
		currentProgress = 0;
		return 1;
	}
	else {
		currentProgress++;
		return 0;
	}
}

/* ask the status*/	
void NRF_ack_status(void){
	unsigned char array[NRF_MASSIV_SIZE] =
		{STATUS};
	Send_SPI_NRF( array, 1 ); 	// read Status
}	

/* read last answer for spi*/	
void NRF_read_value(void){
	int i;
	for(i = 0; i < NRF_MASSIV_SIZE;i++){
		readBuf[i] = valueBufferArrayRx[i];
	}
}



