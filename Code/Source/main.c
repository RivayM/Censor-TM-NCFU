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

void main(){
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
	InCom_SPI_init_Timer();
	InCom_SPI_CLK_init(0);
	/****************/
	valueBufferArrayTx[0] = 0xFA;
	FlagInComSPIGlobal = 1;          // Включить сразу же spi
	/****************/
  while(1){
		
		if(FlagInComSPIGlobal){
			InCom_SPI_exchange();
			FlagInComSPIGlobal = 0 ;
		}
		
		PIN_LED_GREEN =~PIN_LED_GREEN;
		
		if (valueBufferArrayRx[0] == 0x38){
			PIN_LED_RED = ~PIN_LED_RED;
		}
		
	}
}
//********************************************************
// ISR
//********************************************************
void ISR_Timer0() interrupt 1 {  // <Interface_com.H>
	//if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
	if (FlagInComSPIGlobal == 0){FlagInComSPIGlobal = 1;}
	
}
