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
/*UDPʵ����ر���*/
#define AS_SERVICE_PORT  5000
#define REMOTE_PORT  8080
#define STR_BUFFER  MAX_REC_NUM
netif_device_one my_netif_device;
char str[MAX_REC_NUM] = {0};
/*TCPʵ����ر���*/
#define HTTP_PORT 80
//�������ṩ�Ĺ̶���ҳhtml����
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
//��ȡ����������ʣ������ݰ����ݽ����ں˴���
void process_mac(void)
{
	s32_t ret = 0;
	do{
		ret = ethernetif_input(&dm9000_netif);
	}while(ret);
}
//����ӿ�ע��
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
/*************************************UDP��غ���**********************************************/
/*�½�UDP���ƿ�*/
//struct udp_pcb *ubp_new(void)
/*�󶨿��ƿ�*/
//err_t udp_bind(struct udp_pcb *pcb,ip_addr_t *ipaddr, u16_t port)
/*���ӿ��ƿ�*/
//err_t udp_connect(struct udp_pcb *pcb,ip_addr_t *ipaddr, u16_t port)
/*���������*/
//err_t udp_disconnect(struct udp_pcb *pcb)
/*ע��ص�����*/
//void udp_recv(struct udp_pcb *pcb,udp_recv_fn recv,void *recv_arg)
/*UDP���ݷ��ͺ���*/
//err_t udp_send(struct udp_pcb *pcb,struct pbuf *p)
//err_t udp_sendto(struct udp_pcb *pcb,struct pbuf *p,struct ip_addr *dst_ip,uint16_t dst_port)
/*�Ƴ�ubp���ƿ�*/
//void udp_remove(struct udp_pcb *pcb)
/*�û��Զ���Ļص�����*/
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
		pbuf_free(p); //�ͷŽ��յ�������
		printf("%s\r\n",str);
		memset(str,0,sizeof(str));
}
/*udp��ʼ������*/
void my_udp_init(
		netif_device_one *my_netif
)
{
	struct ip_addr ipaddr;
	my_netif->upcb = udp_new();
	//�󶨱��ض˿�
	udp_bind(my_netif->upcb,&(my_netif->ipaddr),AS_SERVICE_PORT);
	//��Զ�˿ͻ���������
	IP4_ADDR(&ipaddr,192,168,1,105);
	udp_connect(my_netif->upcb,&ipaddr,REMOTE_PORT);
	//ע���û��ص�����
	udp_recv(my_netif->upcb,my_udp_callback,NULL);
}
//ͨ�����ڷ�������(Ҫ�����ڷ������ݹ�����������ͨ�ţ���ҪѰ��ԭ��)
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
	//����pbuf����
	memset(q->payload,0,q->len);
	memcpy(q->payload,str,strlen(str));
	udp_send(my_netif->upcb,q);
	printf("%s",str);
	memset(str,0,strlen(str));
	pbuf_free(q); //�ͷŻ���
}
/***********************************tcp��غ���****************************************************/
//��Ϊ������
//���������յ��ͻ��˵���������ʱ���ú����������ã�p�а�������������
err_t http_recv(
	void *arg,
	struct tcp_pcb *pcb,
	struct pbuf *p,
	err_t err
)
{
	char *data = NULL;
	if(p != NULL){
		tcp_recved(pcb,p->tot_len);    //���½��մ���
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
	//��� p == NULL ��ʾ�ͻ����ѹر����ӣ�������Ҳ��Ҫ�ر�����
	else if(err == ERR_OK){
		return tcp_close(pcb);
	}
	return ERR_OK;
}
//���пͻ��������Ӻ󣬸ú��������ã�pcbΪ�������Ӷ�Ӧ���¿��ƿ�
err_t http_accept(
	void *arg,
	struct tcp_pcb *pcb,
	err_t err
)
{
	tcp_recv(pcb,http_recv);  //ע��recv����
	return ERR_OK;
}
//http��������ʼ������
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
//��Ϊ�ͻ���
//������ʧ�ܣ���������������½����ͻ��ˣ����¿�ʼ����
void echo_client_conn_err(
		void *arg,
		err_t err
)
{
	printf("�������ӣ�����\r\n");
	echo_client_init();
}
//�����յ������������ݺ�����������ں˵��ã�p�а����˷���������
err_t echo_client_recv(
	void *arg,
	struct tcp_pcb *pcb,
	struct pbuf *p,
	err_t err
)
{
	if(p != NULL){
		tcp_recved(pcb,p->tot_len);  //���½��մ���
		tcp_write(pcb,p->payload,p->len,TCP_WRITE_FLAG_COPY);  //���Է�����������
		pbuf_free(p);
	}
	else if(err == ERR_OK){
		tcp_close(pcb);
		echo_client_init();
		printf("get serverice error!\n");
		return ERR_OK;
	}
}
//���ͻ�����������������ݳɹ�������������ں˵���
err_t echo_client_sent(
	void *arg,
	struct tcp_pcb *pcb,
	uint16_t len
)
{
	return ERR_OK;
}
//���ͻ���ͬ���������ӽ����ɹ�������������ں˵���
err_t echo_client_connected(
	void *arg,
	struct tcp_pcb *pcb,
	err_t err
)
{
	char GREETING[] = "HI, I am a new client!\r\n";
	tcp_recv(pcb,echo_client_recv);   //ע��recv����
	tcp_sent(pcb,echo_client_sent);   //ע��sent����
	tcp_write(pcb,GREETING,sizeof(GREETING),TCP_WRITE_FLAG_COPY);
	return ERR_OK;
}
//�ͻ��˳�ʼ������
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
