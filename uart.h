#ifndef __UART_H_
#define __UART_H_

typedef struct {
	int Qin;
	int Qout;
	char *Qbuf;
} UartRxdQueue_t;

#define FP_UART_RXD_BUF_SIZE 20
#define FP_UART_RXD_DEAL_SIZE 20


void FP_UartDataInit();
void FP_UartRxdTask();
void Uart_Init_9600() ;
void Uart_Send(char* _data,int _len);


#endif