#include "app_upload.h"
#include "fsl_debug_console.h"
#include "veneer_table.h"
#include "fsl_gpio.h"
#include "app_gpio.h"
#define DEMO_PLAY 	1

static const unsigned pubkey0[2048 / 32] = {
    0x232577d1, 0xa1c2d50d, 0x99706557, 0x46c818ab, 0xaa53916b, 0x63f5f64b, 0x93cd68f4, 0x1ed00fb7,
    0xea292749, 0x39de13c1, 0x065b5911, 0x884b13c9, 0xe6f6c061, 0xef47223e, 0x92e9e488, 0x3931a79d,
    0x14861755, 0xde93cc46, 0x1e74bf9a, 0xb4a3d058, 0x2b63c4b1, 0xd37d1bf7, 0x3fd70745, 0x5095a782,
    0xec4bcd7b, 0xbef831fb, 0x7f682470, 0x56a9a012, 0x6136775a, 0x2777c47d, 0x89b3f94a, 0x62fa6f9f
	,
    0xc97a18ab, 0x55d68409, 0x39007ccb, 0xe3514d48, 0xe817cc0e, 0xafd713ba, 0x14a82e21, 0xe5ff1433,
    0x385a8131, 0x31f2ece8, 0x8d395a2a, 0x85622d91, 0x67634847, 0xb219d21d, 0x1ef8efaa, 0xfaa05682,
    0x109b9a8a, 0x41042b7e, 0x0ebe7f64, 0xdae23bff, 0x5cfd544b, 0x74b9cbf2, 0x9563cafb, 0x462b3911,
    0x16e9cdf4, 0x68ed6d7f, 0xc6e45a20, 0x65838412, 0xa261fc8b, 0xbdd913f2, 0xc1782e4d, 0xbad47a84
};

static const unsigned pubkey_n[128/32]={0x80c07afc,0x9d25404d,0x6555b9ac,0xf3567cf1};//128位。16字节

static const unsigned pub_e = 0x10001;
static uint8_t plaintext[2048 / 8];
extern u32 Para_Upload_Cycle;

__attribute__ ((cmse_nonsecure_entry)) void upload_construct(u8* buf , u32*BufLen)
{
	u8* gps_time;
	u8* local_time;
	rtc_datetime_t date;
	float temperature;
	u32	image_sum;
	u8* digest;
	u32 buflen = 0;

	temperature = app_temp_recv();//温度
	image_sum	= app_camera_read();//平时不拍，在要上传的时候才拍照
	RTC_GetDatetime(RTC, &date);
	local_time 	= time_to_str(date);
	gps_time 	= time_to_str(d_GPS.time);

	uint8_t* d=djsonConstruct(IMEIStr,&temperature,d_GPS.locate.WE,&d_GPS.locate.longitude,d_GPS.locate.SN,&d_GPS.locate.latitude,\
			gps_time,local_time,image_sum,Para_Upload_Cycle);
    uint16_t dlen 	= strlen(d);

    dlen += 16;//对齐AES

    /******************************************************/
    /*													  */
    /*		encrypt and  Digital signature				  */
    /*			 										  */
    /******************************************************/
    app_aes_encrypt(d,dlen);
    //digest 			= app_sha256(d,dlen);//验算时，16进制，带长度
    //return;
    CASPER_ModExp(CASPER, (void *)d, (void *)pubkey_n, 4, pub_e, plaintext);//RSA,digest^e%pubkey
    //CASPER_ModExp(CASPER, d, pubkey_n, 4, pub_e, plaintext);//RSA,digest^e%pubkey

#if DEMO_PLAY
    PRINTF("temperature now: %3.1f\r\n",temperature);
    PRINTF("image sum: %d\r\n",image_sum);
    PRINTF("imei: %s\r\n",IMEIStr);
    PRINTF("GPS time: %s\r\n",gps_time);
    PRINTF("local_time: %s\r\n",local_time);
    PRINTF("GPS locate: longitude %c%6.3f,latitude %c%6.3f\r\n",d_GPS.locate.WE,d_GPS.locate.longitude,d_GPS.locate.SN,d_GPS.locate.latitude);
    PRINTF("Para_Upload_Cycle :%d\r\n",Para_Upload_Cycle);
    PRINTF("AES result: %s\r\n",d);
    //PRINTF("SHA256 result: %s\r\n",digest);
    PRINTF("RAS_sign: %s\r\n", plaintext);
#endif

    buf[0] 	= 0x68;
    buf[2] = (u8)(dlen & 0xff);
    buf[1] = (u8)((dlen >> 8) & 0xff);

    for(int i =0;i<dlen;i++)
    {
    	buf[3+i]=d[i];
    }
    for(int i=0;i<16;i++)
    {
    	buf[3+dlen+i]=plaintext[i];
    }
    buf[3+dlen+16] = 0x16;

    buflen = 3+dlen+16+1;
#if AT_DEBUG
    PRINTF("%s\r\n",buf);
#endif
    free(gps_time);
    free(local_time);
    free(d);
    *BufLen =  buflen;
    //return buflen;
}

