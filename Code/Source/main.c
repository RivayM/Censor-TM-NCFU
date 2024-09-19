//********************************************************
//  PKB NCFU 
//  Date   : 2024.06.20
//********************************************************
//********************************************************
//  Cенсор . Узел Телеметрической системы
//********************************************************

#include <GPIO.h>
#include <CONFIG.h>
#include <N76E003.h>

#include <Interface_com.H>

#define TX_mode

void main(){
	/****************/	
	unsigned char *rec;
	unsigned char a = 0x56;
	unsigned char stat = 0x24;
	unsigned char b[BUFFER_SPI_MASSIV_SIZE] = {0x7A,0x99,0x4E,0x22,0xFF};
	/****************/
	Set_All_GPIO_Quasi_Mode;
//	P11_PushPull_Mode;
//	P15_PushPull_Mode;
//	P10_PushPull_Mode;
	P00_PushPull_Mode;
//	P01_PushPull_Mode;
//	P04_PushPull_Mode;
	PIN_LED_GREEN = 1;
	PIN_LED_RED = 1;
	PIN_RF_ON = 1 ;
	/****************/  
	//SPI_Initial;
  //nRF_config();	
	/****************/
	InCom_SPI_init_Timer();
	InCom_SPI_CLK_init(0);
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeArray = 1;
	//FlagInComSPIexchangeByte = 1;
	InCom_SPI_byte_transitive(&a);
	valueBufferArrayTx[5] = b[5];
//	valueBufferArrayTx[0] = 0x7A;
//	valueBufferArrayTx[1] = 0x99;
//	valueBufferArrayTx[2] = 0x4E;
//	valueBufferArrayTx[3] = 0x22;
//	valueBufferArrayTx[4] = 0xFF;
	
  while(1){
		if (FlagInComSPIGlobal == 0){
			InCom_SPI_byte_transitive(&a);
			FlagInComSPIGlobal = 1;
			FlagInComSPIexchangeArray = 1;
			//FlagInComSPIexchangeByte = 1;
		}

		Timer3_Delay100ms(10);
		PIN_LED_GREEN =~PIN_LED_GREEN;
		if (valueBufferRx == 0x38){
			PIN_LED_RED = ~PIN_LED_RED;
		}
		valueBufferRx = 0x00;
		
	}
}
//********************************************************
// ISR
//********************************************************
void ISR_Timer0()interrupt 1{  // <Interface_com.H>
	if(FlagInComSPIGlobal == 1){ // обмен по SPI - 1 пакет
	    InCom_SPI_exchange();
	}
}
