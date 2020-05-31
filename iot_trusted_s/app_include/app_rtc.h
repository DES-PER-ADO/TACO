#ifndef _APP_RTC_H
#define _APP_RTC_H

#include "fsl_rtc.h"
#include "type.h"
#include "main.h"

void App_Set_RTC(rtc_datetime_t date);
void App_SetAlarm_RTC(u8 minute);

extern bool RTC_set;

#endif
