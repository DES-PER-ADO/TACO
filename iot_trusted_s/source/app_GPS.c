//#include "type.h"

#include "app_GPS.h"
#include "stdio.h"
#define BEIJING 8

enum RCMSTATE
{
	RMCID = 0,
	UTCTIME,
	STATUS,
	LAT,//纬度
	uLAT,//纬度方向
	LON,
	uLON,
	SPD,
	COG,
	DATE,//日期
	MV,
	MVE,
	MODE,
	NAVSTATUS,
};
app_GPS d_GPS =
{
	.locate =
	{
		.WE 		= 'W',
		.longitude 	= 0,
		.SN 		= 'S',
		.latitude 	= 0,
	},
	.time	=
	{
		.year	= 1990,
		.month	= 1,
		.day	= 1,
		.hour	= 0,
		.minute	= 0,
		.second	= 0,
	},
};

bool app_GPS_recv()
{
	bool gps = false;
	if(GPS_recv_sentence == true)
	{
		gps = app_recv_GPS();
		GPS_recv_sentence = false;
	}
	return gps;
}

bool app_recv_GPS()
{
	static u8 GPS_temp_buf[4*1024];
	char * head = NULL,* end = NULL, * temp = NULL,*nexthead = NULL;
	char* UTCTIME_temp = NULL, * STATUS_temp = NULL, * LAT_temp = NULL, * uLAT_temp = NULL, * LON_temp = NULL, * uLON_temp = NULL, * DATE_temp = NULL;
	u8 state = 0;
	AppUartType *GPS_uart = (AppUartType*) AppUartGPS;
	AppUartRecv(AppUartGPS, GPS_temp_buf, sizeof(GPS_temp_buf), TRUE);
	head = strstr(GPS_temp_buf,"$GNRMC,");
	if(head == NULL)
	{
		return false;
	}
	end  = strchr(head, '*');
	if(end == NULL)
	{
		return false;
	}
	memset(end, '\0', 1);

	while (state <= DATE)
	{
		temp = strchr(head, ',');
		if(temp == NULL)
		{
			return false;
		}
		memset(temp, '\0', 1);
		nexthead = temp + 1;
		switch (state)
		{
		case RMCID:
		{
			state++;
			break;
		}
		case UTCTIME:
		{
			UTCTIME_temp = head;
			state++;
			break;
		}
		case STATUS:
		{
			STATUS_temp = head;
			state++;
			break;
		}
		case LAT:
		{
			LAT_temp = head;
			state++;
			break;
		}
		case uLAT:
		{
			uLAT_temp = head;
			state++;
			break;
		}
		case LON:
		{
			LON_temp = head;
			state++;
			break;
		}
		case uLON:
		{
			uLON_temp = head;
			state++;
			break;
		}
		case SPD:
		{
			state++;
			break;
		}
		case COG:
		{
			state++;
			break;
		}
		case DATE:
		{
			DATE_temp = head;
			state++;
			break;
		}
		default:
		{
			state = 0;
			break;
		}
		}
		head = nexthead;
	}
	if(*(u8*)STATUS_temp == 'A')
	{
		d_GPS.time.hour 	= (*(u8*)UTCTIME_temp - 48)*10 + *(u8*)(UTCTIME_temp+1) - 48 + BEIJING;
		d_GPS.time.minute 	= (*(u8*)(UTCTIME_temp+2) - 48)*10 + *(u8*)(UTCTIME_temp+3) - 48;
		d_GPS.time.second 	= (*(u8*)(UTCTIME_temp+4) - 48)*10 + *(u8*)(UTCTIME_temp+5) - 48;
		d_GPS.time.day 		= (*(u8*) DATE_temp - 48)*10 + *(u8*)(DATE_temp+1) - 48;
		d_GPS.time.month 	= (*(u8*)(DATE_temp+2) - 48)*10 + *(u8*)(DATE_temp+3) - 48;
		d_GPS.time.year		= (*(u8*)(DATE_temp+4) - 48)*10 + *(u8*)(DATE_temp+5) - 48 + 2000;
		d_GPS.locate.SN		 	= *uLAT_temp;
		d_GPS.locate.latitude	= (atof(LAT_temp + 2))/60.0 + (*(u8*)LAT_temp - 48)*10 + *(u8*)(LAT_temp+1) - 48;
		d_GPS.locate.WE		 	= *uLON_temp;
		d_GPS.locate.longitude  = (atof(LON_temp + 3))/60.0 + (*(u8*)LON_temp - 48)*100 + (*(u8*)(LON_temp+1) - 48)*10 + *(u8*)(LON_temp + 2) - 48;
		if(d_GPS.time.hour >= 24)
		{
			d_GPS.time.day += 1;
			d_GPS.time.hour-= 24;
		}
		return true;
	}
	else{
	 return false;
	}
	return false;
}

u8* time_to_str(rtc_datetime_t time)
{
	u8* date_str;
	u8 time_buf[20];
	sprintf(time_buf,"%d/%d/%d %d:%d",time.year,time.month,time.day,time.hour,time.minute);
	date_str = malloc(strlen(time_buf) + 1);
	strcpy(date_str, time_buf);
	return date_str;
}

