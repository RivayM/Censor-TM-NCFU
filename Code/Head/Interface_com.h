//********************************************************
//  Interface common
//  ���������� I2c ��� sck + MOSI , ������������� ��������
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
#define FREQ_H FREQ_120KHZ_H         // ��������� �������
#define FREQ_L FREQ_120KHZ_L

void InCom_SPI_init_Timer(void);
/* void ISR_Timer0() interrupt 1 */  // use
//********************************************************
//  ��� NRF24L01
//********************************************************
#define BUFFER_SPI 8   // ���������� ��� � ������
#define BUFFER_SPI_MASSIV_SIZE NRF24_BUFFER_MASSIV_SIZE // ���������� �������( � 1 ��������/������)
#define SPI_MSB 1      // or use SPI_LSB = 0
extern unsigned char valueBufferTx;  // ��������� ����� ������ �����
extern unsigned char valueBufferRx;  // ����� ����� ��������
extern unsigned char valueBufferArrayTx[BUFFER_SPI_MASSIV_SIZE];  // ��������� ����� ������ �����
extern unsigned char valueBufferArrayRx[BUFFER_SPI_MASSIV_SIZE];  // ����� ����� ��������(������ +1)

extern int counterBufferSpi;       // ���������� ��� / ������������ � interrupt 1
extern int counterBufferSpiPacket; // ���������� �������
extern bit FlagInComSPIexchangeContinue;  // ���������� �� �����
extern bit FlagInComSPIGlobal;  	        // ���������� ( ������)
extern bit FlagInComSPIexchangeByte;  	  // ���������� ������ byte  �� ������
extern bit FlagInComSPIexchangeArray;  	  // ���������� ������ ������ �� ������
 
/* USE GPIO.h FOR SPI*/
#define PIN_CLK_SPI    P10
#define PIN_MOSI_SPI   P00
#define PIN_MISO_SPI   P01
#define PIN_CS_SPI     P15

/*���������������*/
bit InCom_SPI_Output_in_buffer(unsigned char *outsidebuffer);
unsigned char InCom_SPI_Value_received(void);  // ���������� ��������
void InCom_SPI(bit valueMosi, unsigned char *outSideBuffer );
void InCom_SPI_CLK_init(bit init);

void InCom_SPI_array_received_clear();         // ��� ��� �������
void InCom_SPI_array_transitive_clear();
void InCom_SPI_byte_transitive_clear();
void InCom_SPI_byte_received_clear();
void InCom_Array_clear(unsigned char *array);
/* �������� ��������*/
void InCom_SPI_byte_transitive(unsigned char *byte);  // ����� �������� ������
unsigned char *InCom_SPI_byte_received();

void InCom_SPI_array_transitive(unsigned char *array);  // ����� �������� ������
unsigned char *InCom_SPI_array_received();

/*�����*/
void InCom_SPI_Input_in_buffer(unsigned char *outsidebuffer);
void InCom_SPI_exchange(void);    
void InCom_SPI_send_byte();
void InCom_SPI_send_array();
//********************************************************


#endif