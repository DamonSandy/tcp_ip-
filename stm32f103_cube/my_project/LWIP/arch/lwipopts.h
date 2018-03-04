#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS         1       //�޲���ϵͳģ��㣬�����ǳ���Ҫ

#define LWIP_SOCKET  0         //������socket API������ļ�
#define LWIP_NETCONN  0        //������sequential API������ļ�

#define MEM_ALIGNMENT   4      //�ڴ��ֽڶ��뷽ʽ���������ƽ̨�������

#define MEM_SIZE  1024*30      //�����ڴ�ѵĴ�С������ʹ��opt.h�е�Ĭ��ֵ

#define TCP_SND_BUF  4000      //����tcpЭ��ʹ�õ�����ͻ��峤��
#define TCP_MSS    1000        //tcp���Ķ���󳤶�

#define ETHARP_DEBUG     LWIP_DBG_ON    //��������ӡARP�����Ϣ
#define ICMP_DEBUG     LWIP_DBG_ON      //��������ӡICMP�����Ϣ

#endif



