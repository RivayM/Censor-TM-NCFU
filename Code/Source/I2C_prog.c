//**************************************************************************
//  I2C_prog
//**************************************************************************

#include <I2C_prog.h>
#include <N76E003.H>

bit FlagI2cStart					=	0;
bit FlagI2cDelay					=	0;

xdata int sizeBufI2c			=	I2C_SIZE_PACKET_BUF;
xdata int sizeADR					=	I2C_SIZE_ADR;
xdata int sizeData				=	I2C_SIZE_DATA;
xdata int sizeByteForSend	=	I2C_AMOUNT_PACKETS;

xdata int counterBitI2c		= 0;
xdata int counterByteI2c	= 0;

//**************************************************************************
/* additional        */
//**************************************************************************

void I2C_change_size_ADR (int value){sizeADR				= value;}		
void I2C_change_size_DATA(int value){sizeData				= value;}		
void I2C_change_size_BUF (int value){sizeBufI2c			= value;}		
void I2C_change_size_BYTE(int value){sizeByteSendI2C= value;}	
void I2C_start(){FlagI2cStart	= 1;}	
void I2C_Write_Buf(long value){
	bufferLowValueTX 	= 	value & 0x00FF;
	bufferHighValueTX = ((value & 0xFF00) >> 8) >> 8;
}

long I2C_Read_Buf(){
	long buf = (bufferHighValueRX << 8) << 8;
	return buf + bufferLowValueRX;
}

//********************************************************
//			_____			 _____				->	CLK
//			|		|			 |	 |
//______|		|______|	 |______
//  f()	 f()	f()		f()	 f()		->	I2C_exchange_start()
//********************************************************
/* main f()for timer/every clk launc. When exchange start*/
void I2C_exchange_start(enum I2cMode mode){
	/*every clk launch*/
	//this put next byte
	I2C_exchange_do(mode/*next byte*/);
}

/*instructions when exchange go */
void I2C_exchange_do(enum I2cMode mode){
	if(counterBitI2c){			
		I2C_SLK =~ I2C_SLK;
	}
	else{	/*counterBit=0*/ 						// what do when start packet
		if(I2C_SLK == I2C_CPOL ){				// check setings
			/*don't change state*/
		}
		else{ 
			I2C_SLK =~ I2C_SLK; 	
		}		
	}

	if( I2C_SLK == I2C_CPHA ){
		switch(counterBitI2c){						//	start exchange	
/*End bit packet	*/
			case I2C_SIZE_PACKET_BUF:	/*	max bit	*/
				/*need next byte?*/
				//if(counterByteI2c > sizeByteSendI2C){
					I2C_exchange_end();
				//}
				//else{
				//	counterByteI2c++;						// next byte
				//}
				break;
/* process going	*/
			default:	/* 0 -> max bit - 1*/
				switch(mode){
					case SEND:  
						I2C_SDA = I2C_Get_Bit_Buf_TX();  // get from buf TX
						break;
					case READ:
						if(I2C_SDA == 1){I2C_Write_Bit_Buf_RX();}  // read pin and write to buf
						break;
					default:break;
				}
/* next bit		*/
				counterBitI2c++; 
/*if will be max size change */
				if(counterBitI2c == sizeBufI2c){ 
					counterBitI2c = I2C_SIZE_PACKET_BUF;
				}
				break;
		}
	}
if(counterBitI2c && I2C_CPHA !=I2C_SLK){
		I2C_SLK =~ I2C_SLK; 
	}
}

/*instructions exchange end */
void I2C_exchange_end(){
	FlagI2cStart	=	0;
	I2C_SLK				=	0;
	I2C_SDA				=	0;
}

/*delay = each iteration when the call */
void I2C_delay(){
	// верм€€ задержки
	// например ширина импульса и следовательно скорость обмена

}

/*init state SLK */
void I2C_init_SLK(bit value){
	I2C_SLK = value;
}

/*read TX buf and send bit*/
static bit I2C_Get_Bit_Buf_TX(){
	int outValue;
	if( counterBitI2c <=15 ){
		outValue = bufferLowValueTX * (0x01 << counterBitI2c);
	}
	else if(counterBitI2c == 16){	// nuvoton << 15 or >>15 (max)!!!
		outValue = bufferLowValueTX * 0x8000;
	}
	else if(counterBitI2c == 32){ // the same as 16 bit for bufferLowValueTX
		outValue = bufferHighValueTX * 0x8000;
	}
	else if( counterBitI2c > 16 /*&& counterBit!=32*/){
		outValue = bufferHighValueTX * (0x01 << counterBitI2c);
	}
	else {} /*error*/
	return outValue ? 1 : 0;
}

/*write in buf Rx ( if only input = 1!)*/
static void I2C_Write_Bit_Buf_RX(){
	if( counterBitI2c <=15 ){
		bufferLowValueRX += (0x01 << counterBitI2c);
	}
	else if(counterBitI2c == 16){	// nuvoton << 15 or >>15 (max)!!!
		bufferLowValueRX += 0x8000;
	}
	else if(counterBitI2c == 32){ // the same as 16 bit for bufferLowValueTX
		bufferHighValueRX += 0x8000;
	}
	else if( counterBitI2c > 16 /*&& counterBit!=32*/){
		bufferHighValueRX += (0x01 << counterBitI2c);
	}
	else {} /*error*/
}


