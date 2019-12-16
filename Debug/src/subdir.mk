################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/connection.c \
../src/network.c \
../src/remote.c \
../src/salute.c \
../src/salute_ui.c \
../src/salute_view.c \
../src/tc_address.c 

OBJS += \
./src/connection.o \
./src/network.o \
./src/remote.o \
./src/salute.o \
./src/salute_ui.o \
./src/salute_view.o \
./src/tc_address.o 

C_DEPS += \
./src/connection.d \
./src/network.d \
./src/remote.d \
./src/salute.d \
./src/salute_ui.d \
./src/salute_view.d \
./src/tc_address.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -mwindows -IC:/msys64/mingw64/include/gtk-3.0 -I"C:\Users\HORACEK\MEGA\eclipse_workspace\aclib\src\include" -IC:/msys64/mingw64/include/cairo -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/pango-1.0 -IC:/msys64/mingw64/include/fribidi -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/atk-1.0 -IC:/msys64/mingw64/include/cairo -IC:/msys64/mingw64/include/pixman-1 -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/freetype2 -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/harfbuzz -IC:/msys64/mingw64/include/libpng16 -IC:/msys64/mingw64/include/gdk-pixbuf-2.0 -IC:/msys64/mingw64/include/libpng16 -IC:/msys64/mingw64/include -IC:/msys64/mingw64/lib/libffi-3.2.1/include -IC:/msys64/mingw64/include/glib-2.0 -IC:/msys64/mingw64/lib/glib-2.0/include -IC:/msys64/mingw64/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


