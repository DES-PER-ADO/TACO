#include "type.h"
#include "fsl_lpadc.h"
#include "fsl_power.h"
#include "app_adc.h"
#include "fsl_debug_console.h"
#include "math.h"

#define DEBUG_ADC 0

#define APP_ADC_BASE ADC0
#define APP_ADC_IRQn ADC0_IRQn
#define APP_ADC_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define APP_ADC_USER_CHANNEL 0U
#define APP_ADC_USER_CMDID 1U /* CMD1 */
#define APP_ADC_VREF_SOURCE kLPADC_ReferenceVoltageAlt2
#define APP_ADC_DO_OFFSET_CALIBRATION false
#define APP_ADC_OFFSET_VALUE_A 10U
#define APP_ADC_OFFSET_VALUE_B 10U

#define ADC_FULL_RANGE 4095.0
#define ADC_FULL_VOLTAGE 3.3

volatile bool d_LpadcConversionCompletedFlag = false;
lpadc_conv_result_t d_LpadcResultConfigStruct;
const uint32_t d_Lpadc_12bitFullRange = 4096U;
//volatile uint32_t d_LpadcInterruptCounter = 0U;

//计算温度的变量
const float Rp=10000.0; //10K
const float T2 = (273.15+25.0);;//T2
const float Bx = 3950.0;//B
const float Ka = 273.15;

void APP_ADC_IRQ_HANDLER_FUNC(void)
{
    //g_LpadcInterruptCounter++;
    if (LPADC_GetConvResult(APP_ADC_BASE, &d_LpadcResultConfigStruct, 0U))
    {
        d_LpadcConversionCompletedFlag = true;
    }
}

bool app_adc_init()
{
	lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 16U, true);
    CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);

    /* Disable LDOGPADC power down */
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;

    //设定参考电压来源
    mLpadcConfigStruct.referenceVoltageSource = APP_ADC_VREF_SOURCE;
	//设定取平均值的数量
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage64;
    LPADC_Init(APP_ADC_BASE, &mLpadcConfigStruct);

//#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
//	#if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
//    //offset的校准
//    if (true == APP_ADC_DO_OFFSET_CALIBRATION)
//    {
//        LPADC_DoOffsetCalibration(APP_ADC_BASE);
//    }
//	else
//    {
//        LPADC_SetOffsetValue(APP_ADC_BASE, APP_ADC_OFFSET_VALUE_A, APP_ADC_OFFSET_VALUE_B);
//    }
//	#endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */
//    /* Request gain calibration. */
//    LPADC_DoAutoCalibration(APP_ADC_BASE);
//
//#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

	//初始化command    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber  = APP_ADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK131;
    LPADC_SetConvCommandConfig(APP_ADC_BASE, APP_ADC_USER_CMDID, &mLpadcCommandConfigStruct);

	//初始化trigger    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = APP_ADC_USER_CMDID; /* CMD15 is executed. */
    mLpadcTriggerConfigStruct.enableHardwareTrigger = true;//硬件触发
    //mLpadcTriggerConfigStruct.enableHardwareTrigger = false;//硬件触发
    LPADC_SetConvTriggerConfig(APP_ADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */

	/* Enable the watermark interrupt. */
    LPADC_EnableInterrupts(APP_ADC_BASE, kLPADC_FIFO0WatermarkInterruptEnable);

    EnableIRQ(APP_ADC_IRQn);

#if DEBUG_ADC
    PRINTF("hello adc");
#endif

    //LPADC_DoSoftwareTrigger(LPADC_BASE, 1U); /* 1U is trigger0 mask. */

	return TRUE;
}

float App_ADC_recv()
{
#if DEBUG_ADC
	while (1)
    {
        GETCHAR();
#endif
        LPADC_DoSoftwareTrigger(APP_ADC_BASE, 1U); /* 1U is trigger0 mask. */
        while (!d_LpadcConversionCompletedFlag)
        {
        }
        d_LpadcConversionCompletedFlag = false;

#if DEBUG_ADC
        PRINTF("ADC value: %d\r\n", ((d_LpadcResultConfigStruct.convValue) >> 3U));
        while((GETCHAR()!= '\n' )&&(GETCHAR()!= '\r'));
    }
#endif
		return d_LpadcResultConfigStruct.convValue >> 3U;
}

float Get_Voltage(float value)
{
	return (value/ADC_FULL_RANGE)*ADC_FULL_VOLTAGE;
}

float Get_Temp(float Rt)
{
  float temp;
  //like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25)),
  temp = Rt/Rp;
  temp = log(temp);//ln(Rt/Rp)
  temp/=Bx;//ln(Rt/Rp)/B
  temp+=(1/T2);
  temp = 1/(temp);
  temp-=Ka;
  return temp;
}

float app_temp_recv()
{
	//float temp    = 0;
	float vt 		= 0;
	float Rt 	= 0;
	vt = Get_Voltage(App_ADC_recv());

	Rt = ADC_FULL_VOLTAGE/(vt/10000.0) - 10000.0;

	return Get_Temp(Rt);
}

