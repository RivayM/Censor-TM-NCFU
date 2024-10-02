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
	/****************/
	/* start work   */
	/****************/

	/****************/
  while(1){
		Work_NRF();
		//PIN_LED_RED = Button_Read();  //чтение кнопки
	}
}

