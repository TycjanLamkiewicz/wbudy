################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Weather_station.axf

# Tool invocations
Weather_station.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\247680\Documents\wbudy123\Lib_CMSISv1p30_LPC13xx\Debug" -L"C:\Users\247680\Documents\wbudy123\Lib_MCU\Debug" -L"C:\Users\247680\Documents\wbudy123\Lib_EaBaseBoard\Debug" -Xlinker --gc-sections -Xlinker -Map=Weather_station.map -mcpu=cortex-m3 -mthumb -T "Weather_station_Debug.ld" -o "Weather_station.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Weather_station.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size Weather_station.axf; # arm-none-eabi-objcopy -O ihex Weather_station.axf Weather_station.hex ;
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
