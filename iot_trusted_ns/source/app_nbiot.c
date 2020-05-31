/***************************************************************************************************
*                    (c) Copyright 1992-2019 Embedded Products Research Center
*                                       All Rights Reserved
*
*\File          nb_process.c
*\Description
*\Note
*\Log           2019.08.01    Ver 1.0    冯传
*               创建文件。
***************************************************************************************************/

#include "app_nbiot.h"
#include "veneer_table.h"
#include "usdl_tools.h"
#include "usdl_buf.h"
#include "usdl_mem.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nb_process.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "cmsis_os.h"
#include "secure_variable.h"
#include "app_nsc.h"
#define AT_DEBUG  	0


//#define osDelay HAL_Delay
#define nb_rst_GPIO SECGPIO /*!<@brief GPIO device name: SECGPIO */
#define nb_rst_PORT 0U      /*!<@brief PORT device name: 0U */
#define nb_rst_PIN  1U       /*!<@brief 0U pin index: 1 */

//s8      IMEIStr[16];        //IMEI号

static u32 CurrSockNum = 0xff;

typedef enum
{
    RE_ERROR,   //nb return error
    RE_OK,      //nb return ok
    RE_WAIT,    //nb returning
    RE_READY,   //nb complete,means can send next
    RE_COMPLETE,//nb complete,current task complete, nb has sleep(in psm mod)
    RE_INCORRECT//nb data incorrect,maybe recv error
}AnaType;


typedef enum
{
    NB_OK,
    NB_ERROR,
    NB_ONE_STR,
    NB_ONE_PARA,
    NB_TWO_PARA,
    NB_THREE_PARA,
    NB_SOCKNUM,
    NB_SOCKRECV,
    NB_NSOSTR,
    NB_SONMI,
    NB_DNS,
    NB_CMD_NUM
}ResultType;
const int8_t* const CmdsRe[NB_CMD_NUM] =
{
    "\r\nOK\r\n",
    "\r\nERROR\r\n",
    "\r\n%[^:]:%[^:\r]\r\n\r\nOK\r\n",
    "\r\n%[^:]:%d\r\n\r\nOK\r\n",
    "\r\n%[^:]:%d,%d\r\n\r\nOK\r\n",
    "\r\n%[^=]=%d,%[^=]=%d,%[^=]=%d\r\n\r\nOK\r\n",
    "\r\n%d\r\n\r\nOK\r\n",
    "\r\n%d,%[^,],%d,%d,%[^,],%d\r\n\r\nOK\r\n",//1,192.168.5.1,1024,2,ABAB,0
    "\r\n+NSOSTR:%d,100,1\r\n",
    "\r\n+NSONMI:%d,%d\r\n",
    "\r\n%[^+]+QDNS:%s\r\n"
};

typedef enum
{
    NB_AT_RESET,
    NB_AT_ECHO_OFF,
    NB_AT_VERSION,
    NB_AT_LIGHT,
    NB_AT_SIM_CHECK,
    NB_AT_FUN1,
    NB_AT_EDRX,
    NB_AT_CPSMS0,
    NB_AT_NNMI,
    NB_AT_CGATT_ON,
    NB_AT_CGATT,
    NB_AT_CEREG,
    NB_AT_CSQ,
    NB_AT_DNS,
    NB_AT_DNS_CFG,
    NB_AT_NSOCR,
    NB_AT_NSOST,
    NB_AT_NSORF,
    NB_AT_CGSN,
    NB_AT_CGATT_OFF,
    NB_AT_SLEEP,
    NB_STAT_NUM_IDLE
}StateType;
typedef struct
{
    u8* ATCmd;
    StateType ST;
    ResultType RT;
    uint8_t RectryCount;
    uint8_t WaitTime;//1=200ms
}StateReMap;


