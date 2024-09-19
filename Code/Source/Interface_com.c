//********************************************************
//  Interface common
//********************************************************
#include <Interface_com.H>
#include <N76E003.H>

unsigned char valueBufferTx = 0x00;  // ��������� ����� ������ �����
unsigned char valueBufferRx = 0x00;  // ����� ����� ��������

unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE] = {0};  // ��������� ����� ������ �����
unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE] = {0};  // ����� ����� ��������(������ +1)
 
int counterBufferSpi             = 0;   // ������������ � interrupt 1
int counterBufferSpiPacket       = 0;
bit FlagInComSPIexchangeContinue = 0;   // ���������� �� �����
bit FlagInComSPIGlobal           = 1;   // 
bit FlagInComSPIexchangeByte     = 0;   // 
bit FlagInComSPIexchangeArray    = 0;   // 

/* ������������� �������(� ��� ������) */
void InCom_SPI_init_Timer(){
	TIMER0_MODE1_ENABLE;			// ����� - 16 ������ ������
	TH0 = FREQ_H;           	// ��������� �������
  TL0 = FREQ_L; 						// 
	set_ET0;									// enable Timer0 interrupt
	set_EA;										// enable interrupts
	set_TR0;                  // Timer0 run
}



/* ��������� spi ����� ������ /������� */
void InCom_SPI_exchange(void){
	/******************************/
	if(FlagInComSPIexchangeByte){
		InCom_SPI( 
		InCom_SPI_Output_in_buffer(
		&valueBufferTx),  // ������ ��� ��������
		&valueBufferRx);	// ������ ��� ������
		return;
	}
	/******************************/
	if(FlagInComSPIexchangeArray){
		if( counterBufferSpiPacket == 0) {              // �������� ����������� �������� �������
			FlagInComSPIexchangeContinue = 1;             // ��� ���� ����� �� ������� cs
		}
		InCom_SPI( 
		InCom_SPI_Output_in_buffer(
		&valueBufferArrayTx[counterBufferSpiPacket]),   // ������ ��� ��������
		&valueBufferArrayRx[counterBufferSpiPacket]);		// ������ ��� ������;
		if(counterBufferSpiPacket + 1 == BUFFER_SPI_MASSIV_SIZE){
			FlagInComSPIexchangeContinue = 0;             // ����� ��������( ��������� �����������)
		}
		return;
	}
}


/* ������ �����( ������ � ��������) */
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer){ 
	int read = 3;                 // ���������� ������ ����� MISO
	/******************************/
	if(counterBufferSpi == 0) {}  //  CPOL - ��������	 (��� slk)
	else {
		PIN_CLK_SPI =~ PIN_CLK_SPI; // ������ ������(� �������)������ ���������
	}
	if( PIN_CLK_SPI == 0) {       //  CPHA - ��������
	/******************************/
		switch(counterBufferSpi){		// ������ ������		
			/*��������� ��� ������*/
			case BUFFER_SPI:          // ����� ������ 
				if(FlagInComSPIexchangeContinue){ // ���������� �� ����� �� ���������� cs
					counterBufferSpiPacket ++;      //��������� �����
				}
				else{
					PIN_CS_SPI   = 1;
					PIN_MOSI_SPI = 1;
					PIN_MISO_SPI = 1;
					PIN_CLK_SPI  = 0;
					FlagInComSPIexchangeArray = 0; // ��������� �����
					FlagInComSPIGlobal        = 0; // ��������� ��� 
					counterBufferSpiPacket    = 0; // ��������� ����� ��������					
				}
				counterBufferSpi = 0;
				break;
			/*��� ��������� ���� ������*/
			default:                            // ���� �����		
				PIN_CS_SPI   = 0;
				if (valueMosi) {PIN_MOSI_SPI =1;} // ��������
				else{PIN_MOSI_SPI = 0;}
				while(read--){                    // ������
					if ( PIN_MISO_SPI == 1){        // ���� 1 ( ��������)
						InCom_SPI_Input_in_buffer(outSideBuffer);
					}	
				}
				counterBufferSpi ++; // ��������� ���
				/*break;*/
		}
	}
}



/* ���������� � ����� ������ ����������� ��� */
void InCom_SPI_Input_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;
	if(counterBufferSpi == 0){                // ��������� ���
		if(SPI_MSB){
			*outSideBuffer = (buf & 0xFE) << 7;   // ������� ��� - ���������� �� �������
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




/* ������ �� ������ �������� � ��������� ��������*/
bit InCom_SPI_Output_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  // ��������� ������� �������� ��������� ��������
	if (counterBufferSpi == 0){		      // �������� 1 ��� ��� ��������
		if(SPI_MSB){ buf = buf & 0x01; }  // ����� ��� MSB
		else       { buf = buf & 0x80; }  // ����� ��� LSB
	}
	if( counterBufferSpi < 8 && counterBufferSpi != 0){	// ����������� ���� ��� �������� 
		buf = buf & (0x01 << counterBufferSpi);
	}
	if(buf) { return 1; } else { return 0; } // ���� 0x00 =>0,� ���� ��� =>1
}



/* �������� ����� �������� ������ ������*/
unsigned char *InCom_SPI_byte_received(){return &valueBufferRx;}

/* �������� ����� ������� ������ ������*/
unsigned char *InCom_SPI_array_received(){return &valueBufferArrayRx;}

/* ������ �������� ������ ��������*/
void InCom_SPI_byte_transitive(unsigned char *byte){
	valueBufferTx = *byte;
}


/* ������ �������� ������� ������ ��������*/
void InCom_SPI_array_transitive(unsigned char *array){
	int i;
	for(i = 0; i> BUFFER_SPI_MASSIV_SIZE ; i++){
		if( i >= BUFFER_SPI_MASSIV_SIZE) {return;}  //��������
		valueBufferArrayTx[i] = *(array + i);
	}
}





/* �������� ����� ��� ������*/
void InCom_SPI_byte_received_clear(){ valueBufferRx = 0x00;}

/* �������� ����� ��� ��������*/
void InCom_SPI_byte_transitive_clear(){ valueBufferTx = 0x00;}

/* �������� ������ ������ ��� ��������*/
void InCom_SPI_array_transitive_clear(){InCom_Array_clear(&valueBufferArrayTx);}

/* �������� ������ ������ ��� ������*/
void InCom_SPI_array_received_clear(){InCom_Array_clear(&valueBufferArrayRx);}

/* ��������������� ������� ��� ������� ������� */
void InCom_Array_clear(unsigned char *array){
	int i;
	for(i = 0; i> BUFFER_SPI_MASSIV_SIZE ; i++){
		if( i >= BUFFER_SPI_MASSIV_SIZE) {return;}  //��������
		*(array + i) = 0x00;
	}
}
	

/* ���������� �������� clk */
void InCom_SPI_CLK_init(bit init){ PIN_CLK_SPI = init;}

/*��������� ��������� ����(�������������� ����������)*/
void InCom_SPI_send_byte(){
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeByte = 1;
}


/*��������� ��������� ������(�������������� ����������)*/
void InCom_SPI_send_array(){
	FlagInComSPIGlobal = 1;
	FlagInComSPIexchangeArray = 1;
}


























