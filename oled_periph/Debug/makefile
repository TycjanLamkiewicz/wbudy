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
all: oled_periph.axf

# Tool invocations
oled_periph.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\247680\Documents\wbudy123\Lib_CMSISv1p30_LPC13xx\Debug" -L"C:\Users\247680\Documents\wbudy123\Lib_MCU\Debug" -L"C:\Users\247680\Documents\wbudy123\Lib_EaBaseBoard\Debug" -Xlinker --gc-sections -Xlinker -Map=oled_periph.map -mcpu=cortex-m3 -mthumb -T "lpcxpresso1343_blinky_Debug.ld" -o "oled_periph.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) oled_periph.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size oled_periph.axf; # arm-none-eabi-objcopy -O ihex oled_periph.axf oled_periph.hex ;
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
