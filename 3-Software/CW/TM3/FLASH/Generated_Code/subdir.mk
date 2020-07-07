################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/CI2C1.c" \
"../Generated_Code/ClockPin1.c" \
"../Generated_Code/Cpu.c" \
"../Generated_Code/DataPin1.c" \
"../Generated_Code/EI2C1.c" \
"../Generated_Code/IO_Map.c" \
"../Generated_Code/Vectors.c" \

C_SRCS += \
../Generated_Code/CI2C1.c \
../Generated_Code/ClockPin1.c \
../Generated_Code/Cpu.c \
../Generated_Code/DataPin1.c \
../Generated_Code/EI2C1.c \
../Generated_Code/IO_Map.c \
../Generated_Code/Vectors.c \

OBJS += \
./Generated_Code/CI2C1_c.obj \
./Generated_Code/ClockPin1_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/DataPin1_c.obj \
./Generated_Code/EI2C1_c.obj \
./Generated_Code/IO_Map_c.obj \
./Generated_Code/Vectors_c.obj \

OBJS_QUOTED += \
"./Generated_Code/CI2C1_c.obj" \
"./Generated_Code/ClockPin1_c.obj" \
"./Generated_Code/Cpu_c.obj" \
"./Generated_Code/DataPin1_c.obj" \
"./Generated_Code/EI2C1_c.obj" \
"./Generated_Code/IO_Map_c.obj" \
"./Generated_Code/Vectors_c.obj" \

C_DEPS += \
./Generated_Code/CI2C1_c.d \
./Generated_Code/ClockPin1_c.d \
./Generated_Code/Cpu_c.d \
./Generated_Code/DataPin1_c.d \
./Generated_Code/EI2C1_c.d \
./Generated_Code/IO_Map_c.d \
./Generated_Code/Vectors_c.d \

C_DEPS_QUOTED += \
"./Generated_Code/CI2C1_c.d" \
"./Generated_Code/ClockPin1_c.d" \
"./Generated_Code/Cpu_c.d" \
"./Generated_Code/DataPin1_c.d" \
"./Generated_Code/EI2C1_c.d" \
"./Generated_Code/IO_Map_c.d" \
"./Generated_Code/Vectors_c.d" \

OBJS_OS_FORMAT += \
./Generated_Code/CI2C1_c.obj \
./Generated_Code/ClockPin1_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/DataPin1_c.obj \
./Generated_Code/EI2C1_c.obj \
./Generated_Code/IO_Map_c.obj \
./Generated_Code/Vectors_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/CI2C1_c.obj: ../Generated_Code/CI2C1.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/CI2C1.args" -o "Generated_Code/CI2C1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/%.d: ../Generated_Code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Generated_Code/ClockPin1_c.obj: ../Generated_Code/ClockPin1.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/ClockPin1.args" -o "Generated_Code/ClockPin1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Cpu_c.obj: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/Cpu.args" -o "Generated_Code/Cpu_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/DataPin1_c.obj: ../Generated_Code/DataPin1.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/DataPin1.args" -o "Generated_Code/DataPin1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/EI2C1_c.obj: ../Generated_Code/EI2C1.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/EI2C1.args" -o "Generated_Code/EI2C1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/IO_Map_c.obj: ../Generated_Code/IO_Map.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/IO_Map.args" -o "Generated_Code/IO_Map_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vectors_c.obj: ../Generated_Code/Vectors.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: S12Z Compiler'
	"$(S12Z_ToolsDirEnv)/mwccs12lisa" -c @@"Generated_Code/Vectors.args" -o "Generated_Code/Vectors_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


