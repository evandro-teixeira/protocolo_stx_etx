/*
 * app.h
 *
 *  Created on: 03/01/2020
 *      Author: evandro
 */

#ifndef APP_INC_APP_H_
#define APP_INC_APP_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "../Inc/stx_etx.h"
#include <stdbool.h>

typedef enum
{
	APP_LED_GREEN = 1,
	APP_LED_BLUE,
	APP_STATUS_BUTTON,
	APP_NO_COMMAND
}app_command_t;

void app_init(void);
void app_status_button_set(bool data);

#endif /* APP_INC_APP_H_ */
