//**************************************************************************
//  I2C_prog
//**************************************************************************

#include <I2C_prog.h>
#include <N76E003.H>

bit FlagI2cStart	=	0;
bit FlagI2cDelay	=	0;

xdata int I2cSizeBuf				=	I2C_SIZE_PACKET_BUF;
xdata int sizeADR						=	I2C_SIZE_ADR;
xdata int sizeData					=	I2C_SIZE_DATA;
int sizeByteForSend	=	I2C_AMOUNT_PACKETS;

void I2C_change_size_ADR(int value){sizeADR	= value;}
void I2C_change_size_DATA(int value){sizeData= value;}
void I2C_change_size_BUF(int value){sizeData= value;}

//********************************************************
//			_____			 _____				->	CLK
//			|		|			 |	 |
//______|		|______|	 |______
//  f()	 f()	f()		f()	 f()		->	I2C_exchange_start()
//********************************************************
/* main f()for timer/every clk launc. When exchange start*/
void I2C_exchange_start(){
	/*every clk launch*/
	
}

/*instructions when exchange go */
void I2C_exchange_do(enum I2cMode mode){
	// здесь мне надо задать cигнал clk 
	// пройти мимо
	// когда вернусь прочитать
	// записать новое значение и пройти мимо ( вернутс€ в начало)
	
	if(counterBit){			
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
		switch(counterBit){							//	start exchange	
/*End bit packet	*/
			case I2C_SIZE_PACKET_BUF:	/*	max bit	*/
				/*need next byte?*/
				if(counterByte > sizeByteForSend){
					FlagI2cStart	=	0;
					I2C_SLK				=	0;
					I2C_SDA				=	0;
				}
				else{
					counterByte++;						// next byte
				}
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
				counterBit++; 
/*if will be max size change */
				if(counterBit == I2cSizeBuf){ 
					counterBit = I2C_SIZE_PACKET_BUF;
				}
				break;
		}
		
	}

}

/*instructions exchange end */
void I2C_exchange_end(){
	//  все линии в 0 или в 1
	// мб добавить сюда настройки

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
	if( counterBit <=15 ){
		outValue = bufferLowValueTX * (0x01 << counterBit);
	}
	else if(counterBit == 16){	// nuvoton << 15 or >>15 (max)!!!
		outValue = bufferLowValueTX * 0x8000;
	}
	else if(counterBit == 32){  // the same as 16 bit for bufferLowValueTX
		outValue = bufferHighValueTX * 0x8000;
	}
	else if( counterBit > 16 /*&& counterBit!=32*/){
		outValue = bufferHighValueTX * (0x01 << counterBit);
	}
	else {} /*error*/
	return outValue ? 1 : 0;
}

/*write in buf Rx ( if only input = 1!)*/
static void I2C_Write_Bit_Buf_RX(){
	if( counterBit <=15 ){
		bufferLowValueRX += (0x01 << counterBit);
	}
	else if(counterBit == 16){	// nuvoton << 15 or >>15 (max)!!!
		bufferLowValueRX += 0x8000;
	}
	else if(counterBit == 32){  // the same as 16 bit for bufferLowValueTX
		bufferHighValueRX += 0x8000;
	}
	else if( counterBit > 16 /*&& counterBit!=32*/){
		bufferHighValueRX += (0x01 << counterBit);
	}
	else {} /*error*/
}


