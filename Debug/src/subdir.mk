################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlinkerDetector.cpp \
../src/MuHi.cpp \
../src/WebcamWorker.cpp \
../src/Writer.cpp 

OBJS += \
./src/BlinkerDetector.o \
./src/MuHi.o \
./src/WebcamWorker.o \
./src/Writer.o 

CPP_DEPS += \
./src/BlinkerDetector.d \
./src/MuHi.d \
./src/WebcamWorker.d \
./src/Writer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


