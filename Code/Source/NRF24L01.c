//***********************************************************************************************************
//  NRF24L01 
//***********************************************************************************************************
#include <GPIO.h>
#include <SPI_prog.H>
#include <NRF24L01.H>
#include <N76E003.h>

//**************************************************************************
/*  struct for send mode RX  */
//**************************************************************************
xdata struct NRF_PACKET_SPI packetRX = {
	/*reg:						,value:   */
	{W_REG|NRF_CONFIG	,0x0F},		//	CONFIG
	{W_REG|EN_AA			,0x00},		//	EN_AA
	{W_REG|SETUP_AW		,0x03},		//	SETUP_AW
	{W_REG|RF_CH			,0x6E},		//	RF_CH		
	{W_REG|RF_SETUP		,0x05},		//	RF_SETUP
	{W_REG|EN_RXADDR	,0x1F},		//	EN_RXADDR		
	{W_REG|DYNPD			,0x00},		//	DYNPD
	{W_REG|FEATURE		,0x00},		//	FEATURE
	
	{W_REG|RX_PW_P0		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P1		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P2		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P3		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P4		,0x01},		//	RX_PW_P0
	{W_REG|RX_PW_P5		,0x01},		//	RX_PW_P0
	//adr write lsbyte first
	{W_REG|TX_ADDR		,'X','X','X','X','X'},	//	TX_ADDR	
	
	{W_REG|RX_ADDR_P0	,'X','X','X','X','X'},	//	RX_ADDR0
	{W_REG|RX_ADDR_P1	,'V','V','V','V','V'},	//	RX_ADDR0
	{W_REG|RX_ADDR_P2	,'q'},									//	RX_ADDR0
	{W_REG|RX_ADDR_P3	,'w'},									//	RX_ADDR0
	{W_REG|RX_ADDR_P4	,'e'},									//	RX_ADDR0
	{W_REG|RX_ADDR_P5	,'i'},									//	RX_ADDR0
	
	{FLUSH_RX					,NOP}			//	FLUSH		
};
//**************************************************************************
/*  struct for send mode TX  */
//**************************************************************************
xdata struct NRF_PACKET_SPI packetTX = {
	/*reg:						,value:   */
	{W_REG|NRF_CONFIG	,0x0E},		//	CONFIG
};
//**************************************************************************
/*  struct for READ state reg  */
//**************************************************************************
xdata struct NRF_PACKET_SPI packetRX_READ = {
	/*reg:						,value:   */
	{R_REG|NRF_CONFIG	},		//	CONFIG
	{R_REG|EN_AA			},		//	EN_AA
	{R_REG|SETUP_AW		},		//	SETUP_AW
	{R_REG|RF_CH			},		//	RF_CH		
	{R_REG|RF_SETUP		},		//	RF_SETUP
	{R_REG|EN_RXADDR	},		//	EN_RXADDR		
	{R_REG|DYNPD			},		//	DYNPD
	{R_REG|FEATURE		},		//	FEATURE
	
	{R_REG|RX_PW_P0		},		//	RX_PW_P0
	{R_REG|RX_PW_P1		},		//	RX_PW_P0
	{R_REG|RX_PW_P2		},		//	RX_PW_P0
	{R_REG|RX_PW_P3		},		//	RX_PW_P0
	{R_REG|RX_PW_P4		},		//	RX_PW_P0
	{R_REG|RX_PW_P5		},		//	RX_PW_P0
	{R_REG|TX_ADDR		},		//	TX_ADDR	
	
	{R_REG|RX_ADDR_P0	},		//	RX_ADDR0
	{R_REG|RX_ADDR_P1	},		//	RX_ADDR0
	{R_REG|RX_ADDR_P2	},		//	RX_ADDR0
	{R_REG|RX_ADDR_P3	},		//	RX_ADDR0
	{R_REG|RX_ADDR_P4	},		//	RX_ADDR0
	{R_REG|RX_ADDR_P5	},		//	RX_ADDR0
	
	{FLUSH_RX					}			//	FLUSH		
};

//**************************************************************************
// ARRAY
//**************************************************************************
xdata unsigned char COMMAND_SEND_RF				[NRF_MASSIV_SIZE] = 
		{W_TX_PL /*next byte for data send[1-x]*/};
xdata unsigned char COMMAND_READ_RF				[NRF_MASSIV_SIZE] = 
		{R_RX_PL};
xdata	unsigned char COMMAND_CLEAR_FLUSH_RX[NRF_MASSIV_SIZE] = 
		{FLUSH_RX};
xdata	unsigned char	COMMAND_CLEAR_FLUSH_TX[NRF_MASSIV_SIZE] = 
		{FLUSH_TX};
xdata	unsigned char COMMAND_CLEAR_IRQ			[NRF_MASSIV_SIZE] = 
		{W_REG|STATUS,0x70};
xdata	unsigned char COMMAND_READ_PIPE0		[NRF_MASSIV_SIZE] = 
		{RX_PW_P0};			
xdata	unsigned char COMMAND_READ_FIFO_STAT[NRF_MASSIV_SIZE] = 
		{FIFO_STATUS};		
xdata	unsigned char COMMAND_W_ACK_PAYLOAD	[NRF_MASSIV_SIZE] = 
		{W_ACK_PL + /*need get(in status) ->PPP*/ 0x00};	
		
//**************************************************************************
/* NRF24L01 additional        */
//**************************************************************************
xdata unsigned char readBuf[NRF_MASSIV_SIZE]={0x00}; 

bit FlagDataReadReady = 0;
int currentProcess = 0;
		
