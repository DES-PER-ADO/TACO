#include "secure_variable.h"

#define RTC_ALARM_ADDRESS 	(0x20040100u)
bool* Rtc_Alarm = (bool*) RTC_ALARM_ADDRESS;

#define IMEIStr_ADDRESS 	(0x20040000u)
s8* IMEIStr 	= (s8*)IMEIStr_ADDRESS;
