/*
 * serial.c
 *
 *  Created on: 8 de jan de 2020
 *      Author: evandro
 */

#include "serial.h"

/**
 *
 */
int serial_file_descriptor;

/**
 *
 */
int serial_blocking_set(int fd, int should_block);

/**
 *
 */
int serial_init (char *portname, int speed, int parity)
{
	serial_file_descriptor = open(portname,O_RDWR | O_NOCTTY | O_SYNC);
	if(serial_file_descriptor < 0)
	{
		printf("\n\r Falha em abrir a %s ",portname);
		return -1;
	}
	else
	{
		printf("\n\r Sucesso em abrir a porta %s",portname);
	}

    struct termios tty;
    if (tcgetattr (serial_file_descriptor, &tty) != 0)
    {
    	printf("\n\r error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;	// 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break as \000 chars
    tty.c_iflag &= ~IGNBRK;         			// disable break processing
    tty.c_lflag = 0;                			// no signaling chars, no echo,
                                    			// no canonical processing
    tty.c_oflag = 0;                			// no remapping, no delays
    tty.c_cc[VMIN]  = 0;            			// read doesn't block
    tty.c_cc[VTIME] = 5;            			// 0.5 seconds read timeout
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); 	// shut off xon/xoff ctrl
    tty.c_cflag |= (CLOCAL | CREAD);			// ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      	// shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (serial_file_descriptor, TCSANOW, &tty) != 0)
    {
    	printf("\n\r error %d from tcsetattr", errno);
        return -1;
    }

    if(serial_blocking_set( serial_file_descriptor , 0) < 0)
    {
    	return -1;
    }

	return 0;
}

/**
 *
 */
int serial_blocking_set(int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		//error_message ("error %d from tggetattr", errno);
		printf("\n\r error %d from tggetattr", errno);
		return - 1;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		//error_message ("error %d setting term attributes", errno);
		printf("\n\r error %d setting term attributes", errno);
		return -1;
	}

	return 0;
}

/**
 *
 */
int serial_write(uint8_t *buffer,uint8_t size)
{
	uint8_t i = 0;
	uint8_t *data = 0;
	//uint8_t size = (uint8_t)(strlen((char *)buffer));

	printf("\n\r Escrever os byte");

	for(i=0;i<size;i++)
	{
		data = &buffer[i];
		if(write(serial_file_descriptor,data,1) < 0)
		{
			printf("\n\r Falha em escrever");
			return -1;
		}
		else
		{
			printf(" [%d]: %d",i,*data);
		}
		usleep(1000);
	}
	return 0;
}

/**
 *
 */
int serial_read(uint8_t *buffer)
{
	uint8_t i = 0;
	uint8_t buf[16] = {0};

	printf("\n\r Le os Bytes");

	if(read(serial_file_descriptor,buf,16) < 0)
	{
		printf("\n\r Falha em Ler");
		return -1;
	}
	else
	{
		for(i=0;i<16;i++)
		{
			printf(" [%d]: %d",i,buf[i]);
			*buffer = buf[i];
			buffer++;
		}
	}

	return 0;
}
