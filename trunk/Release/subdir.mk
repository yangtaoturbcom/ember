################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../chemistry.cpp \
../debugUtils.cpp \
../flameSolver.cpp \
../flameSys.cpp \
../grid.cpp \
../mathUtils.cpp \
../matlabFile.cpp \
../readConfig.cpp \
../strainedFlame.cpp \
../sundialsUtils.cpp 

OBJS += \
./chemistry.o \
./debugUtils.o \
./flameSolver.o \
./flameSys.o \
./grid.o \
./mathUtils.o \
./matlabFile.o \
./readConfig.o \
./strainedFlame.o \
./sundialsUtils.o 

CPP_DEPS += \
./chemistry.d \
./debugUtils.d \
./flameSolver.d \
./flameSys.d \
./grid.d \
./mathUtils.d \
./matlabFile.d \
./readConfig.d \
./strainedFlame.d \
./sundialsUtils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	icc -I/usr/local/cantera/include -I/usr/local/matlab/extern/include -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

