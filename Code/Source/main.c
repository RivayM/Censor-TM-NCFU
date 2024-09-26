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
#include <NRF24L01.H>
#include <Interface_com.H>

void main(){
	/****************/
	unsigned char value[5] = {0x02,0x04,0x06,0x08};
	
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
	//NRF_send(&value,3);
	
	/****************/
	NRF_init_RX();
	
  while(1){
		//подумать о стейт машине я здесь запускаю безумено
		// долго повторяющщийся цикл
	}
}
//********************************************************
// ISR
//********************************************************
void ISR_Timer0() interrupt 1 {  // <Interface_com.H>
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}
