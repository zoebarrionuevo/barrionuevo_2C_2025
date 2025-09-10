/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * Este ejercicio permite mostrar numeros en un display utilizando pines GPIO.
 * Convierte un número en sus dígitos individuales y los grafica en el display,
 * activando y desactivando los pines correspondientes.
 * 
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	Dígito 1	| 	GPIO_20	    |
 * | 	Dígito 2	| 	GPIO_21	    |
 * | 	Dígito 3	| 	GPIO_22	    |
 * | 	Dígito 4	| 	GPIO_23	    |
 * | 	Select 1	| 	GPIO_19	    |
 * | 	Select 2	| 	GPIO_18	    |
 * | 	Select 3	| 	GPIO_9	    |
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 03/09/2025 | Document creation		                         |
 * | 10/09/2025 | Se agrega documentación		                 |
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
/**
 * @brief Convierte un número decimal en un arreglo de dígitos BCD.
 *
 * @param data Número a convertir.
 * @param digits Cantidad de dígitos a extraer.
 * @param bcd_number Arreglo donde se guardan los dígitos BCD.
 */
void  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for ( int i = digits - 1; i >= 0; i-- )
	{
		bcd_number[i] = data % 10;
		data = data / 10;
	}


}

/**
 * @brief Recibe un dígito BCD y un vector de estructuras gpioConf_t para mostrarlo en los pines GPIO.
 *
 * Enciende o apaga los pines de salida según el valor del dígito BCD recibido.
 *
 * @param digitoBCD Dígito en formato BCD a mostrar.
 * @param GPIOdigitos Vector de estructuras gpioConf_t que representa los pines GPIO.
 *
 * Requiere incluir el archivo de cabecera gpio_mcu.h
 */
void setGPIO (uint8_t digitoBCD, gpioConf_t *GPIOdigitos)
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

/**
 * @brief Grafica un número en un display multiplexado usando GPIO.
 *
 * Convierte el número en dígitos individuales y los muestra en el display,
 * activando y desactivando los pines correspondientes.
 *
 * @param dato Número a graficar.
 * @param cant_digitos_salida Cantidad de dígitos a mostrar.
 * @param GPIOdigitos Arreglo de pines para los bits de cada dígito.
 * @param GPIOmapa Arreglo de pines para seleccionar cada dígito del display.
 */
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