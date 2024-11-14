//********************************************************
// TIMER-s init
//********************************************************

#include <Timers.h>
#include <N76E003.h>

/* Init timer and start */
void Init_Timer0(){
	TIMER0_MODE2_ENABLE;			// Mode timer
	TH0 = FREQ_H;           	// setting freq:
  TL0 = FREQ_L; 						// 
	set_ET0;									// enable Timer0 interrupt
	set_TR0;                  // Timer0 run
}


