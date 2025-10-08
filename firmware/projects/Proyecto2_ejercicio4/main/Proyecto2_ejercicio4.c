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
 * | 08/10/2025 | Document creation		                         |
 *
 * @author Zoe Barrionuevo zoe.nicole.barrionuevo@gmail.com
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "analog_io_mcu.h"
#include "uart_mcu.h"
#include "timer_mcu.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_PERIOD 2000 // Período de lectura en microsegundos
/*==================[internal data definition]===============================*/
TaskHandle_t Input_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
void FuncTimerRead(void *param)
{
	vTaskNotifyGiveFromISR(Input_task_handle, pdFALSE); /* Envía una notificación a la tarea asociada */
}

static void InputReadSingle(void *pvParameter)
{
	uint16_t valorLectura = 0;
	while (true)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		AnalogInputReadSingle(CH1, &valorLectura);
		UartSendString(UART_PC, ">medicion:");
		UartSendString(UART_PC, (char *)UartItoa(valorLectura, 10));
		UartSendString(UART_PC, "\r\n");
	}
}
/*==================[external functions definition]==========================*/
void app_main(void){
	analog_input_config_t config={

	.input = CH1,
	.mode = ADC_SINGLE,
	.func_p = NULL,
	.param_p = NULL,
	.sample_frec = 0
	};

	AnalogInputInit(&config);

	serial_config_t my_uart = {

	.port = UART_PC,
	.baud_rate = 115200,
	.func_p = NULL,
	.param_p = NULL
	};

	UartInit(&my_uart);

	timer_config_t Input_Read = {
	.timer = TIMER_A,
	.period = CONFIG_PERIOD,
	.func_p = FuncTimerRead,
	.param_p = NULL
	};

	TimerInit(&Input_Read);

	xTaskCreate(&InputReadSingle, "InputReadSingle", 1024, NULL, 1, &Input_task_handle);
	TimerStart(Input_Read.timer);
}
/*==================[end of file]============================================*/