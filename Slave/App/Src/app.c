/*
 * app.c
 *
 *  Created on: 03/01/2020
 *      Author: evandro
 */

#include "../Inc/app.h"

/**
 *
 */
void app_task(void *pvParameters);
void app_led_green(bool st);
void app_led_blue(bool st);
bool app_status_button_get(void);

/**
 *
 */
static bool status_button = false;
extern QueueHandle_t queue_msg_in;
/**
 *
 */
void app_init(void)
{
	/* Creat Task App */
	if(xTaskCreate(app_task,"App",configMINIMAL_STACK_SIZE,NULL,(configMAX_PRIORITIES-4),NULL) != pdPASS)
	{
		/* Fail in creat task */
	}
}

/**
 *
 */
void app_task(void *pvParameters)
{
	static data_msg_t data_in = {0};
	static data_msg_t data_out = {0};

	app_led_green(0 /* Off */);
	app_led_blue(0 /* Off */);

	while(1)
	{
		if(stx_etx_queue_receive(&data_in,portMAX_DELAY) == true)
		//if(xQueueReceive(queue_msg_in,&data_in,(TickType_t)portMAX_DELAY) == pdTRUE)
		{
			/* State machine with the commands */
			switch(data_in.command)
			{
				case APP_LED_GREEN:
					/* Set LED Green */
					app_led_green((bool)data_in.data[0]);
					/* Prepare data to be transmitted */
					data_out.byte_number = 0;
					data_out.command = 1; // Status Ok
				break;
				case APP_LED_BLUE:
					/* Set LED Blue */
					app_led_blue((bool)data_in.data[0]);
					/* Prepare data to be transmitted */
					data_out.byte_number = 0;
					data_out.command = 1; // Status Ok
				break;
				case APP_STATUS_BUTTON:
					/* Get Status Button */
					data_out.byte_number = 1;
					data_out.command = 1; // Status Ok
					data_out.data[0] = (uint8_t)(app_status_button_get());
				break;
				case APP_NO_COMMAND:
				default:
					/* Signals failure */
					data_out.byte_number = 1;
					data_out.command = 0; 	// Status Faul
					data_out.data[0] = 255;	// Cod. Error
				break;
			}

			/* Send MSG */
			stx_etx_queue_send(data_out,portMAX_DELAY);
		}
	}
}

/**
 *
 */
void app_led_green(bool st)
{
	HAL_GPIO_WritePin(GPIOC,LD3_Pin,(GPIO_PinState)st);
}

/**
 *
 */
void app_led_blue(bool st)
{
	HAL_GPIO_WritePin(GPIOC,LD4_Pin,(GPIO_PinState)st);
}

/**
 *
 */
void app_status_button_set(bool data)
{
	status_button = data;
}

/**
 *
 */
bool app_status_button_get(void)
{
	status_button = HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port,USER_BUTTON_Pin);
	return status_button;
}
