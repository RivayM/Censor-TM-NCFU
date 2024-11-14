//*******************************************************
// TIMER-s init
//********************************************************

//********************************************************
// (TIMER0) set FREQ  
//********************************************************
#ifndef TIMERS
#define TIMERS

#define FREQ_030KHZ_H 0xFF
#define FREQ_030KHZ_L 0xF9
#define FREQ_120KHZ_H 0xFF
#define FREQ_120KHZ_L 0xFB
#define FREQ_480KHZ_H 0xFF
#define FREQ_480KHZ_L 0xFD

/*USE FREQ IN void init_CLK*/
#define FREQ_H FREQ_030KHZ_H         // setting for freq
#define FREQ_L FREQ_030KHZ_L

/*  USE:
		void ISR_Timer0() interrupt 1 {
			if(FlagInComSPIGlobal){ 
			}
		}  
*/

/*FUNC */
void Init_Timer0(void);

#endif /*TIMERS*/











