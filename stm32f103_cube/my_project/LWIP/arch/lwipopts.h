#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS         1       //无操作系统模拟层，这个宏非常重要

#define LWIP_SOCKET  0         //不编译socket API的相关文件
#define LWIP_NETCONN  0        //不编译sequential API的相关文件

#define MEM_ALIGNMENT   4      //内存字节对齐方式，这个宏与平台密切相关

#define MEM_SIZE  1024*30      //定义内存堆的大小，否则使用opt.h中的默认值

#define TCP_SND_BUF  4000      //允许tcp协议使用的最大发送缓冲长度
#define TCP_MSS    1000        //tcp报文段最大长度

#define ETHARP_DEBUG     LWIP_DBG_ON    //允许程序打印ARP相关信息
#define ICMP_DEBUG     LWIP_DBG_ON      //允许程序打印ICMP相关信息

#endif



