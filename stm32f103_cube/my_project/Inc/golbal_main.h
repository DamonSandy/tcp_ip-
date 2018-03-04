#ifndef _GOLBAL_MAIN_
#define _GOLBAL_MAIN_
#include "stm32f1xx_hal.h"
/*******************************************************************/
//串口数据相关变量及函数
/*******************************************************************/
#define ARR_LEN(x)   (sizeof(x)/sizeof(x[0]))    //计算数组长度
#define MAX_REC_NUM   100
#define OVERTIME_NUM   4
#define TRUE     1
#define FALSE    0
typedef struct usart_data   //串口数据的处理
{
	uint8_t receive_buf[MAX_REC_NUM];
	uint8_t rec_bf[1];      //作为接收中断数据缓存用，HAL库使用机制原因用到
	uint8_t data_count;
	uint8_t rec_timecount;
}t_USART_DATA;
//关于t_USART_DATA类型的全局变量的访问器子程序
void add_recbuf_value(t_USART_DATA *USART_deliver,uint8_t value);
uint8_t Is_rectimeover(t_USART_DATA *USART_deliver);
void clr_recbuf(t_USART_DATA *USART_deliver);
uint8_t *get_recbuf_adr(t_USART_DATA *USART_deliver);
uint8_t *get_recbf_adr(t_USART_DATA *USART_deliver);
/******************************************************/
//用户使用定义的的全局变量
/******************************************************/
//串口数据处理全局变量
extern t_USART_DATA G_USART_deliver;

#endif