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


extern int mainStateProgress;

void Work_NRF(void);
void ISR_Timer0(void);


#endif