const StateReMap MapList[NB_STAT_NUM_IDLE] =
{
    { "AT+NRB\r\n",                             NB_AT_RESET,        NB_OK,          1,  50},
    { "ATE0\r\n",                               NB_AT_ECHO_OFF,     NB_OK,          5,  20},
    { "ATI\r\n",                                NB_AT_VERSION,      NB_OK,          5,  20},
    { "AT+QLEDMODE=1\r\n",                      NB_AT_LIGHT,        NB_OK,          20, 10},
    { "AT+NCCID?\r\n",                          NB_AT_SIM_CHECK,    NB_OK,          20, 10},
    { "AT+CFUN=1\r\n",                          NB_AT_FUN1,         NB_OK,          10, 40},
    { "AT+CEDRXS=1,5,\"0010\"\r\n",             NB_AT_EDRX,         NB_OK,          10, 30},
    //{ "AT+CEDRXS?\r\n",               NB_AT_EDRX,         NB_OK,          10, 30},
    { "AT+CPSMS=0,,,01011111,01011111\r\n",     NB_AT_CPSMS0,       NB_OK,          10, 20},
    { "AT+NSONMI=0\r\n",                        NB_AT_NNMI,         NB_OK,          5,  20},
    { "AT+CGATT=1\r\n",                         NB_AT_CGATT_ON,     NB_OK,          10, 20},
    { "AT+CGATT?\r\n",                          NB_AT_CGATT,        NB_ONE_PARA,    20, 20},
    { "AT+CEREG?\r\n",                          NB_AT_CEREG,        NB_TWO_PARA,    10, 20},
    { "AT+CSQ\r\n",                             NB_AT_CSQ,          NB_TWO_PARA,    5,  20},
    { "AT+QDNS=0,%s\r\n",                       NB_AT_DNS,          NB_DNS,         1, 200},
    { "AT+QIDNSCFG=114.114.114.114,8.8.8.8\r\n",NB_AT_DNS_CFG,      NB_OK,          10, 20},
    { "AT+NSOCR=DGRAM,17,3005,1,AF_INET\r\n",   NB_AT_NSOCR,        NB_SOCKNUM,     5,  20},
    { "AT+NSOST=%d,%s,3005,%d,%s\r\n",          NB_AT_NSOST,        NB_OK,          5,  20},
    { "AT+NSORF=%d,1300\r\n",                   NB_AT_NSORF,        NB_SOCKRECV,    50, 30},
    { "AT+CGSN=1\r\n",                          NB_AT_CGSN,         NB_ONE_STR,     10, 20},
    { "AT+CFUN=0\r\n",                          NB_AT_CGATT_OFF,    NB_OK,          10, 20},
    { "AT+CPSMS=1,,,01000001,00000001\r\n",     NB_AT_SLEEP,        NB_OK,          1,  20}
};

uint8_t NBServerAddr[16] = {"47.106.194.107"};//{ "139.129.217.11" };//111.111.111.111\0

#define AT_MAX_BUF_SIZE (1024*2)
#define AT_SEND_ONE_LEN (1024)
static uint8_t ATTmpBuf[3000];
static bool NBResetFlag = TRUE;
static uint8_t buf_nb[AT_MAX_BUF_SIZE];
static u8 NBRecvDataBuf[AT_MAX_BUF_SIZE];
static BufType NBCycleRecvBuf =
{
    .Addr = NBRecvDataBuf,
    .Size = AT_MAX_BUF_SIZE,
    .RPos = 0,
    .WPos = 0,
    .Mode = BUF_MODE_CYCLE,
    .Apply = NULL,
    .Release = NULL
};

static u8 NBSendBuf[AT_MAX_BUF_SIZE];
static u32 NBSendBufLen = 0;
static bool NBSendBusy = FALSE;

bool ImeiCheck(uint8_t* im)
{
    int8_t i = 0;
    for (i = 0; i < 15; i++)
    {
        if ((im[i] >= '0' && im[i] <= '9'))
        {
            continue;
        }
        return FALSE;
    }

    return TRUE;
}

void ATSch(void const* para);


bool AppNBIOTInit()
{
	/*
	osThreadDef(taskAtCollect, ATSch, osPriorityNormal, 0, 512);
    osThreadCreate(osThread(taskAtCollect), NULL);
    return TRUE;
    */

	ATSch(NULL);
}


