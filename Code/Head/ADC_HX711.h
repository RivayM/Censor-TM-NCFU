//**************************************************************************
//  Similar I2C For HX711
//**************************************************************************
/*  description:
Full-scale differential input
Voltage of :
Channel A ±20mV
Channel B ±40mV
_____________________________
| PD_SCK	| 	In	| 	GAIN 	|
|		25		|		A		|		128		|
|		26		|		B		|		32		|
|		27		|		A		|		64		|
|_________|_______|_________| 

*/

#ifndef ADC_HX711
#define ADC_HX711

#define A_CH_128						25		//
#define A_CH_64							27		//
#define B_CH_32     				26		//	

#define SIZE_DATA_I2C_ADC		24		// Data get in PD_SCK

#define ADC_SLK 		ADSK					//
#define	ADC_OUT			ADOUT 				//

extern xdata long	valueAChADC;		//
extern xdata long	valueBChADC;		//
	
static bit statusADC;							// from which channel read data
extern bit FlagDataReadyADC;			//

/*	Func	*/
bit ADCHX711_getStat(void);
void ADCHX711_Initial(void);
void ADCHX711_Read_CH_A(void);
void ADCHX711_Read_CH_B(void);
static void ADCHX711_Read(void);
static void ADCHX711_Save_value(void);

#endif /*ADC_HX711*/
