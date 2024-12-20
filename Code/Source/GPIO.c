//**************************************************************************
//  ��������� ������ ��������� �������
//**************************************************************************

#include <GPIO.h>
#include <N76E003.h>

void GPIO_init(){
	/*setting pin*/
	Set_All_GPIO_Quasi_Mode;
	P14_Input_Mode;									// PIN_BUTTON
	P17_Input_Mode;									// NRF_IRQ
	P00_PushPull_Mode; 							// SPI_MoSi
	P01_Input_Mode;									// SPI_MiSO
	/*****************************/
	//	P11_PushPull_Mode;
	//	P15_PushPull_Mode;
	//  P10_PushPull_Mode;
	//	P01_PushPull_Mode;
	//	P04_PushPull_Mode;
	//	Enable_INT_Port0
	
	P30_Input_Mode;	 				//	init pin ADCDOUT
	P04_PushPull_Mode;			//	init pin ADC_ON
	P04 = 1;								//  on adc
	P05_PushPull_Mode;			//	init pin ADCCLK 
	
	/******************************/
	
	Enable_INT_Port1;
	Enable_BIT0_LowLevel_Trig;
	//	set_EPI;										// dont work with timer 0
	set_EX1;
	
	/*start state pin*/
	//	PIN_LED_GREEN = 1;
	//	PIN_LED_RED = 1;
	//	PIN_RF_ON = 1 ;
	NRF_CE = 0;
}