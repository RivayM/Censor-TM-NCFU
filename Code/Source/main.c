//********************************************************
//  MAIN 
//********************************************************

#include <CENCOR_PROGRAMM.h>

void main(){
	/****************/
	
	/****************/	
	GPIO_init();
	/****************/
	InCom_SPI_init_Timer();
	InCom_SPI_CLK_init(0);
	/****************/
	//NRF_send(&value,3);
	
	/****************/
	// start work
	NRF_init_RX();
	
  while(1){
		Main_Work_NRF();
		// work with ADC
		// work with FRAM
	}
}
