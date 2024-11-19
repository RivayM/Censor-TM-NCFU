//**************************************************************************
//  I2C_prog
//**************************************************************************

#include <I2C_prog.h>

bit FlagI2cStart	=	0;
bit FlagI2cDelay	=	0;

xdata int I2cSizebuf	=	I2C_SIZE_PACKET_BUF;
xdata int sizeADR			=	I2C_SIZE_ADR;
xdata int sizeData		=	I2C_SIZE_DATA;

void I2C_change_size_ADR	(int value){sizeADR	= value;}
void I2C_change_size_DATA	(int value){sizeData= value;}

//********************************************************
//			_____			 _____				->	CLK
//			|		|			 |	 |
//______|		|______|	 |______
//  f()	 f()	f()		f()	 f()		->	I2C_exchange_start()
//********************************************************
/* main func(for timer) */
void I2C_exchange_start(){
	/*every clk launch*/
	// то что использовать в таймере и чо выставл€ть в момент времени
	
	
}


void I2C_exchange_do(I2cMode mode){
	// здесь мне надо задать cигнал clk 
	// пройти мимо
	// когда вернусь прочитать
	// записать новое значение и пройти мимо ( вернутс€ в начало)
	
	if( counterBit || I2C_CPOL){
		
	}
	
	if( I2C_SLK == I2C_CPHA ){
		switch(counterBit){							//	start exchange	
/*End bit packet	*/
			case I2cSizeBuf:	/*	max bit	*/
				/*need next byte?*/
				if(counterByte > sizeByteForDend){+
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
						//складывать в буфер I2C_SDA
						break;
					case READ:
					
					if(I2C_SDA == 1){
						//прочитать
						}
						break;
					default:break;
				}
			
				I2C_SDA = output;
/* next bit		*/
				counterBit++; 		
				break;
		}
		
	}

}

void I2C_exchange_end(){
	//  все линии в 0 или в 1
	// мб добавить сюда настройки

}

void I2C_delay(){
	// верм€€ задержки
	// например ширина импульса и следовательно скорость обмена

}

void I2C_init_CLK(bit value){
	I2C_SLK = value;
}

static bit I2C_Get_Buf_TX(){
	return 1;
}

static bit I2C_Get_Buf_RX(){
	return 1;
}


