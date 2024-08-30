/******************************/
//
/******************************/

/*registers definition section*/
#define CONFIG_ADDRESS              0X00
#define EN_AA_ADDRESS               0X01              /*enable auto acknowledgement feature*/
#define EN_RXADDR_ADDRESS           0X02              /*register containing bits to enable 6 data pipes individually*/
#define SETUP_AW_ADDRESS            0X03              /*address field length is configured in here to be 3, 4 or 5 bytes long*/
#define SETUP_RETR_ADDRESS          0X04              /*setup ARC bits to configure auto retransmission count*/
#define RF_CH_ADDRESS               0X05
#define RF_SETUP_ADDRESS            0X06
#define STATUS_ADDRESS              0X07              /*contains RX_DR, TX_DS, MAX_RT, RX_P_NO, TX_FULL, send R_REGISTER then NOP to read*/ 
#define OBSERVE_TX_ADDRESS          0X08              /*contains ARC_CNT and PLOS_CNT, two counters for retransmission. these counters could be used to assess the network quality*/
#define RPD_REG_ADDRESS             0X09
#define RX_ADDR_P0_ADDRESS          0X0A              /*the address for PRX device. if a packet contains this address, enhanced shockburst starts validating the packet*/
#define RX_ADDR_P1_ADDRESS          0X0B              /*a total of 6 unique addresses could be assigned to a PRX device (Multiceiver feature)*/
#define RX_ADDR_P2_ADDRESS          0X0C              /*these addresses must NOT be the same*/
#define RX_ADDR_P3_ADDRESS          0X0D
#define RX_ADDR_P4_ADDRESS          0X0E
#define RX_ADDR_P5_ADDRESS          0X0F
#define TX_ADDR_ADDRESS             0X10              /*40 bits long register, transmit address, used for a PTX device only. configure address legth in SETUP_AW register. set RX_ADDR_P0 equal to this address to handle automatic acknowledge*/
#define RX_PW_P0_ADDRESS            0X11              /*these registers are for setting the static payload length in static payload length mode (receiver side)*/
#define RX_PW_P1_ADDRESS            0X12
#define RX_PW_P2_ADDRESS            0X13
#define RX_PW_P3_ADDRESS            0X14
#define RX_PW_P4_ADDRESS            0X15
#define RX_PW_P5_ADDRESS            0X16
#define FIFO_STATUS_ADDRESS         0X17
#define DYNPD_ADDRESS               0X1C              /*on receiver side (RX mode), this register must be set to enable dynamic payload length. a PTX in dynamic mode, must have the DYNPD_P0 set*/
#define FEATURE_ADDRESS             0X1D              /*contains the EN_DPL bit to enable dynamic payload length*/

/*commands definition section*/
#define R_REGISTER          0X00              /*read commmand and STATUS registers, 5 bit register map address*/
#define W_REGISTER          0X20              /*write commmand and STATUS registers, 5 bit register map address, executable in POWER DOWN or STANDBY modes only*/
#define R_RX_PAYLOAD        0X61              /*read RX payload, 1-32 bytes. read operation starts at byte 0. payload is deleted from FIFO after its read*/
#define W_TX_PAYLOAD        0XA0              /*write TX payload, starts at byte 0, 1-32 bytes*/
#define FLUSH_TX            0XE1              /*flush TX FIFO, used in TX mode*/
#define FLUSH_RX            0XE2              /*flush RX FIFO, used in RX mode*/
#define REUSE_TX_PL         0XE3              /*used for a PTX device, reuse last transmitted payload for an exact number. alternative to auto retransmission*/
#define R_RX_PL_WID         0X60              /*command for receiver side, in order to read the payload length in dynamic payload length mode*/
#define W_ACK_PAYLOAD       0XA0              /*used in RX mode, to write payload in TX FIFO and later transmit the payloads along with ACK packet to PTX, if DPL is enabled*/
#define W_TX_PAYLOAD_NOACK  0XB0              /*used in TX mode, disables AUTOACK on this specific packet. must be first enabled in FEATURE register by setting the EN_DYN_ACK bit. if used, PTX will not wait for ACK and goes directly to standby I*/
#define NOP_CMD             0XFF              /*might be used to read the status register*/