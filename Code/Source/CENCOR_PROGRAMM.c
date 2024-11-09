//********************************************************
//  CENCOR. 
//********************************************************

#include <CENCOR_PROGRAMM.h>

xdata int mainStateProgressSPI = 0;  	// state main func.( work cencor)
xdata int modeRF = RX_MODE;						// mode by default

bit tact = 0;

//********************************************************
// MAIN FUNC - work censor
//********************************************************
void init_device(){
	GPIO_init();
	SPI_init_Timer();
	SPI_CLK_init(0);
	
	set_EA;										// enable interrupts
}

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
	if(FlagSPIGlobal) {    				// work spi
		/*	waiting	*/
	}
	else{
		switch(mainStateProgressSPI){
			case START_PROCESS: 	Process_init_RF();		break; 				
			case 1: Process_work_RF();									break;
			// case 2: init FRAM
			// case 3: Work with FRAM
			case END_PROCESS:break;
		}
	}
}

/*init radio  NRF24*/
void Process_init_RF(void){
	if(NRF_init(&packetRX)){  	// end init rf?
		mainStateProgressSPI++;   // process ->end go to next
	}
}

/*main work RF*/
void Process_work_RF(void){
	if( NRF_get() ){
		PIN_LED_RED = ~PIN_LED_RED;
	}
	if(readBuf[1] != 0x30){
			PIN_LED_GREEN = ~PIN_LED_GREEN;
	}
	
	// пока прочитать статус и самостоятельно считать
	// а после настроить прерывание по нему ( извлекать)
	// флага нет , не считывать данные и пропускать процедуру
	// считывания
}

//********************************************************
// ISR SPI
//********************************************************
void ISR_Timer0() interrupt 1 {  				// <Interface_com.H>
	if(FlagSPIGlobal){ 
		//TactTimer0 =~ TactTimer0;
		
			if(FlagSPIDelay){
				SPI_Delay(); 						// delay SPI
			} 
			else{
				SPI_exchange_start();		// exchange SPI
			} 
	}
}

//********************************************************
// ISR ADC
//********************************************************
/*void ISR_INT0() interrupt 0 {  // INT0
	if(FlagInComSPIGlobal){ InCom_SPI_exchange();}
}*/

//********************************************************
// ISR NRF
//********************************************************
void ISR_INT1() interrupt 2 {  // INT1
	FlagDataReadReady = 1;
	PIN_LED_RED =~ PIN_LED_RED;
}






