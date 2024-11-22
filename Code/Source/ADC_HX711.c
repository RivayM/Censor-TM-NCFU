//**************************************************************************
//  similar I2C For HX711
//**************************************************************************

#include <ADC_HX711.h>
#include <I2C_prog.h>
#include <N76E003.H>

bit FlagDataReadyADC 	= 0;
bit statusADC 				= 0; // 0 -> A ; 1 -> B

xdata long	valueAChADC	= 0;
xdata long	valueBChADC	= 0;

bit ADCHX711_getStat(){return statusADC;}

void ADCHX711_Initial(void){
	/*	init GPIO				*/
	/*	ADC_OUT as INT 	*/
}

//**************************************************************************
// READ value ( start read when INT = 0);
//**************************************************************************
	/* 
	
	in the START ADC call func, will be:
	0. ADCHX711_Read_CH_A() -> this you get value chA
	1. ADCHX711_Read_CH_B() -> this you get value chA
	2. ADCHX711_Read_CH_A()	-> this you get value chB
	3. ADCHX711_Read_CH_B() -> this you get value chA
	
	NEED:
	0. ADCHX711_Read_CH_B() -> this you get value chA
	1. ADCHX711_Read_CH_A() -> this you get value chB
	2. ADCHX711_Read_CH_B() -> this you get value chA
	
	*The read value goes to the next sequence of 
		clock cycles and not to 1!	

	*/

void ADCHX711_Read_CH_A(){
	statusADC = 0;
	I2C_change_size_BUF(A_CH_64);
	I2C_start();
	ADCHX711_Read();
}

void ADCHX711_Read_CH_B(){
	statusADC = 1;
	I2C_change_size_BUF(B_CH_32);
	I2C_start();
	ADCHX711_Read();
}

//**************************************************************************
// INT
//**************************************************************************

/*Use func in ISR - in 0*/
void ADC_interrupt(){
	FlagDataReadyADC = 1;
	//statusADC =~ statusADC;
}

//**************************************************************************
// I2c
//**************************************************************************

/*Read value as I2C*/
static void ADCHX711_Read(){
	if(FlagDataReadyADC){
		I2C_exchange_start(READ);
	}
}

/*Save value as I2C*/
static void ADCHX711_Save_value(){
	

}

