/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "veneer_table.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"


#include "app_nbiot.h"
#include "app_uart.h"
#include "secure_variable.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PRINTF_NSE DbgConsole_Printf_NSE
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void SystemInit (void)
{
}
/*!
 * @brief Main function
 */

int main(void)
{

    PRINTF_NSE("Welcome in normal world!\r\n");
    AppUartCreatHandle(AppUartNB);
    //printddd();
    while(1)
	{
		AppNBIOTInit();
	}
}
