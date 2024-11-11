//**************************************************************************
//  similar I2C For HX711
//**************************************************************************

#include <I2C_ADC_HX711.h>
#include <GPIO.h>
#include <N76E003.H>

void ADCHX711_Initial(void){
	//ADDO = 1;												// ADDO если подать в 0 вернется к 
	ADSK = 0;													// первоначальным настройкам (подать)
	while(ADOUT);                     // Cделать первое чтение
	ADCHX711_Read();									// для установки настроек
}

unsigned long ADCHX711_Read(void){
	unsigned long Count;
	/*unsigned char i;
	//прочитать -> { ADDO = 0 } -> начать извлечение
	ADSK = 0;
	Count = 0;
	for(i = 0; i < SIZE_DATA; i++ ){
		if(ADOUT) Count++;
		ADSK = 1;
		Count = Count<<1;
		ADSK = 0;
	}
	// cделать оставшиеся исмпульсы согласно настройке
	for(i = 0; i < ( PD_SCK - SIZE_DATA ); i++ ){
		ADSK = 1;
		_nop_();
		ADSK = 0;
	}*/
	Count = 0x800000;  
	return Count;
}






