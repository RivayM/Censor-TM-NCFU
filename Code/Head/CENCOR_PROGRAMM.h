//********************************************************
//  CENCOR. 
//********************************************************

#ifndef CENCOR_PROGRAMM
#define CENCOR_PROGRAMM

//********************************************************
/*  INCLUDE   If include this project 1 = yes , 0 = no	*/
//********************************************************
#define USE_RADIO 			1    				//
#define USE_ADC 				1    				//
#define USE_PERIPHERY		0    	 			//
#define USE_FRAM 				0    				//
#define USE_DISPLAY			0    	 			//

#include <SPI_prog.h>
#include <Timers.h>
#include <GPIO.h>
#include <N76E003.H>
#include <CONFIG.h>
#include <I2C_prog.h>

#if USE_RADIO
	#include <NRF24L01.h>
#endif /*include Radio in project*/
	
#if USE_ADC
	#include <ADC_HX711.h>
#endif /*include ADC in project*/

#if USE_PERIPHERY
	#include <Periphery.h>
#endif /*include ADC in project*/
	
#if USE_DISPLAY
	// #include <Display.H>
#endif /*include Display in project*/
	
#if USE_FRAM
	// #include <FRAM.H>
#endif /*include FRAM in project*/

//********************************************************
// MAIN FUNC - work censor
//********************************************************

void init_device(void);

//********************************************************
//  Work with NRF and FRAM 
//********************************************************

extern xdata int mainStateProgressSPI;


void ISR_Timer0(void);

void Work_NRF(void);
void Process_init_RF(void);
void Process_work_RF(void);

void Work_NRF(void);
void Process_init_ADC(void);
void Process_work_ADC(void);

//********************************************************
//  Work with ADC
//********************************************************

extern xdata int mainStateProgressADC;

#endif /*CENCOR_PROGRAMM*/