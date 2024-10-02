//**************************************************************************
//  настройка прочей переферии сенсора
//**************************************************************************

#include <GPIO.h>
#include <N76E003.h>

void Led_Switch_State(){}
bit Button_Read(){return PIN_BUTTON;}
int Switch_State_Power_RF(){
	return PIN_RF_ON =~PIN_RF_ON;
}

void GPIO_init(){
	Set_All_GPIO_Quasi_Mode;
	//	P11_PushPull_Mode;
	//	P15_PushPull_Mode;
	//  P10_PushPull_Mode;
	//	P01_PushPull_Mode;
	//	P04_PushPull_Mode;
	P14_Input_Mode;
	P00_PushPull_Mode;
	//PIN_LED_GREEN = 1;
	//PIN_LED_RED = 1;
	//PIN_RF_ON = 1 ;
}