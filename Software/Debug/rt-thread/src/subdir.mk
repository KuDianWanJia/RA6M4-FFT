################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/src/clock.c \
../rt-thread/src/components.c \
../rt-thread/src/device.c \
../rt-thread/src/idle.c \
../rt-thread/src/ipc.c \
../rt-thread/src/irq.c \
../rt-thread/src/kservice.c \
../rt-thread/src/mem.c \
../rt-thread/src/object.c \
../rt-thread/src/scheduler.c \
../rt-thread/src/thread.c \
../rt-thread/src/timer.c 

OBJS += \
./rt-thread/src/clock.o \
./rt-thread/src/components.o \
./rt-thread/src/device.o \
./rt-thread/src/idle.o \
./rt-thread/src/ipc.o \
./rt-thread/src/irq.o \
./rt-thread/src/kservice.o \
./rt-thread/src/mem.o \
./rt-thread/src/object.o \
./rt-thread/src/scheduler.o \
./rt-thread/src/thread.o \
./rt-thread/src/timer.o 

C_DEPS += \
./rt-thread/src/clock.d \
./rt-thread/src/components.d \
./rt-thread/src/device.d \
./rt-thread/src/idle.d \
./rt-thread/src/ipc.d \
./rt-thread/src/irq.d \
./rt-thread/src/kservice.d \
./rt-thread/src/mem.d \
./rt-thread/src/object.d \
./rt-thread/src/scheduler.d \
./rt-thread/src/thread.d \
./rt-thread/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/src/%.o: ../rt-thread/src/%.c
	arm-none-eabi-gcc -I"C:\Users\QQ\Desktop\RA6M4-FFT" -I"C:\Users\QQ\Desktop\RA6M4-FFT\board\ports" -I"C:\Users\QQ\Desktop\RA6M4-FFT\board" -I"C:\Users\QQ\Desktop\RA6M4-FFT\libraries\HAL_Drivers\config" -I"C:\Users\QQ\Desktop\RA6M4-FFT\libraries\HAL_Drivers" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\fsp\inc\api" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\fsp\inc\instances" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra\fsp\inc" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra_cfg\fsp_cfg\bsp" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra_cfg\fsp_cfg" -I"C:\Users\QQ\Desktop\RA6M4-FFT\ra_gen" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\drivers\include" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\finsh" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\compilers\common" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\compilers\newlib" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\posix\io\poll" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\posix\io\stdio" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\components\libc\posix\ipc" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\include" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\libcpu\arm\common" -I"C:\Users\QQ\Desktop\RA6M4-FFT\rt-thread\libcpu\arm\cortex-m4" -I"C:\Users\QQ\Desktop\RA6M4-FFT\src\OLED" -I"C:\Users\QQ\Desktop\RA6M4-FFT\src\Hardware" -include"C:\Users\QQ\Desktop\RA6M4-FFT\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