void nb_rst();
/*
{
	GPIO_PortSet(nb_rst_GPIO, nb_rst_PORT, 1U << nb_rst_PIN);
	SDK_DelayAtLeastUs(4000000);
	GPIO_PortClear(nb_rst_GPIO, nb_rst_PORT, 1U << nb_rst_PIN);
}
*/

u32 AppNBRecvLenGet()
{
    return BufGetDataLen(&NBCycleRecvBuf);
}
u32 AppNBDelete(u32 len)
{
    return BufDelHeadData(&NBCycleRecvBuf, len);
}
u32 AppNBRecv(u8* buf, u32 len, bool del)
{
    len = BufGetData(&NBCycleRecvBuf, buf, 0, len);
    if (len != 0)
    {
        if (del != 0)
        {
            BufDelHeadData(&NBCycleRecvBuf, len);
        }
    }
    return len;
}

u32 AppNBSend(u8* buf, u32 len)
{
    u32 timeout = 0;
    u32 len_sent = 0;
    u32 one_len = 0;
    do
    {
        while (NBSendBusy == TRUE)
        {
            timeout++;
            if (timeout >= 100)
            {
                return 0;
            }
            //osDelay(100);
            SDK_DelayAtLeastUs(100000);
            continue;
        }

        one_len = Min(len, AT_SEND_ONE_LEN);

        memcpy(NBSendBuf, buf + len_sent, one_len);
        NBSendBufLen = one_len;

        len_sent += one_len;
        len -= one_len;
    } while (len != 0);

    return len_sent;
}

bool AppNBReset()
{
    NBResetFlag = TRUE;
    return TRUE;
}

