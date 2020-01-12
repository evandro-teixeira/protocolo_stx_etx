/*
 * stx_etx.h
 *
 *  Created on: 8 de jan de 2020
 *      Author: evandro
 */

#ifndef STX_ETX_H_
#define STX_ETX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "serial.h"

#define NUMBER_OF_ITEMS		4
#define VALUE_SYN			(uint8_t)0x16
#define VALUE_STX			(uint8_t)0x02
#define VALUE_ETX			(uint8_t)0x03

typedef struct
{
	uint8_t byte_number;
	uint8_t command;
	uint8_t data[NUMBER_OF_ITEMS];
}msg_t;

void stx_etx_send(msg_t dt);
uint8_t stx_etx_calculate_checksum(msg_t data);

#endif /* STX_ETX_H_ */
