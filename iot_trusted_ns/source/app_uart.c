#include "app_uart.h"
//#include "LPC55S69_cm33_core0.h"

//#include "board.h"
#include "fsl_debug_console.h"

//#define UART_DEBUG 0

#define USART_CLK_SRC 	kCLOCK_Flexcomm0
#define USART_CLK_FREQ	CLOCK_GetFreq(kCLOCK_Flexcomm0)

bool GPS_recv_sentence = false;

AppUartType UartNB =
{
    .Uart = USART0,
    .CycleRecvBuf =
    {
        .Addr = UartNB.UartBuf,
        .Size = UART_MAX_BUF_SIZE,
        .RPos = 0,
        .WPos = 0,
        .Mode = BUF_MODE_CYCLE,
        .Apply = NULL,
        .Release = NULL
    },
};


void* AppUartNB  	= &UartNB;

void USART_UserCallback(USART_Type *base, usart_handle_t *handle, status_t status, void *userData)
{
    AppUartType* app_uart = (AppUartType*) userData;
    if (kStatus_USART_TxIdle == status)
    {
    	//PRINTF("SAD\n");
    }

    if (kStatus_USART_RxIdle == status)
    {
    	app_uart->C = handle->rxData;
    	BufAddData(&app_uart->CycleRecvBuf, &app_uart->C, 1);
    }
}


u32 AppUartSend(void* app_uart, u8* buf, u32 len)
{
	AppUartType* uart   = (AppUartType*)app_uart;
	status_t              usart_status;

	usart_transfer_t      app_data;
	app_data.data       = buf;
	app_data.dataSize   = len;

	/*发送data即可*/
    //USART_WriteBlocking(uart->Uart, buf, len);

	//usart_status = USART_TransferSendNonBlocking(uart->Uart, &dd_uartHandle, &app_data);
	usart_status = USART_TransferSendNonBlocking(uart->Uart, &uart->d_uartHandle, &app_data);

	if ((usart_status == kStatus_InvalidArgument)||(usart_status == kStatus_USART_TxBusy))
	{
		//if fail,return 0;
		return 0;
	}
	return len;
}

bool AppUartCreatHandle(void* app_uart)
{
	AppUartType* uart = (AppUartType*)app_uart;
	USART_TransferCreateHandle(uart->Uart, &uart->d_uartHandle, USART_UserCallback, NULL);
	uart->d_uartHandle.userData = uart;
	USART_EnableInterrupts(uart->Uart, kUSART_RxLevelInterruptEnable);

	return true;
}


bool AppUartInit(void* app_uart,u32 baudRate)
{
    AppUartType* uart = (AppUartType*)app_uart;

	usart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTxFifo = false;
     * config.enableRxFifo = false;
     */
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps = baudRate;//BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    //USART_Init(uart->Uart, &config, USART_CLK_FREQ);
    USART_Init(uart->Uart, &config, USART_CLK_FREQ);
    //USART_TransferCreateHandle(uart->Uart,&dd_uartHandle, USART_UserCallback, NULL);
    USART_TransferCreateHandle(uart->Uart, &uart->d_uartHandle, USART_UserCallback, NULL);
    //
    uart->d_uartHandle.userData = uart;
    //dd_uartHandle.userData = uart;

    USART_EnableInterrupts(uart->Uart, kUSART_RxLevelInterruptEnable);

    return TRUE;
}

u32 AppUartRecv(void* app_uart, u8* buf, u32 len, bool del)
{
    AppUartType* uart = (AppUartType*)app_uart;
    len = BufGetData(&uart->CycleRecvBuf, buf, 0, len);
    if (len != 0)
    {
        if (del != 0)
        {
            BufDelHeadData(&uart->CycleRecvBuf, len);
        }
    }
    return len;
}

//找到nxp的 uart receive callback 实现下面这个函数
/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}
*/

//不用改动。
u32 AppUartClear(void* app_uart)
{
    AppUartType* uart = (AppUartType*)app_uart;
    return BufClearData(&uart->CycleRecvBuf);
}

u32 AppUartDel(void* app_uart, u32 len)
{
    AppUartType* uart = (AppUartType*)app_uart;
    return BufDelHeadData(&uart->CycleRecvBuf, len);
}

