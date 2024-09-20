//***********************************************************************************************************
//  NRF24L01 
//***********************************************************************************************************
#include <GPIO.h>
#include <N76E003.h>
#include <CONFIG.h>
#include <NRF24L01.h>
#include <Interface_com.H>

int currentProgress = 0;

unsigned char NRF_TX_ADDRESS[BUFFER_MASSIV_SIZE ] =
{'i','n','t','e','r'};  // адрес передатчика
unsigned char NRF_RX_ADDRESS[BUFFER_MASSIV_SIZE ] =
{'m','a','r','k','1'};  // адрес приемника 

void NRF_send_byte(unsigned char *byte){
	InCom_SPI_byte_transitive(byte);	
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeByte = 1;
}

void NRF_send_array(unsigned char *array){
	InCom_SPI_byte_transitive(array);	
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeArray = 1;
}


void NRF_config(int mode){ 
	NRF_CE = 0;
	switch(mode){
		case TX_mode:	
			break;
		case RX_mode:	
			break;
		case RTX_mode:
			break;
		default: break;
	}
}
	
	
//  SPI_CE=0;                       
//  Timer0_Delay100us(100);
//	spi_WREG(W_REG|EN_AA,0x01);                             //Enable ShockBurst (Enable Auto ACK)    
//  spi_WREG(W_REG|SETUP_AW,0x03);                          //Puts the address field width 5bytes  
//  spi_WREG(W_REG|RF_CH,0x6E);                             //Set frequency channel 110 (2.510MHz)
//  spi_WREG(W_REG|RF_SETUP,0x06);                          //Setup: 1Mbps, 0dBm, LNA off  
//  spi_WREG(W_REG|EN_RXADDR,0x01);                         //Enable data pipe 0                  
//  spi_WBUF(W_REG|RX_ADDR_P0,RX_ADDRESS,TX_ADD_WIDTH);   //Set static RX address for auto ack
//  spi_WREG(W_REG|RX_PW_P0,TX_PLOAD_WIDTH);                //Set RX payload length
//  spi_WREG(FLUSH_RX,NOP);                                 //Clear the RX_FIFO
//	spi_WREG(FLUSH_TX,NOP);                    //Clear the TX_FIFO
//	spi_WREG(W_REG|CONFIG,0x0F);    //Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled  
//	Timer0_Delay100us(5);
//	SPI_CE=1;



//  SPI_CE=0;    
//	Timer0_Delay100us(100);
//  spi_WREG(W_REG|CONFIG,0x0E);               //PRIM_RX : PTX
//	clear_irq();
//	spi_WREG(W_REG|EN_AA,0x01);                //Enable ShockBurst (Enable Auto ACK)
//  spi_WREG(W_REG|EN_RXADDR,0x01);            //Enable data pipe 0
//  spi_WREG(W_REG|SETUP_AW,0x03);             // Puts the address field width 5bytes
//  spi_WREG(W_REG|SETUP_PETR,0x1A);           //Auto retransmit: wait 500us, 10 retries
//  spi_WREG(W_REG|RF_CH,0x6E);                //Set frequency channel 110 (2.510MHz)
//  spi_WREG(W_REG|RF_SETUP,0x06);             //Setup: 1Mbps, 0dBm, LNA off 
//  //spi_WREG(W_REG|CONFIG,0x0E);             //Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled 
//  spi_WBUF(W_REG|TX_ADDR,TX_ADDRESS,TX_ADD_WIDTH);      //Set static TX address
//  spi_WBUF(W_REG|RX_ADDR_P0,RX_ADDRESS,TX_ADD_WIDTH);   //Set static RX address for auto ack
//	spi_WBUF(W_TX_PL,payload,TX_PLOAD_WIDTH);  //Write specified buffer to FIFO
//  SPI_CE=1;
//  Timer0_Delay100us(1);
//  SPI_CE=0;
//  get_status();
//	spi_WREG(FLUSH_RX,NOP);                    //Clear the RX_FIFO
//	spi_WREG(FLUSH_TX,NOP);                    //Clear the TX_FIFO







