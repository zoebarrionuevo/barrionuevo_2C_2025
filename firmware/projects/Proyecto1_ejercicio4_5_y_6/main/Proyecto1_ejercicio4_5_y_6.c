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
 * | 03/09/2025 | Document creation		                         |
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
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;
/*==================[internal functions declaration]=========================*/
void  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for ( int i = digits - 1; i >= 0; i-- )
	{
		bcd_number[i] = data % 10;
		data = data / 10;
	}


}

void setGPIO (uint8_t numero, gpioConf_t *GPIOdigitos)
{
	for (int i = 0; i<4; i++)
	{
		if ((1 << i) & numero)
		{
			GPIOOn(GPIOdigitos[i].pin);
		}
		else
		{
			GPIOOff(GPIOdigitos[i].pin);
		}
	}
}

void graficarNumero(uint32_t dato, uint8_t cant_digitos_salida, gpioConf_t *GPIOdigitos, gpioConf_t *GPIOmapa)
{
	uint8_t digitosSeparados[3];
	convertToBcdArray(dato, cant_digitos_salida, digitosSeparados);
	for (int i = 0; i < 3; i++)
	{
		setGPIO(digitosSeparados[i], GPIOdigitos);
		GPIOOn(GPIOmapa[i].pin);
		GPIOOff(GPIOmapa[i].pin);
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	// uint8_t bcd_array[3];
	 uint32_t data = 137;
	 uint8_t digits = 3;
	// convertToBcdArray(data, digits, bcd_array);

	// for (int i=0; i<digits; i++)
	// printf("%d", bcd_array[i]);


	 	gpioConf_t GPIOdigitos[] =
	 	{
	 		{GPIO_20, GPIO_OUTPUT},
	 		{GPIO_21, GPIO_OUTPUT},
	 		{GPIO_22, GPIO_OUTPUT},
	 		{GPIO_23, GPIO_OUTPUT}
	 	};

	 for (int i = 0; i < 4; i++)
	 {
	 	GPIOInit(GPIOdigitos[i].pin, GPIOdigitos[i].dir);
	 }

	// setGPIO(5, GPIOdigitos);


		gpioConf_t GPIOmapa[] =
		{
			{GPIO_19, GPIO_OUTPUT},
			{GPIO_18, GPIO_OUTPUT},
			{GPIO_9, GPIO_OUTPUT}
		};

	for (int i = 0; i < 4; i++)
	{
		GPIOInit(GPIOmapa[i].pin, GPIOmapa[i].dir);
	}
	graficarNumero(data, digits, GPIOdigitos, GPIOmapa);

}
/*==================[end of file]============================================*/