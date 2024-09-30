//**************************************************************************
//  настройка GPIO сенсора
//**************************************************************************
/*	РАСПИНОВКА:
		N76 -> Стандартный сенсор 
		P5 	- ADCCLK 
		P6 	- TX_BLE / HOLD_F
		P7 	- RX_BLE / WP_F
		P20 - /nReset
		P30 - ADCDOUT / INT0
		P17 - GDO0 / IRQ_RF / INT1
		P16 - DEBUG_D / LED_GREEN
		P15 - CS_RF
		P14 - SEAL0
		P13 - CS_FRAM
		P12 - ~RF_ON
		P11 - GDO2/CE
		P10 - SCLK
		P0 	- SI
		P1 	- SO
		P2 	- DEBUG_C / LED_GREEN
		P3 	- IRQ_BLE
		P4 	- ADC_ON
*/
//**************************************************************************
#ifndef GPIO
#define GPIO
/* main */
#define PIN_LED_RED P02
#define PIN_LED_GREEN P16
#define PIN_BUTTON P14
#define PIN_RF_ON P12
#define PIN_ADC_ON P04

/* <UART_XXX.h> */
#define UART_TX P06
#define UART_RX P07

/* <FRAM_XXX.h> */ // +SPI.h
#define FRAM_HOLD P06
#define FRAM_WP P07

/* <I2C_ADC_HX711.h> */
#define	ADSK P05
#define ADOUT P20						//INT0

/* <SPI.h>  */
#define SPI_MISO P01
#define SPI_MOSI P00
#define SPI_SCK P10
#define SPI_CSN P15
#define SPI_CSFRAM P13

/* <NRF24.h>  */
#define NRF_CE P11
#define NRF_IRQ P17					//INT1

void Led_Switch_State();
void Button_Read();
int Switch_State_Power_RF();
void GPIO_init();
#endif

