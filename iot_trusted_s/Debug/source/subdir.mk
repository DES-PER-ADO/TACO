################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/aes.c \
../source/app_GPS.c \
../source/app_adc.c \
../source/app_aes.c \
../source/app_camera.c \
../source/app_gpio.c \
../source/app_io.c \
../source/app_json.c \
../source/app_rtc.c \
../source/app_spi.c \
../source/app_uart.c \
../source/app_upload.c \
../source/main_s.c \
../source/rsa.c \
../source/secure_varibale.c \
../source/semihost_hardfault.c 

OBJS += \
./source/aes.o \
./source/app_GPS.o \
./source/app_adc.o \
./source/app_aes.o \
./source/app_camera.o \
./source/app_gpio.o \
./source/app_io.o \
./source/app_json.o \
./source/app_rtc.o \
./source/app_spi.o \
./source/app_uart.o \
./source/app_upload.o \
./source/main_s.o \
./source/rsa.o \
./source/secure_varibale.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/aes.d \
./source/app_GPS.d \
./source/app_adc.d \
./source/app_aes.d \
./source/app_camera.d \
./source/app_gpio.d \
./source/app_io.d \
./source/app_json.d \
./source/app_rtc.d \
./source/app_spi.d \
./source/app_uart.d \
./source/app_upload.d \
./source/main_s.d \
./source/rsa.d \
./source/secure_varibale.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__MULTICORE_NONE -DCPU_LPC55S69JBD100_cm33_core0 -DARM_MATH_CM33 -DCPU_LPC55S69JBD100 -DCPU_LPC55S69JBD100_cm33 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__MULTICORE_MASTER -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -D__NEWLIB__ -I../drivers -I../component/uart -I../component/serial_manager -I../device -I../CMSIS -I../utilities -I../component/lists -I"C:\Users\dgs\Documents\MCUXpressoIDE_11.0.0_2516\workspace\iot_trusted_s\app_include" -I../board -I../source -I../ -I../startup -I../trustzone -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mcpu=cortex-m33 -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -mcmse -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


