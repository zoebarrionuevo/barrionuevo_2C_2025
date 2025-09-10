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
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "hc_sr04.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"
#include "lcditse0803.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_MEDICION 1000
/*==================[internal data definition]===============================*/
uint16_t distancia = 0;
/*==================[internal functions declaration]=========================*/
static void medir_distancia (void *pvParameter){
    while(true){
		distancia = HcSr04ReadDistanceInCentimeters();
        vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);
    }
}

static void prenderLED_segun_distancia(void *pvParameter){
    while(true){
        if (distancia < 10)
		{
		LedsOffAll();
		}
		else if (distancia >= 10 && distancia < 20)
		{
			LedOn(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
		}
		else if (distancia >= 20 && distancia < 30)
		{
			LedOn(LED_2);
			LedOn(LED_1);
			LedOff(LED_3);
		}
		else if (distancia >= 30)
		{
			LedOn(LED_3);
			LedOn(LED_1);
			LedOn(LED_2);
		}
		vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);
    }
}

static void mostrar_en_display(void *pvParameter){
    while(true){
		LcdItsE0803Write(distancia);
		vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);
    }
}

static void teclas(void *pvParameter){
	uint8_t teclas;
	while (true){
    	teclas  = SwitchesRead();
    	switch(teclas){
    		case SWITCH_1:
    			LedToggle(LED_1);
    		break;
    		case SWITCH_2:
    			LedToggle(LED_2);
    		break;
    	}
		vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);

	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	printf("Hello world!\n");
}
/*==================[end of file]============================================*/