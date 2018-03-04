#ifndef _DM9000AEP_H
#define _DM9000AEP_H
#include "fsmc.h"
#include "gpio.h"
//#include "lwip_demo.h"
//#include "sys.h"
#include "lwip/pbuf.h"

//DM9000��λ����
#define DM9000_RST(a)		if(a){HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);}\
												else{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);}
													 
#define DM9000_INT		HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6)			//DM9000�ж�����

//DM9000��ַ�ṹ��
typedef struct
{
	__IO uint16_t REG;
	__IO uint16_t DATA;
}DM9000_TypeDef;

//ʹ��NOR/SRAM�� Bank1.sector2,��ַλHADDR[27,26]=01 A7��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 			    
#define DM9000_BASE        ((u32)(0x64000000|0x000000FE))
#define DM9000             ((DM9000_TypeDef *) DM9000_BASE)

#define DM9000_ID			0X90000A46	//DM9000 ID
#define DM9000_PKT_MAX		1536		//DM9000�����հ�����

#define DM9000_PHY			0X40		//DM9000 PHY�Ĵ������ʱ�־
//DM9000�Ĵ���
#define DM9000_NCR			0X00
#define DM9000_NSR			0X01
#define DM9000_TCR			0X02
#define DM9000_TSRI			0X03
#define DM9000_TSRII		0X04
#define DM9000_RCR			0X05
#define DM9000_RSR			0X06
#define DM9000_ROCR			0X07	
#define DM9000_BPTR			0X08
#define DM9000_FCTR			0X09
#define DM9000_FCR			0X0A
#define DM9000_EPCR			0X0B
#define DM9000_EPAR			0X0C
#define DM9000_EPDRL		0X0D	
#define DM9000_EPDRH		0X0E
#define DM9000_WCR			0X0F
#define DM9000_PAR			0X10		//������ַ0X10~0X15
#define DM9000_MAR			0X16		//�ಥ��ַ0X16~0X1D
#define DM9000_GPCR			0X1E
#define DM9000_GPR			0X1F
#define DM9000_TRPAL		0X22
#define DM9000_TRPAH		0X23
#define DM9000_RWPAL		0X24
#define DM9000_RWPAH		0X25

#define DM9000_VIDL			0X28
#define DM9000_VIDH			0X29
#define DM9000_PIDL			0X2A
#define DM9000_PIDH			0X2B

#define DM9000_CHIPR		0X2C
#define DM9000_TCR2			0X2D
#define DM9000_OCR			0X2E
#define DM9000_SMCR			0X2F
#define DM9000_ETXCSR		0X30
#define DM9000_TCSCR		0X31
#define DM9000_RCSCSR		0X32
#define DM9000_MRCMDX		0XF0
#define DM9000_MRCMDX1		0XF1
#define DM9000_MRCMD		0XF2
#define DM9000_MRRL			0XF4
#define DM9000_MRRH			0XF5
#define DM9000_MWCMDX		0XF6
#define DM9000_MWCMD		0XF8
#define DM9000_MWRL			0XFA
#define DM9000_MWRH			0XFB
#define DM9000_TXPLL		0XFC
#define DM9000_TXPLH		0XFD
#define DM9000_ISR			0XFE
#define DM9000_IMR			0XFF

#define NCR_RST             0X01
#define NSR_SPEED           0X80
#define NSR_LINKST         	0X40
#define NSR_WAKEST          0X20
#define NSR_TX2END          0X08
#define NSR_TX1END          0X04
#define NSR_RXOV            0X02

#define RCR_DIS_LONG        0X20
#define RCR_DIS_CRC         0X10
#define RCR_ALL             0X08
#define RCR_RXEN            0X01

#define IMR_PAR             0X80	
#define IMR_ROOI            0X08	
#define IMR_POI             0X04		//ʹ�ܽ�������ж�
#define IMR_PTI             0X02		//ʹ�ܷ����ж�
#define IMR_PRI             0X01		//ʹ�ܽ����ж�

#define ISR_LNKCHGS         (1<<5)
#define ISR_ROOS            (1<<3)
#define ISR_ROS             (1<<2)
#define ISR_PTS             (1<<1)
#define ISR_PRS             (1<<0)
#define ISR_CLR_STATUS      (ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS)

//DM9000�ڲ�PHY�Ĵ���
#define DM9000_PHY_BMCR		0X00
#define DM9000_PHY_BMSR		0X01
#define DM9000_PHY_PHYID1	0X02
#define DM9000_PHY_PHYID2	0X03
#define DM9000_PHY_ANAR		0X04
#define DM9000_PHY_ANLPAR	0X05
#define DM9000_PHY_ANER		0X06
#define DM9000_PHY_DSCR		0X10
#define DM9000_PHY_DSCSR	0X11
#define DM9000_PHY_10BTCSR	0X12
#define DM9000_PHY_PWDOR	0X13
#define DM9000_PHY_SCR		0X14

//DM9000����ģʽ����
enum DM9000_PHY_mode
{
	DM9000_10MHD 	= 	0, 					//10M��˫��
	DM9000_100MHD 	= 	1,					//100M��˫��	
	DM9000_10MFD 	= 	4, 					//10Mȫ˫��
	DM9000_100MFD 	= 	5,					//100Mȫ˫��
	DM9000_AUTO  	= 	8, 					//�Զ�Э��
};

//DM9000���ýṹ��
struct dm9000_config
{
	enum DM9000_PHY_mode mode;				//����ģʽ
	uint8_t  imr_all;							//�ж����� 
	uint16_t queue_packet_len;					//ÿ�����ݰ���С
	uint8_t  mac_addr[6];						//MAC��ַ
	uint8_t  multicase_addr[8];					//�鲥��ַ
};
extern struct dm9000_config dm9000cfg;		//dm9000���ýṹ��


uint8_t   DM9000_Init(void);
uint16_t  DM9000_ReadReg(uint16_t reg);
void DM9000_WriteReg(uint16_t reg,uint16_t data);
uint16_t  DM9000_PHY_ReadReg(uint16_t reg);
void DM9000_PHY_WriteReg(uint16_t reg,uint16_t data);
uint32_t  DM9000_Get_DeiviceID(void);
uint8_t   DM9000_Get_SpeedAndDuplex(void);	
void DM9000_Set_PHYMode(uint8_t mode);
void DM9000_Set_MACAddress(uint8_t *macaddr);
void DM9000_Set_Multicast(uint8_t *multicastaddr);
void DM9000_Reset(void);
void DM9000_SendPacket(struct pbuf *p);
struct pbuf *DM9000_Receive_Packet(void);
void DMA9000_ISRHandler(void);
#endif