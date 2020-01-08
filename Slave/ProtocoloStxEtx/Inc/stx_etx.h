/*
 * stx_etx.h
 *
 *  Created on: 27/12/2019
 *      Author: evandro
 */

#ifndef PROTOCOLOSTXETX_INC_STX_ETX_H_
#define PROTOCOLOSTXETX_INC_STX_ETX_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "crc.h"
#include <stdbool.h>

#define NUMBER_OF_ITEMS		4
#define VALUE_SYN			(uint8_t)0x16
#define VALUE_STX			(uint8_t)0x02
#define VALUE_ETX			(uint8_t)0x03
#define TIMEOUT				(uint32_t)100000

typedef enum
{
	SYN1 = 0,
	SYN2,
	STX,
	DATA,
	CHK_CRC,
	ETX,
	MAX_STATE_INTERPRETER
}state_interpreter_t;

typedef enum
{
	IDLE = 0,
	PROCESSING,
	MSG_COMPLETED,
	FAULT_START_BYTE,
	FAULT_TIMEOUT,
	FAULT_INCORRECT_CRC,
	FAULT_STOP_BYTE,
	FAULT_NO_COMMAND,
	FAULT_INTERNAL_ERROR
}status_interpreter_t;

typedef enum
{
	BYTE_NUMBER = 0,
	COMMAND,
	DATA_ASSEMBLER,
	MAX_STATE_DATA
}state_data_t;

typedef struct
{
	uint8_t data;
	uint32_t time;
}data_rx_t;

typedef struct
{
	//uint8_t config;
	uint8_t byte_number;
	uint8_t command;
	uint8_t data[4];
}data_msg_t;

/*typedef struct
{
	uint8_t syn1;
	uint8_t syn2;
	uint8_t stx;
	data_msg_t data;
	uint8_t crc;
	uint8_t etx;
}msg_t;*/

void stx_etx_init(void);
void stx_etx_queue_get_data(uint8_t data);
//uint8_t stx_etx_calculate_checksum(data_msg_t data, data_msg_t *data_out);
bool stx_etx_queue_receive(data_msg_t *data_out,uint32_t tick);
bool stx_etx_queue_send(data_msg_t data,uint32_t tick);

#endif /* PROTOCOLOSTXETX_INC_STX_ETX_H_ */
