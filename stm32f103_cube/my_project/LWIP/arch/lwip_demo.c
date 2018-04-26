#include "lwip_demo.h"
#include "ethernetif.h"
#include "tim.h"
#include "lwip/udp.h"
#include "string.h"
#include "golbal_main.h"
/*
	this is test how to use github
*/
void Test_Use_Github(void)
{
	printf("this is test how to use github\r\n");
}

struct netif dm9000_netif;
unsigned int system_tick_num = 0;
/*UDP实验相关变量*/
#define AS_SERVICE_PORT  5000
#define REMOTE_PORT  8080
#define STR_BUFFER  MAX_REC_NUM
netif_device_one my_netif_device;
char str[MAX_REC_NUM] = {0};
/*TCP实验相关变量*/
#define HTTP_PORT 80
//服务器提供的固定网页html数据
const unsigned char htmldata[] = "\
																	<html>\
																	<head><title>Damon And Sandy WebServer!!</title></head>\
																	<center><p>Damon love Sandy very much !</center>\
																	</html>";
/*****************************************************/
u32_t sys_now(void)
{
	return system_tick_num;
}
//读取网卡内所有剩余的数据包并递交给内核处理
void process_mac(void)
{
	s32_t ret = 0;
	do{
		ret = ethernetif_input(&dm9000_netif);
	}while(ret);
}
//网络接口注册
void lwip_init_task(
	netif_device_one *my_netif
)
{
	lwip_init();
	IP4_ADDR(&(my_netif->gw),192,168,1,1);
	IP4_ADDR(&(my_netif->ipaddr),192,168,1,37);
	IP4_ADDR(&(my_netif->netmask),255,255,255,0);
	netif_add(&dm9000_netif,&(my_netif->ipaddr),&(my_netif->netmask),&(my_netif->gw),NULL,&ethernetif_init,&ethernet_input);
	netif_set_default(&dm9000_netif);
	netif_set_up(&dm9000_netif);
}
/*************************************UDP相关函数**********************************************/
/*新建UDP控制块*/
//struct udp_pcb *ubp_new(void)
/*绑定控制块*/
//err_t udp_bind(struct udp_pcb *pcb,ip_addr_t *ipaddr, u16_t port)
/*连接控制块*/
//err_t udp_connect(struct udp_pcb *pcb,ip_addr_t *ipaddr, u16_t port)
/*清除链连接*/
//err_t udp_disconnect(struct udp_pcb *pcb)
/*注册回调函数*/
//void udp_recv(struct udp_pcb *pcb,udp_recv_fn recv,void *recv_arg)
/*UDP数据发送函数*/
//err_t udp_send(struct udp_pcb *pcb,struct pbuf *p)
//err_t udp_sendto(struct udp_pcb *pcb,struct pbuf *p,struct ip_addr *dst_ip,uint16_t dst_port)
/*移除ubp控制块*/
//void udp_remove(struct udp_pcb *pcb)
/*用户自定义的回调函数*/
void my_udp_callback(
	void *arg,
	struct udp_pcb *upcb,
	struct pbuf *p,
	struct ip_addr *addr,
	uint16_t port
)
{
		memcpy(str,p->payload,p->len);
		udp_send(upcb,p);
		pbuf_free(p); //释放接收到的数据
		printf("%s\r\n",str);
		memset(str,0,sizeof(str));
}
/*udp初始化函数*/
void my_udp_init(
		netif_device_one *my_netif
)
{
	struct ip_addr ipaddr;
	my_netif->upcb = udp_new();
	//绑定本地端口
	udp_bind(my_netif->upcb,&(my_netif->ipaddr),AS_SERVICE_PORT);
	//和远端客户建立连接
	IP4_ADDR(&ipaddr,192,168,1,105);
	udp_connect(my_netif->upcb,&ipaddr,REMOTE_PORT);
	//注册用户回调函数
	udp_recv(my_netif->upcb,my_udp_callback,NULL);
}
//通过网口发送数据(要先网口发下数据过来才能正常通信，需要寻找原因)
void send_my_data_by_internet(
		netif_device_one *my_netif
)
{
	struct pbuf *q;
	q = pbuf_alloc(PBUF_TRANSPORT,strlen(str)+20,PBUF_RAM);
	if(q == NULL){
		printf("out of PBUF_RAM\n");
		return;
	}
	//构造pbuf数据
	memset(q->payload,0,q->len);
	memcpy(q->payload,str,strlen(str));
	udp_send(my_netif->upcb,q);
	printf("%s",str);
	memset(str,0,strlen(str));
	pbuf_free(q); //释放缓存
}
/***********************************tcp相关函数****************************************************/
//作为服务器
//当服务器收到客户端的请求数据时，该函数将被调用，p中包含了请求数据
err_t http_recv(
	void *arg,
	struct tcp_pcb *pcb,
	struct pbuf *p,
	err_t err
)
{
	char *data = NULL;
	if(p != NULL){
		tcp_recved(pcb,p->tot_len);    //更新接收窗口
		data = p->payload;
		if(p->len >= 3 && data[0] == 'G' && data[1] == 'E' && data[2] == 'T'){
			tcp_write(pcb,htmldata,sizeof(htmldata),TCP_WRITE_FLAG_COPY);
		}
		else{
			printf("Request error \n");
		}
		pbuf_free(p);
		tcp_close(pcb);
	}
	//如果 p == NULL 表示客户端已关闭连接，服务器也需要关闭连接
	else if(err == ERR_OK){
		return tcp_close(pcb);
	}
	return ERR_OK;
}
//当有客户建立连接后，该函数被调用，pcb为与新连接对应的新控制块
err_t http_accept(
	void *arg,
	struct tcp_pcb *pcb,
	err_t err
)
{
	tcp_recv(pcb,http_recv);  //注册recv函数
	return ERR_OK;
}
//http服务器初始化函数
void http_server_init(void)
{
	struct ip_addr ipaddr;
	struct tcp_pcb *pcb = NULL;
	pcb = tcp_new();
	IP4_ADDR(&ipaddr,192,168,1,37);
	tcp_bind(pcb,&ipaddr,HTTP_PORT);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb,http_accept);
}
/********************************************************/
//作为客户机
//当连接失败，调用这个函数重新建立客户端，重新开始连接
void echo_client_conn_err(
		void *arg,
		err_t err
)
{
	printf("重新连接！！！\r\n");
	echo_client_init();
}
//当接收到服务器的数据后，这个函数被内核调用，p中包含了服务器数据
err_t echo_client_recv(
	void *arg,
	struct tcp_pcb *pcb,
	struct pbuf *p,
	err_t err
)
{
	if(p != NULL){
		tcp_recved(pcb,p->tot_len);  //更新接收窗口
		tcp_write(pcb,p->payload,p->len,TCP_WRITE_FLAG_COPY);  //回显服务器的数据
		pbuf_free(p);
	}
	else if(err == ERR_OK){
		tcp_close(pcb);
		echo_client_init();
		printf("get serverice error!\n");
		return ERR_OK;
	}
}
//当客户端向服务器发送数据成功后，这个函数被内核调用
err_t echo_client_sent(
	void *arg,
	struct tcp_pcb *pcb,
	uint16_t len
)
{
	return ERR_OK;
}
//当客户端同服务器连接建立成功后，这个函数被内核调用
err_t echo_client_connected(
	void *arg,
	struct tcp_pcb *pcb,
	err_t err
)
{
	char GREETING[] = "HI, I am a new client!\r\n";
	tcp_recv(pcb,echo_client_recv);   //注册recv函数
	tcp_sent(pcb,echo_client_sent);   //注册sent函数
	tcp_write(pcb,GREETING,sizeof(GREETING),TCP_WRITE_FLAG_COPY);
	return ERR_OK;
}
//客户端初始化函数
void echo_client_init(void)
{
	err_t er;
	struct ip_addr ipaddr;
	struct tcp_pcb *pcb = NULL;
	pcb = tcp_new();
	if(pcb == NULL){
		printf("pcb new is failed...\n");
	}
	IP4_ADDR(&ipaddr,192,168,1,105);
	
	er = tcp_connect(pcb,&ipaddr,5050,echo_client_connected);
	printf("%d\r\n",er);
	tcp_err(pcb,echo_client_conn_err);
}
/***************************************************************************************************/
void lwip_demo(
	void *pdata
)
{
	lwip_init_task(&my_netif_device);
//	my_udp_init(&my_netif_device);
//	http_server_init();
	echo_client_init();
	__HAL_TIM_ENABLE(&htim7);
	while(1){
		sys_check_timeouts();
	}
}
