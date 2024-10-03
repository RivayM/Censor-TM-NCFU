/******************************/
/* NRF24L01*/
/******************************/
#ifndef NRF24L01
#define NRF24L01
#define BUFFER_MASSIV_SIZE NRF24_BUFFER_MASSIV_SIZE		// size buffer
/******************************/
/*SETINGS*/
/******************************/
#define START_DELAY  10          		 // first start
/******************************/
/*Commands*/
/******************************/
#define R_REG        0x00            //Read command from reg
#define W_REG        0x20            //Write command to reg
#define R_RX_PL      0x61            //RX payload register address
#define W_TX_PL      0xA0            //TX payload register address
#define FLUSH_TX     0xE1            //Flush TX register 
#define FLUSH_RX     0xE2            //Flush RX register 
#define REUSE_TX_PL  0xE3            //Reuse TX payload register
#define NOP          0xFF            //No operation, might be used to read status register
/******************************/
/*Register Address*/
/******************************/
#define NRF_CONFIG   0x00            //'Config' Reg  
#define EN_AA        0x01            //Enable Auto Ack Reg 
#define EN_RXADDR    0x02            //Enabled RX Addresses Reg
#define SETUP_AW     0x03            //Setup of Address Widths Reg
#define SETUP_PETR   0x04            //Setup of Automatic Retransmission Reg
#define RF_CH        0x05            //RF Channel Reg
#define RF_SETUP     0x06            //RF Setup Reg
#define STATUS       0x07            //Status Reg
#define OBSERVE_TX   0x08            //Transmit observe Reg
#define CD           0x09            //Carrier Detect Reg
#define RX_ADDR_P0   0x0A            //Receive address data pipe 0
#define RX_ADDR_P1   0x0B            //Receive address data pipe 1
#define RX_ADDR_P2   0x0C            //Receive address data pipe 2
#define RX_ADDR_P3   0x0D            //Receive address data pipe 3
#define RX_ADDR_P4   0x0E            //Receive address data pipe 4
#define RX_ADDR_P5   0x0F            //Receive address data pipe 5
#define TX_ADDR      0x10            //Transmit address. Used for a PTX device only
#define RX_PW_P0     0x11            //RX payload width, pipe 0 
#define RX_PW_P1     0x12            //RX payload width, pipe 1
#define RX_PW_P2     0x13            //RX payload width, pipe 2
#define RX_PW_P3     0x14            //RX payload width, pipe 3
#define RX_PW_P4     0x15            //RX payload width, pipe 4
#define RX_PW_P5     0x16            //RX payload width, pipe 5
#define FIFO_STATUS  0x17            //FIFO Status Reg
/******************************/
/*FLAG*/
/******************************/
#define FLAG_RX_DR   0x40            // Data Ready RX FIFO interrupt
#define FLAG_TX_DS   0x20            // Data Sent TX FIFO interrupt.
#define FLAG_MAX_RT  0x10            // Maximum number of TX retransmits

extern int currentProgress;      		 // current progress
extern xdata unsigned char readBuf[BUFFER_MASSIV_SIZE];      // buffer 
	
extern struct PACKET{  //PACKET vXXXX -> v= value XXX=REGISTR
	unsigned char vCONFIG			[BUFFER_MASSIV_SIZE];			// 
	unsigned char vEN_AA			[BUFFER_MASSIV_SIZE];			//
	unsigned char vSETUP_AW		[BUFFER_MASSIV_SIZE];			//
	unsigned char vRF_CH			[BUFFER_MASSIV_SIZE];			//
	unsigned char vRF_SETUP		[BUFFER_MASSIV_SIZE];			//
	unsigned char vEN_RXADDR	[BUFFER_MASSIV_SIZE];			//
	unsigned char vRX_PW_P0		[BUFFER_MASSIV_SIZE];			//
	// unsigned char vRX_PW_P1[BUFFER_MASSIV_SIZE];			//
	unsigned char vTX_ADDR		[BUFFER_MASSIV_SIZE];			//
	unsigned char vRX_ADDR0		[BUFFER_MASSIV_SIZE];			//
	//unsigned char vRX_ADDR1		[BUFFER_MASSIV_SIZE];		//
	unsigned char vFLUSH			[BUFFER_MASSIV_SIZE];			//	
};

extern xdata struct PACKET packetRX;
extern xdata struct PACKET packetTX;

/*длительные процессы*/
bit NRF_init(struct PACKET *packet);
bit NRF_clear_FIFO(void);
bit NRF_send(void);
bit NRF_get(void);

/*вспомогательное*/
void NRF_clear_IRQ(void);
void NRF_ack_status(void);
void NRF_get_value(void);

void Send_SPI_NRF(unsigned char *message,int amountMessage);

#endif







