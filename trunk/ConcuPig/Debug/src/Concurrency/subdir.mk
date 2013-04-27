################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Concurrency/Fifo.cpp \
../src/Concurrency/SignalHandler.cpp 

OBJS += \
./src/Concurrency/Fifo.o \
./src/Concurrency/SignalHandler.o 

CPP_DEPS += \
./src/Concurrency/Fifo.d \
./src/Concurrency/SignalHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/Concurrency/%.o: ../src/Concurrency/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


