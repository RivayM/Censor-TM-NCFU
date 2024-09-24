//********************************************************
//  Interface common
//********************************************************
#include <Interface_com.H>
#include <N76E003.H>

unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  // прочитать можно только тогда
unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  // когда обмен закончен(размер +1)

int amountByteArrayForSend = BUFFER_SPI_MASSIV_SIZE; // количество байт которое надо отправить
int counterBit             = 0;   // использовать в interrupt 1
int counterByte            = 0;
bit FlagInComSPIGlobal     = 0;   // 

/* Инициализация таймера(и его запуск) */
void InCom_SPI_init_Timer(){
	TIMER0_MODE1_ENABLE;			// Режим - 16 битный таймер
	TH0 = FREQ_H;           	// настройка частоты
  TL0 = FREQ_L; 						// 
	set_ET0;									// enable Timer0 interrupt
	set_EA;										// enable interrupts
	set_TR0;                  // Timer0 run
}

/* запустить spi обмен пакета /пакетов */
void InCom_SPI_exchange(void){
	InCom_SPI( 
		InCom_SPI_Output_in_buffer(
			&valueBufferArrayTx[0]),   // буффер для передачи
		&valueBufferArrayRx[0]);	    // буффер для приема;
}

/* начать обмен( чтения и передача) */
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer){ 
	int read = 2;                 // количество чтений линии MISO
	/******************************/
	if(counterBit == 0) {}  //  CPOL - имитация	 (для slk)
	else {
		PIN_CLK_SPI =~ PIN_CLK_SPI; // каждый запуск(в таймере)менять состояние
	}
	if( PIN_CLK_SPI == 0) {       //  CPHA - имитация
	/******************************/
		switch(counterBit){		// Начало обмена		
			/*Последний бит пакета*/
			case BUFFER_SPI:          // Конец обмена 
				PIN_CS_SPI   = 1;
				PIN_MOSI_SPI = 1;
				PIN_MISO_SPI = 1;
				PIN_CLK_SPI  = 0;
				counterBit = 0;
				FlagInComSPIGlobal = 0; // Завершить все 
				break;
			/*Все остальные биты пакета*/
			default:                            // Идет обмен		
				PIN_CS_SPI   = 0;
				PIN_MOSI_SPI = valueMosi; // отправка
				
					if ( PIN_MISO_SPI == 1){       
						InCom_SPI_Input_in_buffer(outSideBuffer);
					}	
				
				counterBit ++; // следующий бит
				break;
		}
	}
}


/* складывать в буфер приема прочитанный бит */
void InCom_SPI_Input_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;
	if(counterBit == 0){                // начальный бит
		if(SPI_MSB){
			*outSideBuffer = 0x80;   // младщий бит - установить на старший
			return;
		}
	}
	if(counterBit < BUFFER_SPI){
		if(SPI_MSB){
			buf = buf + (0x01 << counterBit -1);
		}
		else {
			buf = buf + (0x01 << counterBit);
		}
		*outSideBuffer = buf;	
	}
}


/* читать из буфера передачи и отправить значение*/
bit InCom_SPI_Output_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  // сохранить текущее значение буфера для передачи
	if (counterBit == 0){		      // получить 1 бит для отправки
		if(SPI_MSB){ buf = buf & 0x01; }  // маска для MSB
		else       { buf = buf & 0x80; }  // маска для LSB
	}
	if( counterBit < 8 && counterBit != 0){	// последующие биты для отправки 
		buf = buf & (0x01 << counterBit);
	}
	if(buf) { return 1; } else { return 0; } // если 0x00 =>0,а если нет =>1
}

/* установить значение clk */
void InCom_SPI_CLK_init(bit init){ PIN_CLK_SPI = init;}

