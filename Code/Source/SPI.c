//**************************************************************************
//  SPI_ N76
//**************************************************************************
#include <SPI.h>
#include <GPIO.h>
#include <N76E003.H>


void SPI_Initial(void){ 
//	set_DISMODF;								// SS General purpose I/O ( No Mode Fault ) 
//  //clr_SSOE;
//	/***/ // настройка управлением cs
//	//clr_LSBFE;                    // 1 младший бит       
//  set_LSBFE;
//	/***/ // настройка отправки данных начинается с msb
//	clr_CPOL;                     // 
//  clr_CPHA;                     // 
//	/***/ // настройки 2 битов отвечающие за вид clk
//	set_MSTR;                     // SPI in Master mode 
//	SPICLK_DIV16;                 // Select SPI clock : 1M bit/s
//	set_SPIEN;                    // Enable SPI function (настраивать в конце!)
//  /***/ // настройка режимов SPI
//  //clr_SPIF;
//	
SPICLK_DIV2;                        			// Select SPI clock 
	
		set_DISMODF;// SS General purpose I/O ( No Mode Fault ) 
		set_MSTR;		// SPI in Master mode 
		clr_CPOL;		// The SPI clock is low in idle mode
		set_CPHA;		// The data is sample on the second edge of SPI clock 
		set_SPIEN;	// Enable SPI function 

}

unsigned char SPI_Send(unsigned char sendData){
	unsigned char buffer;
	SPDR = sendData;
	while(!(SPSR & SET_BIT7));
	//Timer0_Delay100us(10);
	//buffer = SPDR;
	clr_SPIF;
	return buffer;
}

unsigned char SPI_Read(unsigned char receiveData){
	//CS= 0;
	PCON |= SET_BIT0;         // Enter idle mode
	//receiveData = SPDR;
	//  need clr_SPIF;
	//CS = 1;
	return SPDR;
}




