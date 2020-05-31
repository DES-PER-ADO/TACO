#include "app_JSON.h"

//uint8_t* djsonConstruct(char* imei,float temperature, char* longitude,char* latitude,char* GTime,char* LTime,int image,int Cycle)
uint8_t* djsonConstruct(char* imei,float* temperature, char WE, float* longitude,char SN,float* latitude,uint8_t* GTime,uint8_t* LTime,int image,int Cycle)
{
	//uint8_t* json;// = (unsigned char*)malloc(300);
	char* json;
	static uint8_t dd[500];
	/*sprintf(dd,"%s%s%s%3.1f%s%c%6.3f%s%c%6.3f%s%s%s%s%s%d%s%d%s","{\"imei\":\"",imei, "\",\"temperature\":",*temperature,",\"GPS Longitude\":\"",WE,*longitude,
		"\",\"GPS Latitude\":\"",SN,*latitude,"\",\"GPS Time\":\"", GTime,"\",\"Local Time\":\"", LTime,"\",\"Image\":",image,",\"Para Upload Cycle\":",Cycle,"}");*/
	//sprintf(dd,"%s%s%s%3.1f%s%c%6.3f%s%c%6.3f%s%s%s%s%s%d%s%d%s%c","{\"imei\":\"",imei, "\",\"temperature\":",*temperature,",\"GPS Longitude\":\"",WE,*longitude,"\",\"GPS Latitude\":\"",SN,*latitude,"\",\"GPS Time\":\"", GTime,"\",\"Local Time\":\"", LTime,"\",\"Image\":",image,",\"Para Upload Cycle\":",Cycle,"}",'\0');
	snprintf(dd,sizeof(dd),"{\"imei\":\"%s\",\"temperature\":%3.1f,\"GPS Longitude\":\"%c%6.3f\",\"GPS Latitude\":\"%c%6.3f\",\"GPS Time\":\"%s\",\"Local Time\":\"%s\",\"Image\":%d,\"Para Upload Cycle\":%d}",
			imei, *temperature,WE,*longitude,SN,*latitude,GTime, LTime,image,Cycle);


	json = malloc(strlen(dd)+16);
	strcpy(json, dd);
	return json;
}
