//********************************************************
//  Interface common
//********************************************************
#include <Interface_com.H>
#include <N76E003.H>

unsigned char valueBufferArrayTx
	[BUFFER_SPI_MASSIV_SIZE];  // ��������� ����� ������ �����
unsigned char valueBufferArrayRx
	[BUFFER_SPI_MASSIV_SIZE];  // ����� ����� ��������(������ +1)

int amountByteArrayForSend = BUFFER_SPI_MASSIV_SIZE; // ���������� ���� ������� ���� ���������
int counterBit             = 0;   // ������������ � interrupt 1
int counterByte            = 0;
bit FlagInComSPIGlobal     = 0;   // 

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
	InCom_SPI( 
		InCom_SPI_Output_in_buffer(
			&valueBufferArrayTx[0]),   // ������ ��� ��������
		&valueBufferArrayRx[0]);	    // ������ ��� ������;
}

/* ������ �����( ������ � ��������) */
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer){ 
	int read = 3;                 // ���������� ������ ����� MISO
	/******************************/
	if( PIN_CLK_SPI == 0) {       //  CPHA - ��������
	/******************************/
		switch(counterBit){		// ������ ������		
			/*��������� ��� ������*/
			case BUFFER_SPI:          // ����� ������ 
				PIN_CS_SPI   = 1;
				PIN_MOSI_SPI = 1;
				PIN_MISO_SPI = 1;
				PIN_CLK_SPI  = 0;
				FlagInComSPIGlobal = 0; // ��������� ��� 
				counterBit = 0;
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
				counterBit ++; // ��������� ���
				break;
		}
	}
	PIN_CLK_SPI =~ PIN_CLK_SPI; // ������ ������(� �������)������ ���������
}


/* ���������� � ����� ������ ����������� ��� */
void InCom_SPI_Input_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;
	if(counterBit == 0){                // ��������� ���
		if(SPI_MSB){
			*outSideBuffer = (buf & 0xFE) << 7;   // ������� ��� - ���������� �� �������
			buf = buf >> 1;
			*outSideBuffer = *outSideBuffer | buf;
		}
		return;
	}
	if(counterBit < BUFFER_SPI){
		if(SPI_MSB){
			counterBit = counterBit - 1;
		}
		buf = buf + (0x01 << counterBit);
		*outSideBuffer = buf;	
	}
}


/* ������ �� ������ �������� � ��������� ��������*/
bit InCom_SPI_Output_in_buffer(unsigned char *outSideBuffer){
	unsigned char buf;
	buf = *outSideBuffer;  // ��������� ������� �������� ������ ��� ��������
	if (counterBit == 0){		      // �������� 1 ��� ��� ��������
		if(SPI_MSB){ buf = buf & 0x01; }  // ����� ��� MSB
		else       { buf = buf & 0x80; }  // ����� ��� LSB
	}
	if( counterBit < 8 && counterBit != 0){	// ����������� ���� ��� �������� 
		buf = buf & (0x01 << counterBit);
	}
	if(buf) { return 1; } else { return 0; } // ���� 0x00 =>0,� ���� ��� =>1
}

/* ���������� �������� clk */
void InCom_SPI_CLK_init(bit init){ PIN_CLK_SPI = init;}