AnaType WaitResult(const StateReMap* cmd_type, void* r1, void* r2, void* r3, void* r1_str, void* r2_str, void* r3_str);
void ATSch(void const* para)
{
    static StateType st = NB_AT_RESET;
    static AnaType re = RE_READY;//at back data ok
    static uint16_t retry = 20;
    static uint8_t buf[AT_MAX_BUF_SIZE];
    static uint8_t buf_tmp[AT_MAX_BUF_SIZE];
    //cJSON* json_tmp = NULL;
    //uint8_t buf_data_len = 0;
    bool net_faile = FALSE;
    uint8_t restart_count = 0;
    uint8_t wait_data_count = 0;
    uint32_t r1 = 0;
    uint32_t r2 = 0;
    uint32_t r3 = 0;

    while (1)
    {
    	app_GPS_recv_s();

        button_check();
    	/*
    	AppNBRecv(buf_nb,AT_MAX_BUF_SIZE,true);
    	download_construct(buf_nb);//download_para
		*/
        //HAL_Delay(300);
        SDK_DelayAtLeastUs(300000);
        //AppUartSend(AppUartNB, "hello", 5);
        if (re == RE_READY)
        {
            r1 = 0;
            //判断是否需要获取IMEI号
            if ((st == NB_AT_NSOCR) && (IMEIStr[0] == 0))
            {
                st = NB_AT_CGSN;
            }
            //判断是否需要解析DNS
            else if ((st == NB_AT_NSOCR) && (NBServerAddr[0] == 0))
            {
                st = NB_AT_DNS;
            }

            //判断是否有数据需要发送
            if (((st == NB_AT_CSQ) || (st == NB_AT_NSORF)) && (NBSendBufLen != 0))
            {
                st = NB_AT_NSOST;
            }

            if (NBResetFlag == TRUE)
            {
                st = NB_AT_RESET;
                NBResetFlag = FALSE;
            }

            if (st == NB_AT_RESET)
            {
                //AppIOSet(IO_O_NB_PWR_ONOFF, 1, IO_FLASH_NONE);
                //HAL_Delay(1000);
                SDK_DelayAtLeastUs(1000000);
                //AppIOSet(IO_O_NB_PWR_ONOFF, 0, IO_FLASH_NONE);

                nb_rst();
            }
            else if (st == NB_AT_DNS)
            {
                r1 = sprintf((char*)buf, (char*)MapList[st].ATCmd, SERVER_DOMAIN);
            }
            else if (st == NB_AT_NSOST)
            {
            	//strcpy(NBSendBuf,"hello");
				//NBSendBufLen = 5;
            	if(*Rtc_Alarm)
            	{
            		//NBSendBufLen = 0;
       				upload_construct(NBSendBuf , &NBSendBufLen);
            		*Rtc_Alarm = false;
            	}

                if (NBSendBufLen != 0)
                {
                    BytesMapHexStr(buf_tmp, sizeof(buf_tmp), (u8*)NBSendBuf, NBSendBufLen, NULL);
                }
                else
                {
                    NBSendBufLen = 1;
                    BytesMapHexStr(buf_tmp, sizeof(buf_tmp), (u8*)" ", 1, NULL);
                }
                r1 = sprintf((char*)buf, (char*)MapList[st].ATCmd, CurrSockNum, NBServerAddr, NBSendBufLen, buf_tmp);
            }
            else if (st == NB_AT_NSORF)
            {
                r1 = sprintf((char*)buf, (char*)MapList[st].ATCmd, CurrSockNum);
            }
            else
            {
                strcpy((char*)buf, (char*)MapList[st].ATCmd);
                r1 = strlen((char*)MapList[st].ATCmd);
            }
            if (r1 != 0)
            {
                //send command
                AppUartClear(AppUartNB);
                r1 = AppUartSend(AppUartNB, buf, r1);
#if AT_DEBUG
                //AppUartSend(AppUartDBG, (uint8_t *)buf, r1);
                PRINTF(buf);
#endif
            }

            //AppIOSet(IO_O_NB_STATE, 0, IO_FLASH_NONE);
            re = RE_WAIT;
        }
        else if ((re == RE_WAIT) || (re == RE_INCORRECT))
        {
            r1 = r2 = r3 = 0;
            re = WaitResult(&MapList[st], &r1, &r2, &r3, buf, buf_tmp, buf_tmp);

            if (re == RE_ERROR)
            {
                if (NB_AT_DNS_CFG == st)
                {
                    st = NB_AT_DNS;
                    re = RE_READY;
                    continue;
                }

                //AppIOSet(IO_O_NB_STATE, 7, IO_FLASH_NONE);
                retry--;
                if (st != NB_AT_RESET)
                {
                    re = RE_READY;//resend cmd
                }
                else
                {
                    re = RE_WAIT;//don't resend cmd,still wait until retry == 0
                }
                if (retry == 0)
                {
                    //max nb module restart count is 2,then go to psm mod, and return fault
                    restart_count++;
                    if (restart_count > 1)
                    {
                        st = NB_AT_SLEEP;
                        re = RE_READY;
                        restart_count = 0;
                    }
                    else
                    {
                        //reset the nb module
                        st = NB_AT_RESET;
                        re = RE_READY;
                    }
                    retry = MapList[st].RectryCount;
                }
            }
            else if (re == RE_OK)
            {
                switch (st)
                {
                case NB_AT_RESET:
                    st = NB_AT_ECHO_OFF;
                    //AppIOSet(IO_O_NB_STATE, 0, IO_FLASH_NONE);
                    //AppIOSet(IO_O_NB_SIM, 1, IO_FLASH_NONE);
                    break;
                case NB_AT_ECHO_OFF:
                    st = NB_AT_VERSION;
                    break;
                case NB_AT_VERSION:
                    st = NB_AT_LIGHT;
                    break;
                case NB_AT_LIGHT:
                    st = NB_AT_SIM_CHECK;
                    break;
                case NB_AT_SIM_CHECK:
                    st = NB_AT_FUN1;
                    //AppIOSet(IO_O_NB_SIM, 0, IO_FLASH_NONE);
                    break;
                case NB_AT_FUN1:
                    st = NB_AT_EDRX;
                    break;
                case NB_AT_EDRX:
                    st = NB_AT_CPSMS0;
                    break;
                case NB_AT_CPSMS0:
                    st = NB_AT_CGSN;
                    break;

                case NB_AT_CGSN:
                    memcpy(IMEIStr, buf, 15);
                    st = NB_AT_CGATT_ON;
                    break;
                case NB_AT_CGATT_ON:
                    st = NB_AT_CGATT;
                    wait_data_count = 0;
                    net_faile = FALSE;
                    break;
                case NB_AT_CGATT:
                    if (r1 == 1)
                    {
                        st = NB_AT_CEREG;
                        wait_data_count = 0;
                        //AppIOSet(IO_O_NB_STATE, 1, IO_FLASH_NONE);
                    }
                    else
                    {
                        wait_data_count++;
                        if (wait_data_count >= 40)
                        {
                            st = NB_AT_RESET;
                        }
                    }
                    break;
                case NB_AT_CEREG:
                    //goto next stat
                    if (r2 == 1)
                    {
                        st = NB_AT_DNS_CFG;
                        //st = NB_AT_NSOCR;
                        //AppIOSet(IO_O_NB_STATE, 2, IO_FLASH_NONE);
                        wait_data_count = 0;
                    }
                    else
                    {
                        wait_data_count++;
                        if (wait_data_count >= 20)
                        {
                            st = NB_AT_RESET;
                        }
                    }
                    break;

                case NB_AT_DNS_CFG:
                    st = NB_AT_DNS;
                    break;
                case NB_AT_DNS:
                    //goto next stat
                    st = NB_AT_NSOCR;

                    if (strcmp(buf, "FAIL") == 0)
                    {
                        st = NB_AT_DNS_CFG;
                        wait_data_count++;
                        if (wait_data_count >= 5)
                        {
                            st = NB_AT_NSOCR;
                        }
                    }
                    else
                    {
                        strcpy((char*)NBServerAddr, (char*)buf);
                        //AppIOSet(IO_O_NB_STATE, 3, IO_FLASH_NONE);
                    }

                    break;
                case NB_AT_NSOCR:
                    st = NB_AT_NNMI;
                    CurrSockNum = r1;
                    break;
                 case NB_AT_NNMI:
                    st = NB_AT_CSQ;
                    break;
                case NB_AT_CSQ:
                    //RunningPara.CSQ = r1;
                    if (r1 == 99)
                    {
                        wait_data_count++;
                        if (wait_data_count >= 5)
                        {
                            st = NB_AT_RESET;
                        }
                        //AppIOSet(IO_O_CSQ, 0, IO_FLASH_NONE);
                    }
                    else
                    {
                        //AppIOSet(IO_O_CSQ, 1, r1 / 5);

                        st = NB_AT_NSOST;
                    }
                    break;
                case NB_AT_NSOST:
                    st = NB_AT_NSORF;
                    NBSendBusy = FALSE;
                    NBSendBufLen = 0;
                    //AppIOSet(IO_O_NB_STATE, 4, IO_FLASH_NONE);
                    break;

                case NB_AT_NSORF://PENDING=0,SENT=1,ERROR=0
                    //send complete, start query read
                    if (r1 != 0)
                    {
                        wait_data_count = 0;

                        r1 = HexStrToBytes(buf_tmp, sizeof(buf_tmp), buf, FALSE);
                        BufAddData(&NBCycleRecvBuf, buf_tmp, r1);
//                        download = download_construct(buf_tmp);//download_para
                        //AppIOSet(IO_O_NB_STATE, 5, IO_FLASH_NONE);
                    }
                    else
                    {
                        //AppIOSet(IO_O_NB_STATE, 6, IO_FLASH_NONE);
                    }


                    wait_data_count++;
                    if (wait_data_count >= 20)
                    {
                        st = NB_AT_CSQ;
                        wait_data_count = 0;
                    }
                    break;

                case NB_AT_CGATT_OFF:
                    st = NB_AT_RESET;
                    net_faile = TRUE;
                    break;
                case NB_AT_SLEEP:
                    //nb has sleep
                    re = RE_READY;
                    if (net_faile == TRUE)
                    {
                        st = NB_AT_RESET;
                    }
                    else
                    {
                        st = NB_AT_CSQ;
                    }
                    retry = MapList[st].RectryCount;
                default:
                    st = NB_AT_RESET;
                }

                //go to next cmd
                re = RE_READY;
                retry = MapList[st].RectryCount;
            }
        }
        else
        {
            st = NB_AT_RESET;
            re = RE_READY;
            retry = MapList[st].RectryCount;
        }
    }
}

