#ifndef __UART_H_
#define __UART_H_

typedef struct {
	int Qin;
	int Qout;
	char *Qbuf;
} UartRxdQueue_t;

#define FP_UART_RXD_BUF_SIZE 30
#define FP_UART_RXD_DEAL_SIZE 30
extern int rxlenth;
extern char idata FPRxdData[FP_UART_RXD_DEAL_SIZE];
void FP_UartDataInit();
void FP_UartRxdTask();
void Uart_Init_115200() ;
void Uart_Init_9600() ;
void Uart_Send(char* _data,int _len);
void Uart_Test();

#endif