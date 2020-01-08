/*
 * stx_etx.c
 *
 *  Created on: 8 de jan de 2020
 *      Author: evandro
 */


#include "stx_etx.h"

/**
 *
 */
uint8_t stx_etx_calculate_checksum(msg_t data);

/*
 *
 */
void stx_etx_send(msg_t dt)
{
	uint8_t buf[16] = {0};
	uint8_t i = 0, ii = 0;

	buf[i++] = VALUE_SYN;
	buf[i++] = VALUE_SYN;
	buf[i++] = VALUE_STX;
	buf[i++] = dt.byte_number;
	buf[i++] = dt.command;
	for(ii=0;ii<dt.byte_number;ii++)
	{
		buf[i++] = dt.data[ii];
	}
	buf[i++] = stx_etx_calculate_checksum(dt);
	buf[i++] = VALUE_ETX;

	serial_write(buf,i);
}

/**
 * @brief
 * @note: http://blog.datek.com.br/2019/10/ccomo-calcular-checksum/
 */
uint8_t stx_etx_calculate_checksum(msg_t data)
{
	uint8_t checksum = 0;
	uint8_t i = 0;

	checksum ^= data.byte_number;
	checksum ^= data.command;
	for(i=0;i<data.byte_number;i++)
	{
		checksum ^= data.data[i];
	}

	return (0xFF - checksum);
}