AnaType WaitResult(const StateReMap* cmd_type, void* r1, void* r2, void* r3, void* r1_str, void* r2_str, void* r3_str)
{

    static uint16_t time_out = 0;
    static uint16_t last_len = 0;
    uint8_t* tmp_ptr = NULL;
    uint16_t length = 0;
    uint16_t length_need_del = 1;
    uint8_t num = 0;

    if (time_out >= cmd_type->WaitTime)
    {
        //time out = fault
        time_out = 0;
        AppUartClear(AppUartNB);
        return RE_ERROR;
    }

    memset(ATTmpBuf, 0, sizeof(ATTmpBuf));
    length = AppUartRecv(AppUartNB, ATTmpBuf, sizeof(ATTmpBuf), FALSE);
    //new data come, reset timeout count
    if (length != last_len)
    {
        time_out = 0;
    }
    last_len = length;

    if (length < 6)
    {
        //time out increase
        time_out++;
        return RE_WAIT;
    }

#if AT_DEBUG
    //AppUartSend(AppUartDBG, (uint8_t *)ATTmpBuf, length);
    PRINTF(ATTmpBuf);
#endif

    //check
    if (MemMem((uint8_t*)ATTmpBuf, length, (uint8_t*)CmdsRe[NB_ERROR], strlen((const char*)CmdsRe[NB_ERROR])) != 0)
    {
        //fault
        AppUartClear(AppUartNB);
        return RE_ERROR;
    }

    //data pre ok, need to delete
    tmp_ptr = (uint8_t*)MemMem((u8*)ATTmpBuf, length, (u8*)CmdsRe[NB_OK], strlen((const char*)CmdsRe[NB_OK]));
    if ((tmp_ptr == 0) && ((MemMem((u8*)ATTmpBuf, length, (u8*)"+NSONMI", 7) == 0) || (MemMem((u8*)ATTmpBuf, length, (u8*)"+QDNS", 5) == 0)))
    {
        time_out++;
        return RE_WAIT;
    }
    //ok
    length_need_del = tmp_ptr - ATTmpBuf + strlen((char*)CmdsRe[NB_OK]);
    if (cmd_type->RT == NB_OK)
    {
        AppUartClear(AppUartNB);
        return RE_OK;
    }

    //recv again
    length = AppUartRecv(AppUartNB, ATTmpBuf, sizeof(ATTmpBuf), FALSE);

    if (cmd_type->RT == NB_SOCKRECV)
    {
        if(length > 500)
        {
            length++;length--;
        }
        //1,192.168.5.1,1024,2,ABAB,0
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r2, r2_str, r3, r1, r1_str, r3);
        //get para
        if (num == 6)
        {
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
    }

    if (cmd_type->RT == NB_DNS)
    {
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r2_str, r1_str);
        //get para
        if (num == 2)
        {
            AppUartClear(AppUartNB);
            return RE_OK;
        }
        else
        {
            time_out++;
            return RE_WAIT;
        }
    }

    if (cmd_type->RT == NB_SONMI)
    {
        //    "\r\n+NSONMI:%d,%[^,],%d,%d,%[^\r]\r\n"
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r2, r3, r3_str, r1, r2_str, r3, r1, r1_str);
        //get para
        if (num == 8)
        {
            AppUartClear(AppUartNB);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            time_out = 0;
            AppUartDel(AppUartNB, 1);
            return RE_WAIT;
        }
    }

    if (cmd_type->RT == NB_SOCKNUM)
    {
        //get para
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r1);
        if (num == 1)
        {
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            time_out = 0;
            AppUartDel(AppUartNB, 1);
            return RE_INCORRECT;
        }
    }

    //s-r-s-r-s-r
    if (cmd_type->RT == NB_THREE_PARA)
    {
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r1_str, r1, r2_str, r2, r3_str, r3);
        //get para
        if (num == 6)
        {
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            time_out = 0;
            AppUartDel(AppUartNB, 1);
            return RE_INCORRECT;
        }
    }

    //s-r-r
    if (cmd_type->RT == NB_TWO_PARA)
    {
        //get para
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r1_str, r1, r2);
        if (num == 3)
        {
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            time_out = 0;
            AppUartDel(AppUartNB, 1);
            return RE_INCORRECT;
        }
    }


    //s-r
    if (cmd_type->RT == NB_ONE_PARA)
    {
        //get para
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r1_str, r1);
        if (num == 2)
        {
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            time_out = 0;
            AppUartDel(AppUartNB, 1);
            return RE_INCORRECT;
        }
    }
    //s-s
    if (cmd_type->RT == NB_ONE_STR)
    {
        //get para
        num = sscanf((char*)ATTmpBuf, (char*)CmdsRe[cmd_type->RT], r1_str, r1_str);
        if (num == 2)
        {
            AppUartDel(AppUartNB, length_need_del);
            return RE_OK;
        }
        else
        {
            //parse fail wait restart
            time_out = 0;
            AppUartDel(AppUartNB, 1);
            return RE_INCORRECT;
        }
    }


    return RE_ERROR;
}

