//***********************************************************************************************************
//  PKB NCFU 
//  Date   : 2024.06.20
//***********************************************************************************************************

//***********************************************************************************************************
//  Cенсор . Узел Телеметрической системы
//***********************************************************************************************************

#include <GPIO.h>
#include <N76E003.h>
#include <SPI.h>


//#include <NRF_ex.h>

#define TX_MODE

void main(){
	/****************/	
	unsigned char a = 0x20;
	unsigned char *rec;
	unsigned char stat;
	/****************/
	SPI_Initial();
	//Set_All_GPIO_Quasi_Mode;
	/*P01_PushPull_Mode;*/ P0M1&=~SET_BIT1;P0M2|=SET_BIT1;
	/*P00_PushPull_Mode; */P0M1&=~SET_BIT0;P0M2|=SET_BIT0;
	/*P10_PushPull_Mode; */P1M1&=~SET_BIT0;P1M2|=SET_BIT0;
	/*P11_PushPull_Mode; */P1M1&=~SET_BIT1;P1M2|=SET_BIT1;
	/*P15_PushPull_Mode;*/ P1M1&=~SET_BIT5;P1M2|=SET_BIT5;
	PIN_LED_GREEN = 1;
	PIN_LED_RED = 1;
	PIN_RF_ON = 1 ;
	/****************/
	Timer0_Delay100us(100);
	SPI_CE=0;                             //Chip enable
  SPI_CSRF=1;                           //SPI disable
	/****************/
	while(1){

		SPI_CSRF= 0;
		SPI_Send(a);
		SPI_Send(a);
		SPI_CSRF=1; 
		Timer0_Delay100us(1000);
//		#ifndef RX_MODE
//		rec=RX_PL();
//		Timer0_Delay100us(1000);
//		if(*rec == a) {PIN_LED_RED =~PIN_LED_RED;}
//		PIN_LED_GREEN =~PIN_LED_GREEN;
//		#endif
//		#ifndef TX_MODE
//		stat = TX_PL(&a);
//    switch(stat){
//			case 0:{PIN_LED_GREEN =~PIN_LED_GREEN;};
//			case 1:{PIN_LED_RED =~PIN_LED_RED;};			
//		Timer0_Delay100us(1000);
//		}
//		#endif		
	}
}

