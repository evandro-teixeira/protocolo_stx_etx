/*
 * serial.h
 *
 *  Created on: 8 de jan de 2020
 *      Author: evandro
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#define OK	 (int)( 0 )
#define FAIL (int)( -1)


int serial_init (char *portname, int speed, int parity);
int serial_write(uint8_t *buffer, uint8_t size);
int serial_read(uint8_t *buffer);

#endif /* SERIAL_H_ */
