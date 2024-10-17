//********************************************************
//  MAIN 
//********************************************************

#include <CENCOR_PROGRAMM.h>

void main(){
	//int count = 0;
	
	/****************/
	/*		init      */
	/****************/	
	init_device();
	/****************/
	/* start work   */
	/****************/
	//FlagSPIGlobal = 1;
	/****************/
	SPI_Delay_Set(100);
  while(1){
	/****************/
	/*  main work   */
	/****************/

		if(FlagSPIGlobal==0){PIN_CLK_SPI = 1;}
		
		//PIN_CLK_SPI = TactTimer0;
		//Work_NRF();
		//PIN_LED_RED = Button_Read();  //чтение кнопки
	}
}

