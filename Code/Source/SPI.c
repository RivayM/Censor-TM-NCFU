//**************************************************************************
//  SPI_ N76
//**************************************************************************
#include <SPI.h>
#include <GPIO.h>
#include <N76E003.H>

void SPI_Initial(void){ 
	
//	/***/ // ��������� ����������� cs
//	set_DISMODF;								// SS General purpose I/O ( No Mode Fault ) 
//  clr_SSOE;
//	/***/ // ��������� �������� ������ ���������� � msb
//	clr_LSBFE;                    // ���������� 1 ������� ���       
//  //set_LSBFE;
//	/***/ // ��������� 2 ����� ���������� �� ��� clk
//	clr_CPOL;                     //  clr_CPOL set_CPHA
//  set_CPHA;                     // 
//	/***/ // ��������� ������� SPI
//	set_MSTR;                     // SPI in Master mode 
//	SPICLK_DIV16;                 // Select SPI clock : 1M bit/s
//	set_SPIEN;                    // Enable SPI function (����������� � �����!)
//	/***/ // ��������� ���������� spi
//	clr_SPIF;
//	set_EA;
	
	  SPICLK_DIV16;                        			// Select SPI clock 
		set_DISMODF;// SS General purpose I/O ( No Mode Fault ) 
		set_MSTR;		// SPI in Master mode 
		clr_CPOL;		// The SPI clock is low in idle mode
		set_CPHA;		// The data is sample on the second edge of SPI clock 
		set_SPIEN;	// Enable SPI function 
	  clr_SPIF;
    set_EA;
}

unsigned char SPI_Send(unsigned char sendData){
	unsigned char buffer23;
	SPDR = sendData;
	while(!(SPSR & SET_BIT7));
	//buffer = SPDR;
	clr_SPIF;
	return buffer23;
}

unsigned char SPI_Read(unsigned char receiveData){
	//CS= 0;
	PCON |= SET_BIT0;         // Enter idle mode
	//receiveData = SPDR;
	//  need clr_SPIF;
	//CS = 1;
	return SPDR;
}




