//********************************************************
//  CENCOR. 
//********************************************************

#ifndef CENCOR_PROGRAMM
#define CENCOR_PROGRAMM

#include <GPIO.h>
#include <CONFIG.h>
#include <N76E003.h>
#include <NRF24L01.H>
#include <Interface_com.H>

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



#endif