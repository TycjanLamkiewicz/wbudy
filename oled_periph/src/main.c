/*****************************************************************************
 *   Peripherals such as temp sensor, light sensor, accelerometer,
 *   and trim potentiometer are monitored and values are written to
 *   the OLED display.
 *
 *   Copyright(C) 2009, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/


#include "mcu_regs.h"
#include "type.h"
#include "uart.h"
#include "stdio.h"
#include "timer32.h"
#include "i2c.h"
#include "gpio.h"
#include "ssp.h"
#include "adc.h"
#include <stdbool.h>

#include "light.h"
#include "oled.h"
#include "temp.h"
#include "acc.h"
#include "joystick.h"

#define P1_2_HIGH() (LPC_GPIO1->DATA |= (0x1<<2))
#define P1_2_LOW()  (LPC_GPIO1->DATA &= ~(0x1<<2))

static uint32_t msTicks = 0;
static uint8_t buf[10];

static void intToString(int value, uint8_t* pBuf, uint32_t len, uint32_t base)
{
    static const char* pAscii = "0123456789abcdefghijklmnopqrstuvwxyz";
    int pos = 0;
    int tmpValue = value;

    // the buffer must not be null and at least have a length of 2 to handle one
    // digit and null-terminator
    if (pBuf == NULL || len < 2)
    {
        return;
    }

    // a valid base cannot be less than 2 or larger than 36
    // a base value of 2 means binary representation. A value of 1 would mean only zeros
    // a base larger than 36 can only be used if a larger alphabet were used.
    if (base < 2 || base > 36)
    {
        return;
    }

    // negative value
    if (value < 0)
    {
        tmpValue = -tmpValue;
        value    = -value;
        pBuf[pos++] = '-';
    }

    // calculate the required length of the buffer
    do {
        pos++;
        tmpValue /= base;
    } while(tmpValue > 0);


    if (pos > len)
    {
        // the len parameter is invalid.
        return;
    }

    pBuf[pos] = '\0';

    do {
        pBuf[--pos] = pAscii[value % base];
        value /= base;
    } while(value > 0);

    return;

}

void SysTick_Handler(void) {
    msTicks++;
}

static uint32_t getTicks(void)
{
    return msTicks;
}


int main (void)
{
    int8_t x = 0;
    int8_t y = 0;
    int8_t z = 0;

    int32_t t = 0;
    uint32_t lux = 0;
    uint32_t trim = 0;

    uint8_t joy = 0;

    GPIOInit();
    init_timer32(0, 10);

    UARTInit(115200);
    UARTSendString((uint8_t*)"OLED - Peripherals\r\n");

    I2CInit( (uint32_t)I2CMASTER, 0 );
    SSPInit();
    ADCInit( ADC_CLK );

    oled_init();
    light_init();
    joystick_init();

    temp_init (&getTicks);


    /* setup sys Tick. Elapsed time is e.g. needed by temperature sensor */
    SysTick_Config(SystemCoreClock / 1000);
    if ( !(SysTick->CTRL & (1<<SysTick_CTRL_CLKSOURCE_Msk)) )
    {
      /* When external reference clock is used(CLKSOURCE in
      Systick Control and register bit 2 is set to 0), the
      SYSTICKCLKDIV must be a non-zero value and 2.5 times
      faster than the reference clock.
      When core clock, or system AHB clock, is used(CLKSOURCE
      in Systick Control and register bit 2 is set to 1), the
      SYSTICKCLKDIV has no effect to the SYSTICK frequency. See
      more on Systick clock and status register in Cortex-M3
      technical Reference Manual. */
      LPC_SYSCON->SYSTICKCLKDIV = 0x08;
    }

    light_enable();
    light_setRange(LIGHT_RANGE_4000);

    oled_clearScreen(OLED_COLOR_WHITE);

    oled_putString(1,9,  (uint8_t*)"Light  : ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    oled_putString(1,17, (uint8_t*)"Trimpot: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);


    bool temp_type = false;

    while(1) {

        /* Temperature */
    	if (temp_type == false) {
    		oled_putString(1,1,  (uint8_t*)"Temp C : ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    		t = temp_read()/10;

    	}
    	else {
    		oled_putString(1,1,  (uint8_t*)"Temp F : ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
    		t = ((temp_read() * 1.8) + 32)/10;
    	}


        /* light */
        lux = light_read();

        /* trimpot */
        trim = ADCRead(0);

        /* output values to OLED display */

        intToString(t, buf, 10, 10);
        oled_fillRect((1+9*6),1, 80, 8, OLED_COLOR_WHITE);
        oled_putString((1+9*6),1, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);

        intToString(lux, buf, 10, 10);
        oled_fillRect((1+9*6),9, 80, 16, OLED_COLOR_WHITE);
        oled_putString((1+9*6),9, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);

        intToString(trim, buf, 10, 10);
        //oled_fillRect((1+9*6),17, 80, 24, OLED_COLOR_WHITE);
        //oled_putString((1+9*6),17, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        if(trim < 700) {
        	oled_fillRect((1+9*6),17, 80, 24, OLED_COLOR_WHITE);
        	oled_putString((1+9*6),17, "malo", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }
        else {
        	oled_fillRect((1+9*6),17, 80, 24, OLED_COLOR_WHITE);
        	oled_putString((1+9*6),17, "duzo", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
        }

        joy = joystick_read();

                if ((joy & JOYSTICK_CENTER) != 0) {
                    continue;
                }

                if ((joy & JOYSTICK_DOWN) != 0) {

                	oled_putString(1,55, (uint8_t*)"dol", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
                }

                if ((joy & JOYSTICK_UP) != 0) {
                	oled_putString(1,55, (uint8_t*)"gor", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
                }

                if ((joy & JOYSTICK_LEFT) != 0) {
                	temp_type = false;
                }

                if ((joy & JOYSTICK_RIGHT) != 0) {
                	temp_type = true;
                }

        /* delay */
        delay32Ms(0, 200);
    }

}
