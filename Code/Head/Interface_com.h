//********************************************************
//  Interface common
//  пародирует I2c или sck + MOSI , настраивается таймером
//********************************************************
#ifndef Interface_common
#define Interface_common
#include "CONFIG.h"
//********************************************************
#define FREQ_120KHZ_H 0xFF
#define FREQ_120KHZ_L 0xFB
#define FREQ_480KHZ_H 0xFF
#define FREQ_480KHZ_L 0xFD
//********************************************************
/*USE FREQ IN void init_CLK*/
#define FREQ_H FREQ_120KHZ_H         // настройка частоты
#define FREQ_L FREQ_120KHZ_L

void InCom_SPI_init_Timer(void);
/* void ISR_Timer0() interrupt 1 */  // use
//********************************************************
//  для NRF24L01
//********************************************************
#define BUFFER_SPI 8   // количество бит в пакете
#define BUFFER_SPI_MASSIV_SIZE NRF24_BUFFER_MASSIV_SIZE // количество пакетов( в 1 передаче/приеме)
#define SPI_MSB 1      // or use SPI_LSB = 0
extern unsigned char valueBufferTx;  // прочитать можно только тогда
extern unsigned char valueBufferRx;  // когда обмен закончен
extern unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  // прочитать можно только тогда
extern unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  // когда обмен закончен(размер +1)

extern int counterBufferSpi;       // количество бит / использовать в interrupt 1
extern int counterBufferSpiPacket; // количетсво пакетов
extern bit FlagInComSPIexchangeContinue;  // продолжить ли обмен
extern bit FlagInComSPIGlobal;  	        // Разрешение ( обмена)
extern bit FlagInComSPIexchangeByte;  	  // Разрешение отдать byte  из буфера
extern bit FlagInComSPIexchangeArray;  	  // Разрешение отдать массив из буфера
 
/* USE GPIO.h FOR SPI*/
#define PIN_CLK_SPI    P10
#define PIN_MOSI_SPI   P00
#define PIN_MISO_SPI   P01
#define PIN_CS_SPI     P15

/*Вспомогательное*/
bit InCom_SPI_Output_in_buffer(unsigned char *outsidebuffer);
unsigned char InCom_SPI_Value_received(void);  // полученное значение
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer );
void InCom_SPI_CLK_init(bit init);

void InCom_SPI_array_received_clear();         // Все для очистки
void InCom_SPI_array_transitive_clear();
void InCom_SPI_byte_transitive_clear();
void InCom_SPI_byte_received_clear();
void InCom_Array_clear(unsigned char *array);
/* Получить значения*/
void InCom_SPI_byte_transitive(unsigned char *byte);  // какое значение отдать
unsigned char *InCom_SPI_byte_received();

void InCom_SPI_array_transitive(unsigned char *array);  // какое значение отдать
unsigned char *InCom_SPI_array_received();

/*Обмен*/
void InCom_SPI_Input_in_buffer(unsigned char *outsidebuffer);
void InCom_SPI_exchange(void);    
void InCom_SPI_send_byte();
void InCom_SPI_send_array();
//********************************************************


#endif