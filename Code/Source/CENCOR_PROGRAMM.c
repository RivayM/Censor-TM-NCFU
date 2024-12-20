//********************************************************
//  CENCOR. 
//********************************************************

#include <CENCOR_PROGRAMM.h>

xdata int mainStateProgressSPI = 0;  	// number current process spi
xdata int mainStateProgressADC = 0;		// number current process adc

//********************************************************
// MAIN FUNC - work censor
//********************************************************
void init_device(){
	GPIO_init();
	
	Init_Timer0();
	SPI_CLK_init(0);
	#if	USE_RADIO
		/*		*/
	#endif
	#if USE_ADC
		/*		*/
	#endif
	#if USE_PERIPHERY
		/*		*/
	#endif
	#if USE_DISPLAY
		/*		*/
	#endif
	#if USE_FRAM
		/*		*/
	#endif

	set_EA;										// enable interrupts
}

//********************************************************
//  Work with NRF
//********************************************************
#if USE_RADIO

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
	if(NRF_init()){  	// end init rf?
		mainStateProgressSPI++;   // process ->end go to next
	}
}

/*main work NRF24*/
void Process_work_RF(void){
	if( NRF_get() ){
		
	}
	if(readBuf[1]){
			PIN_LED_GREEN = ~PIN_LED_GREEN;
	}
	
	// пока прочитать статус и самостоятельно считать
	// а после настроить прерывание по нему ( извлекать)
	// флага нет , не считывать данные и пропускать процедуру
	// считывания
}


#endif /*include radio in project*/
//********************************************************
//  Work with periphery /led/button
//********************************************************
#if USE_PERIPHERY
#endif /*include Periphery in project*/
//********************************************************
//  Work with ADC
//********************************************************
#if USE_ADC


void Work_ADC(){
	ADCHX711_Read_CH_A()
}

#endif /*include ADC in project*/
//********************************************************
//  FRAM
//********************************************************
#if USE_FRAM
#endif /*include FRAM in project*/
//********************************************************
//  DISPLAY
//********************************************************
#if USE_DISPLAY
#endif /*include DISPLAY in project*/



//********************************************************
// ISR SPI
//********************************************************
void ISR_Timer0() interrupt 1 {  				// <Interface_com.H>
	if(FlagSPIGlobal){ 
			if(FlagSPIDelay){
				SPI_Delay(); 						// delay SPI
			} 
			else{
				SPI_exchange_start();		// exchange SPI
			} 
	}
/*********************/
	if(FlagI2cStart){
		if(FlagI2cDelay){
			/*delay i2c*/
		}
		else{
			I2C_exchange_start(SEND);	// mode I2c (adc)
		}
	}
}

//********************************************************
// ISR ADC
//********************************************************
void ISR_INT0() interrupt 0 {  // INT0
	
	
	
}

//********************************************************
// ISR NRF
//********************************************************
void ISR_INT1() interrupt 2 {  // INT1
	FlagDataReadReady = 1;
	PIN_LED_RED =~ PIN_LED_RED;
}






