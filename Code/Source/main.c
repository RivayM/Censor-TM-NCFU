//********************************************************
//  MAIN 
//********************************************************

#include <CENCOR_PROGRAMM.h>

void main(){
	/****************/
	/*		init      */
	/****************/	
	GPIO_init();
	InCom_SPI_init_Timer();
	InCom_SPI_CLK_init(0);
	
	set_EA;										// enable interrupts
	/****************/
	/* start work   */
	/****************/

	/****************/
  while(1){
	/****************/
	/*  main work   */
	/****************/
		Work_NRF();
		//PIN_LED_RED = Button_Read();  //чтение кнопки
	}
}

