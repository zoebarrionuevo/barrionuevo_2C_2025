/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 20/08/2025 | Document creation		                         |
 *
 * @author Barrionuevo Zoe (zoe.nicole.barrionuevo@gmail.com)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 100
#define ON 1
#define OFF 0
#define TOGGLE 2
/*==================[internal data definition]===============================*/
struct leds
{
	uint8_t mode;	  //    ON, OFF, TOGGLE
	uint8_t n_led;	  //  indica el número de led a controlar
	uint8_t n_ciclos; // indica la cantidad de ciclos de encendido/apagado
	uint16_t periodo; // indica el tiempo de cada ciclo
} my_leds;
/*==================[internal functions declaration]=========================*/
void funcion(struct leds *my_leds)
{
	if (my_leds->mode == ON)
	{
		if (my_leds->n_led == LED_1)
		{
			LedOn(LED_1);
		}
		else if (my_leds->n_led == LED_2)
		{
			LedOn(LED_2);
		}
		else if (my_leds->n_led == LED_3)
		{
			LedOn(LED_3);
		}
	}

	if (my_leds->mode == OFF)
	{
		if (my_leds->n_led == LED_1)
		{
			LedOff(LED_1);
		}
		else if (my_leds->n_led == LED_2)
		{
			LedOff(LED_2);
		}
		else if (my_leds->n_led == LED_3)
		{
			LedOff(LED_3);
		}
	}

	if (my_leds->mode == TOGGLE)
	{
		for (int i = 0; i < my_leds->n_ciclos; i++)
		{
		if (my_leds->n_led == LED_1)
		{
			LedToggle(LED_1);
		}
		else if (my_leds->n_led == LED_2)
		{
			LedToggle(LED_2);
		}
		else if (my_leds->n_led == LED_3)
		{
			LedToggle(LED_3);
		}

			for (int j = 0; j < my_leds->periodo / CONFIG_BLINK_PERIOD; j++)
			{
				vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			}
		}
	}
}
	/*==================[external functions definition]==========================*/
	void app_main(void)
	{
		LedsInit();

		my_leds.mode = TOGGLE;
		my_leds.n_ciclos = 10;
		my_leds.n_led = LED_1;
		my_leds.periodo = 500;

		funcion(&my_leds);
	}
	/*==================[end of file]============================================*/