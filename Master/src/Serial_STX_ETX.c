/*
 ============================================================================
 Name        : Serial_STX_ETX.c
 Author      : Evandro Teixeira
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "stx_etx.h"
/*#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
*/
/**
 *
 */
#define OK	 (int)( 0 )
#define FAIL (int)(-1)

/**
 *
 */
//char *portname = "/dev/ttyACM0";

//int serial_init(int fd, int speed, int parity);
//int serial_blocking_set(int fd, int should_block);
//int serial_write(int fd, uint8_t *buffer,uint8_t size);

/**
 *
 */
int main(void)
{
//	uint8_t buffer_out[8] = {22,22,2,1,1,0,255,3};
	msg_t data = {0};
	uint8_t buffer_in[16] = {0};
	uint8_t i = 0;

	printf("\n\r Aplicacao de Teste Protocolo STX ETX");

	if(serial_init("/dev/ttyACM0",B115200,0) >= 0)
	{
		printf("\n\r Sucesso ao iniciar Serial");
	}
/*
	serial_write(buffer_out,8);
*/
	// LED Green
	data.byte_number = 1;
	data.command = 10;
	data.data[0] = 1;
	stx_etx_send(data);

	sleep(1);

	serial_read(buffer_in);
	printf("\n\r ");

	for(i=0;i<16;i++)
	{
		//printf(" [%d]: %d",i , buffer_in[i]);
	}

	// LED Blue
	data.byte_number = 1;
	data.command = 2;
	data.data[0] = 1;
	stx_etx_send(data);

	sleep(1);

	serial_read(buffer_in);
	printf("\n\r ");

	for(i=0;i<16;i++)
	{
		//printf(" [%d]: %d",i , buffer_in[i]);
	}
	/*
	//int ret = 0;
	uint8_t i = 0;
	uint8_t buffer_byte[8] = {22,22,2,1,1,1,254,3};
	uint8_t buf[8] = {0};

	int fd = open(portname,O_RDWR | O_NOCTTY | O_SYNC);
	if(fd < 0)
	{
		printf("\n\r Falha em abrir a porta /dev/ttyACM0 ");
	}
	else
	{
		printf("\n\r Sucesso em abrir a porta /dev/ttyACM0 ");
	}

	if(serial_init(fd, B115200, 0) == OK)
	{
		printf("\n\r Inicializado Comunicação Serial com Sucesso ");
	}
	else
	{
		printf("\n\r Falha na Inicializado Comunicação Serial ");
	}

	if(serial_blocking_set(fd,0) == OK)
	{
		printf("\n\r Configurado com sucesso ");
	}
	else
	{
		printf("\n\r Falha em configurar ");
	}
	*/
	//write(fd,"Teste",5);
/*
	for(i=0;i<8;i++)
	{
		if(write(fd,(void *)buffer_byte[i],1) < 0)
		{
			puts("\n\r Falha em escrever");
		}
		else
		{
			puts("\n\r Sucesso ao escrever");
			printf("[%d]: %d",i,buffer_byte[i]);
		}
		//buffer_byte++;
	}
*/
/*
	serial_write(fd,buffer_byte,8);

	sleep(1);

	printf("\n\r Le resposta do MCU ");
	read(fd,buf,8);

	for(i=0;i<8;i++)
	{
		printf("\n\r [%d]: %d",i,buf[i]);
	}
	*/
	printf("\n\r");

	return EXIT_SUCCESS;
}

/**
 *
 */
/*
int serial_init(int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr (fd, &tty) != 0)
    {
    	//error_message ("error %d from tcgetattr", errno);
    	printf("\n\r error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
    	//error_message ("error %d from tcsetattr", errno);
    	printf("\n\r error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}
*/
/**
 *
 */
/*
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
*/

/**
 *
 */
/*int serial_write(int fd, uint8_t *buffer,uint8_t size)
{
	uint8_t i = 0;
	uint8_t *data = 0;
	printf("\n\r Escrever os byte");

	for(i=0;i<size;i++)
	{
		data = &buffer[i];
		if(write(fd,data,1) < 0)
		{
			printf("\n\r Falha em escrever");
			return -1;
		}
		else
		{
			//puts("\n\r Sucesso ao escrever");
			printf("\n\r [%d]: %d",i,*data);
			//buffer++;
		}
		usleep(1000);
	}
	return 0;
}
*/
