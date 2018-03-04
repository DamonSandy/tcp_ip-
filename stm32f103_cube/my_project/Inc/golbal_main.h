#ifndef _GOLBAL_MAIN_
#define _GOLBAL_MAIN_
#include "stm32f1xx_hal.h"
/*******************************************************************/
//����������ر���������
/*******************************************************************/
#define ARR_LEN(x)   (sizeof(x)/sizeof(x[0]))    //�������鳤��
#define MAX_REC_NUM   100
#define OVERTIME_NUM   4
#define TRUE     1
#define FALSE    0
typedef struct usart_data   //�������ݵĴ���
{
	uint8_t receive_buf[MAX_REC_NUM];
	uint8_t rec_bf[1];      //��Ϊ�����ж����ݻ����ã�HAL��ʹ�û���ԭ���õ�
	uint8_t data_count;
	uint8_t rec_timecount;
}t_USART_DATA;
//����t_USART_DATA���͵�ȫ�ֱ����ķ������ӳ���
void add_recbuf_value(t_USART_DATA *USART_deliver,uint8_t value);
uint8_t Is_rectimeover(t_USART_DATA *USART_deliver);
void clr_recbuf(t_USART_DATA *USART_deliver);
uint8_t *get_recbuf_adr(t_USART_DATA *USART_deliver);
uint8_t *get_recbf_adr(t_USART_DATA *USART_deliver);
/******************************************************/
//�û�ʹ�ö���ĵ�ȫ�ֱ���
/******************************************************/
//�������ݴ���ȫ�ֱ���
extern t_USART_DATA G_USART_deliver;

#endif