/*initialization RF*/
bit NRF_init(struct NRF_PACKET_SPI *packet){
	switch(currentProcess){
		case START_PROCESS: NRF_CE = 0;									break;
		
		case 1:	/*Send_SPI_NRF( packet->vCONFIG,		2 );*/	break;
		case 2: NRF_delay();														break;
		case 3: Send_SPI_NRF( packet->vEN_AA,			2 );	break;
		case 4:	Send_SPI_NRF( packet->vSETUP_AW,	2 );	break;	
		case 5:	Send_SPI_NRF( packet->vRF_CH, 		2 );	break;	
		case 6:	Send_SPI_NRF( packet->vRF_SETUP, 	2 );	break;	
		case 7:	Send_SPI_NRF( packet->vEN_RXADDR, 2 );	break;
		case 8:	Send_SPI_NRF( packet->vDYNPD, 		2 );	break;	
		case 9: Send_SPI_NRF( packet->vFEATURE, 	2 );	break;
		
		case 10:Send_SPI_NRF( packet->vTX_ADDR, 	6 );	break;

		case 11:Send_SPI_NRF( packet->vRX_PW_P0, 	2 );	break;
		case 12:Send_SPI_NRF( packet->vRX_PW_P1, 	2 );	break;
		case 13:Send_SPI_NRF( packet->vRX_PW_P2, 	2 );	break;
		case 14:Send_SPI_NRF( packet->vRX_PW_P3, 	2 );	break;
		case 15:Send_SPI_NRF( packet->vRX_PW_P4, 	2 );	break;
		case 16:Send_SPI_NRF( packet->vRX_PW_P5, 	2 );	break;
		
		case 17:Send_SPI_NRF( packet->vRX_ADDR0,	6 );	break;
		case 18:Send_SPI_NRF( packet->vRX_ADDR1,	6 );	break;
		case 19:Send_SPI_NRF( packet->vRX_ADDR2,	2 );	break;
		case 20:Send_SPI_NRF( packet->vRX_ADDR3,	2 );	break;
		case 21:Send_SPI_NRF( packet->vRX_ADDR4,	2 );	break;
		case 22:Send_SPI_NRF( packet->vRX_ADDR5,	2 );	break;
		case 23:Send_SPI_NRF( packet->vCONFIG,		2 );	break;
		case 24: NRF_delay();														break;
		
		case 25:NRF_CE = 1;															break;
		case 26:currentProcess = END_PROCESS;						break;
		default: break;
	}
	return Check_Out();
}

/*NRF change mode to RX or TX */
bit NRF_change_mode_RF(struct NRF_PACKET_SPI *packet, bit stateCeEnd){
	switch(currentProcess){
		case START_PROCESS: NRF_CE = 0;									break;
		case 1: NRF_delay();														break;
		case 2: Send_SPI_NRF( packet->vCONFIG,			2 );break;
		case 3:	NRF_delay();														break;
		case 4:	NRF_CE = stateCeEnd;										break;
		case 5:	currentProcess = END_PROCESS;						break;
		default: break;
	}
	return Check_Out();
}

/*NRF send(radio) */
bit NRF_send(/*struct DATA_PACKET_SEND *packet*/){
	switch(currentProcess){
		case START_PROCESS: NRF_CE = 0;								break;
		case 1: COMMAND_SEND_RF[1] = 0x30;						break;		
		case 2: Send_SPI_NRF( &COMMAND_SEND_RF, 2 );	break;
		case 5: NRF_CE = 1;														break;
		case 6: NRF_delay();NRF_delay();NRF_delay();	break;
		case 7: NRF_CE = 0;														break;
		case 8: Send_SPI_NRF( &COMMAND_CLEAR_IRQ, 2 );break;
		case 9: currentProcess = END_PROCESS;					break;
		default: break;
	}
	return Check_Out();
}

/*NRF get(radio) */
bit NRF_get(/*struct DATA_PACKET_SAVE *packet*/){
	switch(currentProcess){
		case START_PROCESS: /*NRF_CE = 1;*/					break;
		case 1: NRF_CE = 1;													break;
		case 2: NRF_delay();NRF_delay();NRF_delay();break;
		case 3: /*NRF_CE = 0;*/											break;	
		case 4:	
			Send_SPI_NRF( &COMMAND_READ_RF, 2 );
			break;
		case 7:	
			Send_SPI_NRF( &COMMAND_CLEAR_FLUSH_RX, 2 );
			break;
		case 8:	
			Send_SPI_NRF( &COMMAND_CLEAR_IRQ, 2 );
			break;
		case 9: currentProcess = END_PROCESS;break;
		default: break;
	}
	return Check_Out();
}	

/*send info for SPI*/
void Send_SPI_NRF(unsigned char *message,int amountMessage){
	int i;
	for(i = 0; i <= amountMessage; i++){
		valueBufferArrayTx[i] = *(message + i);
	}		
	amountByteArrayForSend = amountMessage;
	SPI_Start();      			//start exchange
}

/* clear all flags*/		
void NRF_clear_IRQ(void){
	Send_SPI_NRF(&COMMAND_CLEAR_IRQ,		2);		// Clear RX_DR, TX_DS, MAX_RT flags
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
		//valueBufferArrayRx[i] = 0x00;  				// forced zeroing
	}
}

/*check - end process?*/
bit Check_Out(){
	if(currentProcess == END_PROCESS){ 
		currentProcess = 0;
		return 1;
	}
	else {
		currentProcess++;
		return 0;
	}
}

/*NRF delay(wait)*/
void NRF_delay(void){
	SPI_Delay_Set(DELAY);
}



	



