#ifndef _LWIP_DEMO_
#define _LWIP_DEMO_

#include "stm32f1xx_hal.h"
#include "netif.h"
#include "ip.h"
#include "tcp.h"
#include "init.h"
#include "etharp.h"
#include "timers.h"

typedef struct _netif_device_one
{
	struct udp_pcb *upcb;   //udp控制块
	struct ip_addr ipaddr,netmask,gw;  //网卡参数
}netif_device_one;

u32_t sys_now(void);
void process_mac(void);
/*UDP*/
void lwip_init_task(netif_device_one *my_netif);
void my_udp_init(netif_device_one *my_netif);
void send_my_data_by_internet(netif_device_one *my_netif);
/*TCP*/
//做服务器
err_t http_recv(void *arg,struct tcp_pcb *pcb,struct pbuf *p,err_t err);
err_t http_accept(void *arg,struct tcp_pcb *pcb,err_t err);
void http_server_init(void);
//做客户机
void echo_client_conn_err(void *arg,err_t err);
err_t echo_client_recv(void *arg,struct tcp_pcb *pcb,struct pbuf *p,err_t err);
err_t echo_client_sent(void *arg,struct tcp_pcb *pcb,uint16_t len);
err_t echo_client_connected(void *arg,struct tcp_pcb *pcb,err_t err);
void echo_client_init(void);


void lwip_demo(void *pdata);
#endif