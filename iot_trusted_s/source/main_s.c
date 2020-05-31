#if (__ARM_FEATURE_CMSE & 1) == 0
#error "Need ARMv8-M security extensions"
#elif (__ARM_FEATURE_CMSE & 2) == 0
#error "Compile with --cmse"
#endif

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "arm_cmse.h"
#include "board.h"
#include "veneer_table.h"
#include "tzm_config.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "main.h"

#include "rsa.h"
#include "secure_variable.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CODE_START_NS         0x00020000
#define NON_SECURE_START      CODE_START_NS
#define GPS_TIME_LIMIT 		  3
/* typedef for non-secure callback functions */
typedef void (*funcptr_ns) (void) __attribute__((cmse_nonsecure_call));

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void) 
{
/* The TrustZone should be configured as early as possible after RESET.
 * Therefore it is called from SystemInit() during startup. The SystemInitHook() weak function 
 * overloading is used for this purpose.
*/
    BOARD_InitTrustZone();
}
void CLOCK_AttachClktoFLEXCOM();
void App_Set_RTC(rtc_datetime_t date);

u32 Para_Upload_Cycle = 1;


int main(void)
{

	funcptr_ns ResetHandler_ns;
	rtc_datetime_t 		rtcdate;
	bool GPS_recv 		= false;
	u8	GPS_try_time	= 0;

    /* Init board hardware. */
    /* attach main clock divide to FLEXCOMM */
	CLOCK_AttachClktoFLEXCOM();

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();
    CASPER_Init(CASPER);
    ButtonLedInit();

    PRINTF("Hello from secure world!\r\n");

    rtcdate.year   = 1990U;
    rtcdate.month  = 1U;
    rtcdate.day    = 1U;
    rtcdate.hour   = 0U;
    rtcdate.minute = 0;
    rtcdate.second = 0;
    RTC_Init(RTC);
    App_Set_RTC(rtcdate);
    *Rtc_Alarm	   = true;

    app_adc_init();
    ButtonLedInit();
    //AppUartInit(AppUartNB,9600);
    AppNBInit();
    AppUartInit(AppUartDBG,38400);
    AppUartInit(AppUartGPS,9600);
    AppUartInit(AppUartCamera,38400);
    int image_sum = 0;
    extern bool Camera_Debug;
    Camera_Debug = false;

    while(GPS_try_time < GPS_TIME_LIMIT)
    {
    	GPS_recv = app_GPS_recv();//北斗。如果有信号，就会改变gps的值
    	if(GPS_recv)
    	{
    		rtcdate = d_GPS.time;
    		App_Set_RTC(rtcdate);
    		break;
    	}
    	GPS_try_time ++;
    }

    /* Set non-secure main stack (MSP_NS) */
    __TZ_set_MSP_NS(*((uint32_t *)(NON_SECURE_START)));
    /* Set non-secure vector table */
    SCB_NS->VTOR = NON_SECURE_START;
    /* Get non-secure reset handler */
    ResetHandler_ns = (funcptr_ns)(*((uint32_t *)((NON_SECURE_START) + 4U)));

    /* Call non-secure application */
    PRINTF("Entering normal world.\r\n");
    /* Jump to normal world */
    ResetHandler_ns();
    while (1)
    {
        /* This point should never be reached */
    }
}

void CLOCK_AttachClktoFLEXCOM()
{
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM6);
	CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);
}
