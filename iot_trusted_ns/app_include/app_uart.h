#ifndef _APP_UART_H
#define _APP_UART_H

#include "type.h"
#include "usdl_buf.h"
#include "fsl_usart.h"
#define UART_MAX_BUF_SIZE (1024*3)

typedef struct AppUartType_st
{
    USART_Type *   	Uart;//从hal库的一个结构体，变成了裸的usart基地址
    u8      		UartBuf[UART_MAX_BUF_SIZE];
    BufType 		CycleRecvBuf;
    u8      		C;
    usart_handle_t 	d_uartHandle;
}AppUartType;

extern void* AppUartNB;

u32 AppUartSend(void* app_uart, u8* buf, u32 len);
/*
u32 AppUartRecv(void* app_uart, u8* buf, u32 len, bool del);
bool AppUartInit(void* app_uart,u32 baudRate);
u32 AppUartClear(void* app_uart);
u32 AppUartDel(void* app_uart, u32 len);
extern bool GPS_recv_sentence;
*/
#endif
