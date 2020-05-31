/***************************************************************************************************
*                    (c) Copyright 1992-2019 Embedded Products Research Center
*                                       All Rights Reserved
*
*\File          app_nbiot.h
*\Description
*\Note
*\Log           2019.08.01    Ver 1.0    冯传
*               创建文件。
***************************************************************************************************/
#ifndef _APP_NBIOT_H
#define _APP_NBIOT_H

#include "type.h"
#include "stdbool.h"
#include "app_uart.h"
//#include "main.h"
#define SERVER_DOMAIN "www.lwos.net"

bool AppNBIOTInit();
u32 AppNBRecvLenGet();
u32 AppNBDelete(u32 len);
u32 AppNBRecv(u8* buf, u32 len, bool del);
u32 AppNBSend(u8* buf, u32 len);
bool AppNBReset();

u32 BytesMapHexStr(u8* str, u32 max_str_len, u8* buf, u32 buf_len, const u8* seperator);
u32 HexStrToBytes(u8* buf, u32 buf_len, u8* hex, bool skip_invalid);



#define NB_MASTER_POWR_PIN          GPIO_PIN_11
#define NB_MASTER_POWER_GPIO_PORT   GPIOA

#define NB_RST_PIN          GPIO_PIN_8
#define NB_RST_GPIO_PORT    GPIOA

#define NB_PWR_PIN          GPIO_PIN_12
#define NB_PWR_GPIO_PORT    GPIOA

#define NB_PSM_INT_PIN          GPIO_PIN_15
#define NB_PSM_INT_GPIO_PORT    GPIOB

#endif /*_APP_NBIOT_H*/
