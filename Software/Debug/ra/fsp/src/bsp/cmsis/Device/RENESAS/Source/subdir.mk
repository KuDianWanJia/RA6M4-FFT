################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	arm-none-eabi-gcc -I"C:\Users\QQ\Desktop\RA6M4-FFT" -I"C:\Users\QQ\Desktop\RA6M4-FFT\board\ports" -I"C:\Users\QQ\Desktop\RA6M4-FFT\board" -I"C:\Users\QQ\Desktop\RA6M4-FFT\libraries\HAL_Drivers\config" -I"C:\Users\QQ\Desktop\RA6M4-FFT\libraries\HAL_Drivers" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\fsp\inc\api" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\fsp\inc\instances" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\fsp\inc" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra_cfg\fsp_cfg\bsp" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra_cfg\fsp_cfg" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra_gen" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\drivers\include" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\finsh" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\compilers\common" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\compilers\newlib" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\posix\io\poll" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\posix\io\stdio" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\posix\ipc" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\include" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\libcpu\arm\common" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\libcpu\arm\cortex-m4" -I"C:\Users\QQ\Desktop\RA6M4-FFT\src\OLED" -I"C:\Users\QQ\Desktop\RA6M4-FFT\src\Hardware" -include"C:\Users\QQ\Desktop\RA6M4-FFT\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

