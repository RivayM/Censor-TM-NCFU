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

/*initialization RF*/
bit NRF_init(struct PACKET *packet){
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
bit NRF_send(void){
	
	//процесс очистки.
	// дергать сe ( как ардуино гдето 200микросекунд или более)
	//или попробовать ждать прерывание об окончании отправки пакета
	// и се= 0
	
}

/*NRF get(radio) */
bit NRF_get(void){
	// ждать или прочитать статус
	// считать данные ( по регистуру) его записать со значением
	// после чтения стереть буффер.
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
	unsigned char array[BUFFER_MASSIV_SIZE] = 
		{W_REG|STATUS,0x70};
	Send_SPI_NRF(&array,		2);		// Clear RX_DR, TX_DS, MAX_RT flags
}

/* Clear all FIFO*/		
bit NRF_clear_FIFO(void){
	unsigned char arrayRX[BUFFER_MASSIV_SIZE] = 
		{W_REG|FLUSH_RX,NOP};
	unsigned char	arrayTX[BUFFER_MASSIV_SIZE] = 
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
	unsigned char array[BUFFER_MASSIV_SIZE] =
		{STATUS};
	Send_SPI_NRF( array, 1 ); 	// read Status
}	

/* read last answer for spi*/	
void NRF_get_value(void){
	int i;
	for(i = 0; i < BUFFER_MASSIV_SIZE;i++){
		readBuf[i] = valueBufferArrayRx[i];
	}
}
	
	
	
	
	
void NRF_read(){
	
}



