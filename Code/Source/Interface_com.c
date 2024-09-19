//********************************************************
//  Interface common
//********************************************************
#include <Interface_com.H>
#include <N76E003.H>

unsigned char valueBufferTx = 0x00;  // прочитать можно только тогда
unsigned char valueBufferRx = 0x00;  // когда обмен закончен

unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE] = {0};  // прочитать можно только тогда
unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE] = {0};  // когда обмен закончен(размер +1)
 
int counterBufferSpi             = 0;   // использовать в interrupt 1
int counterBufferSpiPacket       = 0;
bit FlagInComSPIexchangeContinue = 0;   // продолжить ли обмен
bit FlagInComSPIGlobal           = 1;   // 
bit FlagInComSPIexchangeByte     = 0;   // 
bit FlagInComSPIexchangeArray    = 0;   // 

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
	/******************************/
	if(FlagInComSPIexchangeByte){
		InCom_SPI( 
		InCom_SPI_Output_in_buffer(
		&valueBufferTx),  // буффер для передачи
		&valueBufferRx);	// буффер для приема
		return;
	}
	/******************************/
	if(FlagInComSPIexchangeArray){
		if( counterBufferSpiPacket == 0) {              // Включить продолжение отправки пакетов
			FlagInComSPIexchangeContinue = 1;             // для того чтобы не дергать cs
		}
		InCom_SPI( 
		InCom_SPI_Output_in_buffer(
		&valueBufferArrayTx[counterBufferSpiPacket]),   // буффер для передачи
		&valueBufferArrayRx[counterBufferSpiPacket]);		// буффер для приема;
		if(counterBufferSpiPacket + 1 == BUFFER_SPI_MASSIV_SIZE){
			FlagInComSPIexchangeContinue = 0;             // Обмен закончен( выключить продолжение)
		}
		return;
	}
}


/* начать обмен( чтения и передача) */
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer){ 
	int read = 3;                 // количество чтений линии MISO
	/******************************/
	if(counterBufferSpi == 0) {}  //  CPOL - имитация	 (для slk)
	else {
		PIN_CLK_SPI =~ PIN_CLK_SPI; // каждый запуск(в таймере)менять состояние
	}
	if( PIN_CLK_SPI == 0) {       //  CPHA - имитация
	/******************************/
		switch(counterBufferSpi){		// Начало обмена		
			/*Последний бит пакета*/
			case BUFFER_SPI:          // Конец обмена 
				if(FlagInComSPIexchangeContinue){ // продолжать ли обмен не переключая cs
					counterBufferSpiPacket ++;      //следующий пакет
				}
				else{
					PIN_CS_SPI   = 1;
					PIN_MOSI_SPI = 1;
					PIN_MISO_SPI = 1;
					PIN_CLK_SPI  = 0;
					FlagInComSPIexchangeArray = 0; // завершить обмен
					FlagInComSPIGlobal        = 0; // Завершить все 
					counterBufferSpiPacket    = 0; // последний пакет завершен					
				}
				counterBufferSpi = 0;
				break;
			/*Все остальные биты пакета*/
			default:                            // Идет обмен		
				PIN_CS_SPI   = 0;
				if (valueMosi) {PIN_MOSI_SPI =1;} // отправка
				else{PIN_MOSI_SPI = 0;}
				while(read--){                    // чтение
					if ( PIN_MISO_SPI == 1){        // если 1 ( записать)
						InCom_SPI_Input_in_buffer(outSideBuffer);
					}	
				}
				counterBufferSpi ++; // следующий бит
				/*break;*/
		}
	}
}



/* складывать в буфер приема прочитанный бит */
void InCom_SPI_Input_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;
	if(counterBufferSpi == 0){                // начальный бит
		if(SPI_MSB){
			*outSideBuffer = (buf & 0xFE) << 7;   // младщий бит - установить на старший
			buf = buf >> 1;
			*outSideBuffer = *outSideBuffer | buf;
		}
		return;
	}
	if(counterBufferSpi < BUFFER_SPI){
		if(SPI_MSB){
			counterBufferSpi = counterBufferSpi - 1;
		}
		buf = buf + (0x01 << counterBufferSpi);
		*outSideBuffer = buf;	
	}
}




/* читать из буфера передачи и отправить значение*/
bit InCom_SPI_Output_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  // сохранить текущее значение буферадля передачи
	if (counterBufferSpi == 0){		      // получить 1 бит для отправки
		if(SPI_MSB){ buf = buf & 0x01; }  // маска для MSB
		else       { buf = buf & 0x80; }  // маска для LSB
	}
	if( counterBufferSpi < 8 && counterBufferSpi != 0){	// последующие биты для отправки 
		buf = buf & (0x01 << counterBufferSpi);
	}
	if(buf) { return 1; } else { return 0; } // если 0x00 =>0,а если нет =>1
}



/* получить адрес значения буфера приема*/
unsigned char *InCom_SPI_byte_received(){return &valueBufferRx;}

/* получить адрес массива буфера приема*/
unsigned char *InCom_SPI_array_received(){return &valueBufferArrayRx;}

/* задать значение буфера передачи*/
void InCom_SPI_byte_transitive(unsigned char *byte){
	valueBufferTx = *byte;
}


/* задать значение массива буфера передачи*/
void InCom_SPI_array_transitive(unsigned char *array){
	int i;
	for(i = 0; i> BUFFER_SPI_MASSIV_SIZE ; i++){
		if( i >= BUFFER_SPI_MASSIV_SIZE) {return;}  //проверка
		valueBufferArrayTx[i] = *(array + i);
	}
}





/* очистить буфер для приема*/
void InCom_SPI_byte_received_clear(){ valueBufferRx = 0x00;}

/* очистить буфер для передачи*/
void InCom_SPI_byte_transitive_clear(){ valueBufferTx = 0x00;}

/* очистить массив буфера для передачи*/
void InCom_SPI_array_transitive_clear(){InCom_Array_clear(&valueBufferArrayTx);}

/* очистить массив буфера для приема*/
void InCom_SPI_array_received_clear(){InCom_Array_clear(&valueBufferArrayRx);}

/* Вспомогательная функция для очистки массива */
void InCom_Array_clear(unsigned char *array){
	int i;
	for(i = 0; i> BUFFER_SPI_MASSIV_SIZE ; i++){
		if( i >= BUFFER_SPI_MASSIV_SIZE) {return;}  //проверка
		*(array + i) = 0x00;
	}
}
	

/* установить значение clk */
void InCom_SPI_CLK_init(bit init){ PIN_CLK_SPI = init;}

/*Разрешить отправить байт(предварительно записанный)*/
void InCom_SPI_send_byte(){
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeByte = 1;
}


/*Разрешить отправить массив(предварительно записанный)*/
void InCom_SPI_send_array(){
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeArray = 1;
}


























