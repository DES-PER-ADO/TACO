/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v6.0
processor: LPC55S69
package_id: LPC55S69JBD100
mcu_data: ksdk2_0
processor_version: 6.0.2
pin_labels:
- {pin_num: '7', pin_signal: PIO0_1/FC3_CTS_SDA_SSEL0/CT_INP0/SCT_GPI1/SD1_CLK/CMP0_OUT/SECURE_GPIO0_1, label: nb_rst_N, identifier: nb_rst;nb_rst_N}
- {pin_num: '88', pin_signal: PIO0_5/FC4_RXD_SDA_MOSI_DATA/CTIMER3_MAT0/SCT_GPI5/FC3_RTS_SCL_SSEL1/MCLK/SECURE_GPIO0_5, label: S1 button}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '92', peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI_DATA/SD1_D2/CTIMER2_MAT3/SCT0_OUT8/CMP0_OUT/PLU_OUT2/SECURE_GPIO0_29,
    mode: inactive, slew_rate: standard, invert: disabled, open_drain: disabled}
  - {pin_num: '94', peripheral: FLEXCOMM0, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_30/FC0_TXD_SCL_MISO_WS/SD1_D3/CTIMER0_MAT0/SCT0_OUT9/SECURE_GPIO0_30, mode: inactive,
    slew_rate: standard, invert: disabled, open_drain: disabled}
  - {pin_num: '7', peripheral: SECGPIO, signal: 'SECPIO0, 1', pin_signal: PIO0_1/FC3_CTS_SDA_SSEL0/CT_INP0/SCT_GPI1/SD1_CLK/CMP0_OUT/SECURE_GPIO0_1, identifier: nb_rst_N,
    direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: '60', peripheral: FLEXCOMM2, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_26/FC2_RXD_SDA_MOSI_DATA/CLKOUT/CT_INP14/SCT0_OUT5/USB0_IDVALUE/FC0_SCK/HS_SPI_MOSI/SECURE_GPIO0_26}
  - {pin_num: '27', peripheral: FLEXCOMM2, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_27/FC2_TXD_SCL_MISO_WS/CTIMER3_MAT2/SCT0_OUT6/FC7_RXD_SDA_MOSI_DATA/PLU_OUT0/SECURE_GPIO0_27}
  - {pin_num: '40', peripheral: FLEXCOMM1, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO1_10/FC1_RXD_SDA_MOSI_DATA/CTIMER1_MAT0/SCT0_OUT3}
  - {pin_num: '93', peripheral: FLEXCOMM1, signal: TXD_SCL_MISO_WS, pin_signal: PIO1_11/FC1_TXD_SCL_MISO_WS/CT_INP5/USB0_VBUS}
  - {pin_num: '18', peripheral: ADC0, signal: VREFN, pin_signal: VREFN}
  - {pin_num: '17', peripheral: ADC0, signal: VREFP, pin_signal: VREFP}
  - {pin_num: '16', peripheral: ADC0, signal: VDDA, pin_signal: VDDA}
  - {pin_num: '19', peripheral: ADC0, signal: VSSA, pin_signal: VSSA}
  - {pin_num: '20', peripheral: ADC0, signal: 'CH, 0', pin_signal: PIO0_23/MCLK/CTIMER1_MAT2/CTIMER3_MAT3/SCT0_OUT4/FC0_CTS_SDA_SSEL0/SD1_D1/SECURE_GPIO0_23/ADC0_0}
  - {pin_num: '24', peripheral: ADC0, signal: 'CH, 4', pin_signal: PIO1_8/FC0_CTS_SDA_SSEL0/SD0_CLK/SCT0_OUT1/FC4_SSEL2/ADC0_4}
  - {pin_num: '90', peripheral: FLEXCOMM7, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_19/FC4_RTS_SCL_SSEL1/UTICK_CAP0/CTIMER0_MAT2/SCT0_OUT2/FC7_TXD_SCL_MISO_WS/PLU_IN4/SECURE_GPIO0_19,
    mode: inactive}
  - {pin_num: '74', peripheral: FLEXCOMM7, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_20/FC3_CTS_SDA_SSEL0/CTIMER1_MAT1/CT_INP15/SCT_GPI2/FC7_RXD_SDA_MOSI_DATA/HS_SPI_SSEL0/PLU_IN5/SECURE_GPIO0_20/FC4_TXD_SCL_MISO_WS,
    mode: inactive}
  - {pin_num: '30', peripheral: FLEXCOMM4, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO1_21/FC7_CTS_SDA_SSEL0/CTIMER3_MAT2/FC4_RXD_SDA_MOSI_DATA/PLU_OUT3}
  - {pin_num: '4', peripheral: FLEXCOMM4, signal: TXD_SCL_MISO_WS, pin_signal: PIO1_20/FC7_RTS_SCL_SSEL1/CT_INP14/FC4_TXD_SCL_MISO_WS/PLU_OUT2}
  - {pin_num: '2', peripheral: FLEXCOMM6, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO1_13/FC6_RXD_SDA_MOSI_DATA/CT_INP6/USB0_OVERCURRENTN/USB0_FRAME/SD0_CARD_DET_N}
  - {pin_num: '87', peripheral: FLEXCOMM6, signal: TXD_SCL_MISO_WS, pin_signal: PIO1_16/FC6_TXD_SCL_MISO_WS/CTIMER1_MAT3/SD0_CMD}
  - {pin_num: '88', peripheral: SECGPIO, signal: 'SECPIO0, 5', pin_signal: PIO0_5/FC4_RXD_SDA_MOSI_DATA/CTIMER3_MAT0/SCT_GPI5/FC3_RTS_SCL_SSEL1/MCLK/SECURE_GPIO0_5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 (Core #0) */
void BOARD_InitPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* Enables the clock for the SECGPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio_Sec);

    gpio_pin_config_t nb_rst_N_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize SECGPIO functionality on pin PIO0_1 (pin 7)  */
    GPIO_PinInit(BOARD_INITPINS_nb_rst_N_GPIO, BOARD_INITPINS_nb_rst_N_PORT, BOARD_INITPINS_nb_rst_N_PIN, &nb_rst_N_config);

    IOCON->PIO[0][1] = ((IOCON->PIO[0][1] &
                         /* Mask bits to zero which are setting */
                         (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                        /* Selects pin function.
                         * : PORT01 (pin 7) is configured as SECURE_GPIO0_1. */
                        | IOCON_PIO_FUNC(0x0Au)

                        /* Select Digital mode.
                         * : Enable Digital mode.
                         * Digital input is enabled. */
                        | IOCON_PIO_DIGIMODE(PIO0_1_DIGIMODE_DIGITAL));

    IOCON->PIO[0][19] = ((IOCON->PIO[0][19] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT019 (pin 90) is configured as FC7_TXD_SCL_MISO_WS. */
                         | IOCON_PIO_FUNC(PIO0_19_FUNC_ALT7)

                         /* Selects function mode (on-chip pull-up/pull-down resistor control).
                          * : Inactive.
                          * Inactive (no pull-down/pull-up resistor enabled). */
                         | IOCON_PIO_MODE(PIO0_19_MODE_INACTIVE)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO0_19_DIGIMODE_DIGITAL));

    IOCON->PIO[0][20] = ((IOCON->PIO[0][20] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT020 (pin 74) is configured as FC7_RXD_SDA_MOSI_DATA. */
                         | IOCON_PIO_FUNC(PIO0_20_FUNC_ALT7)

                         /* Selects function mode (on-chip pull-up/pull-down resistor control).
                          * : Inactive.
                          * Inactive (no pull-down/pull-up resistor enabled). */
                         | IOCON_PIO_MODE(PIO0_20_MODE_INACTIVE)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO0_20_DIGIMODE_DIGITAL));

    IOCON->PIO[0][23] = ((IOCON->PIO[0][23] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_DIGIMODE_MASK | IOCON_PIO_ASW_MASK)))

                         /* Selects pin function.
                          * : PORT023 (pin 20) is configured as ADC0_0. */
                         | IOCON_PIO_FUNC(PIO0_23_FUNC_ALT0)

                         /* Selects function mode (on-chip pull-up/pull-down resistor control).
                          * : Inactive.
                          * Inactive (no pull-down/pull-up resistor enabled). */
                         | IOCON_PIO_MODE(PIO0_23_MODE_INACTIVE)

                         /* Select Digital mode.
                          * : Disable digital mode.
                          * Digital input set to 0. */
                         | IOCON_PIO_DIGIMODE(PIO0_23_DIGIMODE_ANALOG)

                         /* Analog switch input control.
                          * : For all pins except PIO0_9, PIO0_11, PIO0_12, PIO0_15, PIO0_18, PIO0_31, PIO1_0 and
                          * PIO1_9 analog switch is closed (enabled). */
                         | IOCON_PIO_ASW(PIO0_23_ASW_VALUE1));

    IOCON->PIO[0][26] = ((IOCON->PIO[0][26] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT026 (pin 60) is configured as FC2_RXD_SDA_MOSI_DATA. */
                         | IOCON_PIO_FUNC(PIO0_26_FUNC_ALT1)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO0_26_DIGIMODE_DIGITAL));

    IOCON->PIO[0][27] = ((IOCON->PIO[0][27] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT027 (pin 27) is configured as FC2_TXD_SCL_MISO_WS. */
                         | IOCON_PIO_FUNC(PIO0_27_FUNC_ALT1)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO0_27_DIGIMODE_DIGITAL));

    const uint32_t port0_pin29_config = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
                                         IOCON_PIO_FUNC1 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN29 (coords: 92) is configured as FC0_RXD_SDA_MOSI_DATA */
    IOCON_PinMuxSet(IOCON, 0U, 29U, port0_pin29_config);

    const uint32_t port0_pin30_config = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
                                         IOCON_PIO_FUNC1 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN30 (coords: 94) is configured as FC0_TXD_SCL_MISO_WS */
    IOCON_PinMuxSet(IOCON, 0U, 30U, port0_pin30_config);

    IOCON->PIO[0][5] = ((IOCON->PIO[0][5] &
                         /* Mask bits to zero which are setting */
                         (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                        /* Selects pin function.
                         * : PORT05 (pin 88) is configured as SECURE_GPIO0_5. */
                        | IOCON_PIO_FUNC(0x0Au)

                        /* Select Digital mode.
                         * : Enable Digital mode.
                         * Digital input is enabled. */
                        | IOCON_PIO_DIGIMODE(PIO0_5_DIGIMODE_DIGITAL));

    IOCON->PIO[1][10] = ((IOCON->PIO[1][10] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT110 (pin 40) is configured as FC1_RXD_SDA_MOSI_DATA. */
                         | IOCON_PIO_FUNC(PIO1_10_FUNC_ALT2)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO1_10_DIGIMODE_DIGITAL));

    IOCON->PIO[1][11] = ((IOCON->PIO[1][11] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT111 (pin 93) is configured as FC1_TXD_SCL_MISO_WS. */
                         | IOCON_PIO_FUNC(PIO1_11_FUNC_ALT2)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO1_11_DIGIMODE_DIGITAL));

    IOCON->PIO[1][13] = ((IOCON->PIO[1][13] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT113 (pin 2) is configured as FC6_RXD_SDA_MOSI_DATA. */
                         | IOCON_PIO_FUNC(PIO1_13_FUNC_ALT2)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO1_13_DIGIMODE_DIGITAL));

    IOCON->PIO[1][16] = ((IOCON->PIO[1][16] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT116 (pin 87) is configured as FC6_TXD_SCL_MISO_WS. */
                         | IOCON_PIO_FUNC(PIO1_16_FUNC_ALT2)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO1_16_DIGIMODE_DIGITAL));

    IOCON->PIO[1][20] = ((IOCON->PIO[1][20] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT120 (pin 4) is configured as FC4_TXD_SCL_MISO_WS. */
                         | IOCON_PIO_FUNC(PIO1_20_FUNC_ALT5)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO1_20_DIGIMODE_DIGITAL));

    IOCON->PIO[1][21] = ((IOCON->PIO[1][21] &
                          /* Mask bits to zero which are setting */
                          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))

                         /* Selects pin function.
                          * : PORT121 (pin 30) is configured as FC4_RXD_SDA_MOSI_DATA. */
                         | IOCON_PIO_FUNC(PIO1_21_FUNC_ALT5)

                         /* Select Digital mode.
                          * : Enable Digital mode.
                          * Digital input is enabled. */
                         | IOCON_PIO_DIGIMODE(PIO1_21_DIGIMODE_DIGITAL));

    IOCON->PIO[1][8] = ((IOCON->PIO[1][8] &
                         /* Mask bits to zero which are setting */
                         (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_DIGIMODE_MASK | IOCON_PIO_ASW_MASK)))

                        /* Selects pin function.
                         * : PORT18 (pin 24) is configured as ADC0_4. */
                        | IOCON_PIO_FUNC(PIO1_8_FUNC_ALT0)

                        /* Selects function mode (on-chip pull-up/pull-down resistor control).
                         * : Inactive.
                         * Inactive (no pull-down/pull-up resistor enabled). */
                        | IOCON_PIO_MODE(PIO1_8_MODE_INACTIVE)

                        /* Select Digital mode.
                         * : Disable digital mode.
                         * Digital input set to 0. */
                        | IOCON_PIO_DIGIMODE(PIO1_8_DIGIMODE_ANALOG)

                        /* Analog switch input control.
                         * : For all pins except PIO0_9, PIO0_11, PIO0_12, PIO0_15, PIO0_18, PIO0_31, PIO1_0 and
                         * PIO1_9 analog switch is closed (enabled). */
                        | IOCON_PIO_ASW(PIO1_8_ASW_VALUE1));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
