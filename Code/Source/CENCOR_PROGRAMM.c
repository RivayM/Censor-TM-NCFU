//********************************************************
//  CENCOR. 
//********************************************************

#include <CENCOR_PROGRAMM.h>

xdata int mainStateProgressADC = 0;		// number current process adc

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
//  Work with NRF and FRAM (spi)
//********************************************************
#ifdef USE_RADIO
xdata int mainStateProgressSPI = 0;  	// number current process spi

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
	
	// ���� ��������� ������ � �������������� �������
	// � ����� ��������� ���������� �� ���� ( ���������)
	// ����� ��� , �� ��������� ������ � ���������� ���������
	// ����������
}

#endif /*include radio in project*/
//********************************************************
// ISR SPI
//********************************************************
void ISR_Timer0() interrupt 1 {  				// <Interface_com.H>
	#ifdef USE_RADIO
	if(FlagSPIGlobal){ 
			if(FlagSPIDelay){
				SPI_Delay(); 						// delay SPI
			} 
			else{
				SPI_exchange_start();		// exchange SPI
			} 
	}
	#endif /*include radio in project*/
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






