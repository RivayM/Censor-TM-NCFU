//********************************************************
//  CENCOR. 
//********************************************************

#include <CENCOR_PROGRAMM.h>

int mainStateProgress = 0;  // state main func.( work cencor)


//********************************************************
// MAIN FUNC - work censor
//********************************************************

void Work_NRF(){
	
	if(FlagInComSPIGlobal) {    // exchange spi
		/*waiting*/
	}
	else{
		// on each 0 ( packet send) next state
		switch(mainStateProgress){
			/*init radio  NRF24*/
			case START_PROGRESS: 
				if(NRF_init( &packetTX) ){ //end init rf?
					mainStateProgress++;
				} 
				break; 
			/*init ADC HX711*/
			case 1: 
				
				break;
			/*ON read ADC HX711 full time*/
			case 2:

				break;
			/*Mode censor*/
			case 3:break;
			/*END main progress(return?)*/
			case END_PROGRESS:break;
		}
	}

	// ƒ–”√Œ… œ–Œ÷≈——

}


//********************************************************
// ISR SPI
//********************************************************
void ISR_Timer0() interrupt 1 {  // <Interface_com.H>
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}

//********************************************************
// ISR ADC
//********************************************************
/*void ISR_Timer0() interrupt 1 {  // <Interface_com.H>
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}*/

//********************************************************
// ISR NRF
//********************************************************
/*void ISR_Timer0() interrupt 1 {  // <Interface_com.H>
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}*/






