//********************************************************
//  CENCOR. 
//********************************************************

#include <CENCOR_PROGRAMM.h>

int mainStateProgress = 0;  // state main func.( work cencor)


//********************************************************
void Main_Work_NRF(){
	
	if(FlagInComSPIGlobal) {    // exchange spi
		//waiting
	}
	else{
		// on each 0 ( packet send) next state
		switch(mainStateProgress){
			case START_PROGRESS: NRF_init_RX();mainStateProgress++; break;
		}
	}
}

//********************************************************
// ISR
//********************************************************
void ISR_Timer0() interrupt 1 {  // <Interface_com.H>
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}