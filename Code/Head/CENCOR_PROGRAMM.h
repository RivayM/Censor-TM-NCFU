//********************************************************
//  CENCOR. 
//********************************************************

#ifndef CENCOR_PROGRAMM
#define CENCOR_PROGRAMM


//********************************************************
/*  INCLUDE   If include this project 1=yes , 0=no			*/
//********************************************************
#define USE_RADIO 1     //

#include <SPI_prog.h>

	#ifdef USE_RADIO
		#include <NRF24L01.h>
	#endif /*include radio in project*/


#include <I2C_ADC_HX711.h>

#include <GPIO.h>

#include <N76E003.H>

#include <CONFIG.h>

//#include <Driver_LCD1602.H>
//#include <I2C_prog.H>
//#include <Timer0.H>


//********************************************************
// MAIN FUNC - work censor
//********************************************************

void init_device(void);

//********************************************************
//  Work with NRF and FRAM 
//********************************************************

extern xdata int mainStateProgressSPI;

void Work_NRF(void);
void ISR_Timer0(void);
void Process_init_RF(void);
void Process_work_RF(void);
//********************************************************
//  Work with ADC
//********************************************************


extern xdata int mainStateProgressADC;



#endif /*CENCOR_PROGRAMM*/