const u8 HexList[] = "0123456789ABCDEF";
u32 BytesMapHexStr(u8* str, u32 max_str_len, u8* buf, u32 buf_len, const u8* seperator)
{
    u32 i;
    u32 trunk_len = 0;
    u32 str_len = 0;

    if ((str == NULL) || (buf == NULL))
    {
        return 0;
    }

    if (seperator == NULL)
    {
        if ((buf_len * 2) > max_str_len)
        {
            return 0;
        }
        for (i = 0; i < buf_len; i++)
        {
            str[i * 2] = HexList[buf[i] / 16];
            str[i * 2 + 1] = HexList[buf[i] % 16];
        }
        str[buf_len * 2] = 0x00;
        return buf_len * 2;
    }

    trunk_len = 2 + strlen((char*)seperator);

    if (max_str_len < (buf_len * trunk_len))
    {
        buf_len = max_str_len / trunk_len;
    }
    str_len = 0;
    for (i = 0; i < buf_len; i++)
    {
        sprintf((char*)str + str_len, "%0*X%s", 2, buf[i], seperator);
        str_len += trunk_len;
    }

    if (buf_len > 0)
    {
        str_len -= strlen((char*)seperator);
    }

    str[str_len] = 0x00;
    return str_len;
}

u32 HexStrToBytes(u8* buf, u32 buf_len, u8* hex, bool skip_invalid)
{
    u32 str_len = 0;
    bool odd_digit = FALSE;
    u32 i;
    u8 ch;
    u8 tmp;

    str_len = strlen((char*)hex);
    if (str_len > buf_len * 2)
    {
        str_len = buf_len * 2;
    }

    for (i = 0; i < str_len; ++i)
    {
        ch = hex[i];

        if (ch >= '0' && ch <= '9')
        {
            tmp = ch - '0';
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            tmp = ch - 'a' + 10;
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            tmp = ch - 'A' + 10;
        }
        else
        {
            if (skip_invalid == TRUE)
            {
                continue;
            }
            else
            {
                return 0;
            }
        }

        if (odd_digit)
        {
            *buf |= tmp;
            buf++;
            odd_digit = FALSE;
        }
        else
        {
            *buf = tmp << 4;
            odd_digit = TRUE;
        }
    }
    return (str_len + 1) / 2;
}

