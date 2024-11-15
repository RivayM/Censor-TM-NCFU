//**************************************************************************
//  Similar I2C For HX711
//**************************************************************************
/*  description:
Full-scale differential input
Voltage of :
Channel A �20mV
Channel B �40mV
_____________________________
| PD_SCK	| 	In	| 	GAIN 	|
|		25		|		A		|		128		|
|		26		|		B		|		32		|
|		27		|		A		|		64		|
|_________|_______|_________| 

*/
//**************************************************************************
/*	USE:

void ISR_INT0() interrupt 0 {  // INT0
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}

*/
//**************************************************************************

#ifndef ADC_HX711
#define ADC_HX711

#define A_CH_128						25		//
#define A_CH_64							27		//
#define B_CH_32     				26		//	

#define SIZE_DATA_I2C_ADC		24		//	Data get in PD_SCK

/*	Func	*/
unsigned long ADCHX711_Read(void);
void ADCHX711_Initial(void);


#endif