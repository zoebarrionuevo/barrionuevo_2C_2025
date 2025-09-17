/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * Este programa mide la distancia utilizando un sensor ultrasónico HC-SR04 y muestra la distancia medida en centímetros en un display LCD.
 * Además, enciende diferentes LEDs según la distancia medida y permite controlar la medición y la visualización mediante teclas.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |   HC_SR04      |   EDU-ESP	|
 * |:--------------:|:-------------:|
 * | 	Vcc 	    |	5V      	|
 * | 	Echo		| 	GPIO_3		|
 * | 	Trig	 	| 	GPIO_2		|
 * | 	Gnd 	    | 	GND     	|
 * 
 *  |   Display      |   EDU-ESP	|
 * |:--------------:|:-------------:|
 * | 	Vcc 	    |	5V      	|
 * | 	BCD1		| 	GPIO_20		|
 * | 	BCD2	 	| 	GPIO_21		|
 * | 	BCD3	 	| 	GPIO_22		|
 * | 	BCD4	 	| 	GPIO_23		|
 * | 	SEL1	 	| 	GPIO_19		|
 * | 	SEL2	 	| 	GPIO_18		|
 * | 	SEL3	 	| 	GPIO_9		|
 * | 	Gnd 	    | 	GND     	|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 10/09/2025 | Creación del documento                         |
 * | 17/09/2025 | Finalización del documento		             |
 * | 17/09/2025 | Se agrega documentación                        |
 *
 * @author Barrionuevo Zoe (zoe.nicole.barrionuevo@gmail.com)
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
#define CONFIG_MEDICION_2 100
/*==================[internal data definition]===============================*/
/**
 * @brief Almacena la distancia medida en centímetros.
 */
uint16_t distancia = 0;

/**
 * @brief Indica si la medición está activa (true) o detenida (false).
 */
bool activar_medicion = false;

/**
 * @brief Indica si el valor mostrado está en modo HOLD (true) o en actualización normal (false).
 */
bool hold = false;

TaskHandle_t medir_distancia_task_handle = NULL;
TaskHandle_t prendoled_task_handle = NULL;
TaskHandle_t mostrar_en_display_task_handle = NULL;
TaskHandle_t switchies_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
/**
 * @brief Tarea que mide la distancia usando el sensor ultrasónico HC-SR04.
 * Actualiza la variable global 'distancia' cada 1 segundo si la medición está activa.
 */
static void medir_distancia(void *pvParameter)
{
	while (true)
	{
		if (activar_medicion)
		{
			distancia = HcSr04ReadDistanceInCentimeters();
		}
		vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);
	}
}

/**
 * @brief Tarea que enciende los LEDs según la distancia medida.
 * LED_1: 10-20cm, LED_1+LED_2: 20-30cm, LED_1+LED_2+LED_3: >30cm, todos apagados si <10cm.
 */
static void prenderLED_segun_distancia(void *pvParameter)
{
	while (true)
	{

		if (activar_medicion)
		{
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
		}
		else
			LedsOffAll();
		vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);
	}
}

/**
 * @brief Tarea que muestra la distancia en el display LCD.
 * Si está activado el modo HOLD, mantiene el valor mostrado.
 */
static void mostrar_en_display(void *pvParameter)
{
	while (true)
	{
		if (activar_medicion)
		{
			if (!hold)
			{
				LcdItsE0803Write(distancia);
			}
		}
		else
			LcdItsE0803Off();
		vTaskDelay(CONFIG_MEDICION / portTICK_PERIOD_MS);
	}
}

/**
 * @brief Tarea que gestiona las teclas TEC1 y TEC2.
 * TEC1 activa/detiene la medición. TEC2 mantiene el resultado (HOLD).
 */
static void teclas(void *pvParameter)
{
	uint8_t teclas;
	while (true)
	{
		teclas = SwitchesRead();
		switch (teclas)
		{
		case SWITCH_1:
			activar_medicion = !activar_medicion;
			break;
		case SWITCH_2:
			hold = !hold;
			break;
		}
		vTaskDelay(CONFIG_MEDICION_2 / portTICK_PERIOD_MS);
	}
}
/*==================[external functions definition]==========================*/
/**
 * @brief Función principal. Inicializa hardware y crea las tareas del sistema.
 */
void app_main(void)
{
	LedsInit();
	LcdItsE0803Init();
	SwitchesInit();
	HcSr04Init(GPIO_3, GPIO_2); // Echo, Trigger
	xTaskCreate(&medir_distancia, "Mido distancia", 512, NULL, 5, &medir_distancia_task_handle);
	xTaskCreate(&prenderLED_segun_distancia, "Prendo led segun distancia", 512, NULL, 5, &prendoled_task_handle);
	xTaskCreate(&mostrar_en_display, "Muestro distancia en display", 512, NULL, 5, &mostrar_en_display_task_handle);
	xTaskCreate(&teclas, "Switchies", 512, NULL, 5, &switchies_task_handle);
}
/*==================[end of file]============================================*/