__attribute__ ((cmse_nonsecure_entry)) void app_GPS_recv_s()
{
	bool GPS_recv = app_GPS_recv();//北斗。如果有信号，就会改变gps的值
	rtc_datetime_t 		rtcdate;
	if(GPS_recv)
	{
		rtcdate = d_GPS.time;
		App_Set_RTC(rtcdate);
	}
}

__attribute__ ((cmse_nonsecure_entry))  void download_construct(uint8_t* buf)
{
    signed char* index;
    signed char* end;
    uint8_t  paralen = 0;
    uint32_t para_tmp = 0;
    index = strstr(buf, "\"imei\":\"");

    if (index == NULL)
    {
        return ;
    }

    //注意接收来的报文的格式，空格之类的。
    for (int i = 0; i < strlen(IMEIStr); i++)
    {
        if (IMEIStr[i] != index[i + 8])
        {
            return ;
        }
    }

    index = strstr(buf, "Cycle");
    end = strchr(index, '}');

    if (index == NULL || end == NULL)
    {
        return ;
    }

    paralen = (end - (index + 7));
    for (int i = 0; i < paralen; i++)
    {
        para_tmp += (index[i + 7] - '0') * pow(10, paralen - i - 1);
    }
    Para_Upload_Cycle = para_tmp;

    App_SetAlarm_RTC(Para_Upload_Cycle);

    return ;
}

#define nb_rst_GPIO SECGPIO /*!<@brief GPIO device name: SECGPIO */
#define nb_rst_PORT 0U      /*!<@brief PORT device name: 0U */
#define nb_rst_PIN  1U   	/*!<@brief 0U pin index: 1 */

__attribute__ ((cmse_nonsecure_entry)) void nb_rst()
{
	GPIO_PortSet(nb_rst_GPIO, nb_rst_PORT, 1U << nb_rst_PIN);
	SDK_DelayAtLeastUs(4000000);
	GPIO_PortClear(nb_rst_GPIO, nb_rst_PORT, 1U << nb_rst_PIN);
}

__attribute__ ((cmse_nonsecure_entry)) void app_nb_send(u8* buf,u32 buflen)
{
	AppUartSend(AppUartNB, buf, buflen);
}

bool Camera_Debug = false;
__attribute__ ((cmse_nonsecure_entry)) void button_check()
{
	 if (!GPIO_PinRead(SECURE_SW1_GPIO, SECURE_SW1_GPIO_PORT, SECURE_SW1_GPIO_PIN))
	    {
		 while(!GPIO_PinRead(SECURE_SW1_GPIO, SECURE_SW1_GPIO_PORT, SECURE_SW1_GPIO_PIN))
		 {}
		 	 Camera_Debug = !Camera_Debug;
		 	 if(Camera_Debug)
		 	 {
		 		LED_GREEN_ON();
		 	 }
		 	 else
		 	 {
		 		LED_GREEN_OFF();
		 	 }
	    }
}

__attribute__ ((cmse_nonsecure_entry)) void printddd()
{
	PRINTF("call secure world!\r\n");
}
