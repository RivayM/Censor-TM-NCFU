//********************************************************
//  CENCOR. 
//********************************************************

#include <CENCOR_PROGRAMM.h>

xdata int mainStateProgressSPI = 0;  	// state main func.( work cencor)
xdata int modeRF = RX_MODE;						// mode by default

//********************************************************
// MAIN FUNC - work censor
//********************************************************

//********************************************************
//  Work with periphery /led/button
//********************************************************

//********************************************************
//  Work with ADC
//********************************************************


//********************************************************
//  Work with NRF and FRAM 
//********************************************************

void Work_NRF(){
	
	if(FlagInComSPIGlobal) {    // exchange spi
		/*waiting*/
	}
	else{
		switch(mainStateProgressSPI){
			case START_PROCESS: 	
				Process_init_RF();
				break; 				
			/*main work RF*/
			case 1: 
				Process_work_RF();
				break;
			// case 2: init FRAM
			// case 3: Work with FRAM
			case END_PROCESS:break;
		}
	}
}

/*init radio  NRF24*/
void Process_init_RF(void){
	switch(modeRF){
		case TX_MODE:
			if(NRF_init(&packetTX)){  	// end init rf?
				mainStateProgressSPI++;   // process ->end go to next
			}
		case RX_MODE:
			if(NRF_init(&packetRX)){  	// end init rf?
				mainStateProgressSPI++;   // process ->end go to next
			}
		default:break;
	}
}

/*main work RF*/
void Process_work_RF(void){
	if( NRF_get() ){
		/*mainStateProgressSPI++;*/
	}
	
	// пока прочитать статус и самостоятельно считать
	//	а после настроить прерывание по нему ( извлекать)
	//	флага нет , не считывать данные и пропускать процедуру
	// считывания
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
/*void ISR_INT0() interrupt 1 {  // INT0
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}*/

//********************************************************
// ISR NRF
//********************************************************
void ISR_INT1() interrupt 2 {  // INT1
	FlagDataReadReady = 1;
	PIN_LED_RED = ~PIN_LED_RED;
}






