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
#include "app.h"

/**
 *
 */
int main(void)
{
//	msg_t data = {0};
//	uint8_t buffer_in[16] = {0};
//	uint8_t i = 0;



	app();

	/*
	if(serial_init("/dev/ttyACM0",B115200,0) >= 0)
	{
		printf("\n\r Sucesso ao iniciar Serial");
	}
	*/
/*
	serial_write(buffer_out,8);
*/
	/*
	// LED Green
	data.byte_number = 1;
	data.command = 10;
	data.data[0] = 1;
	stx_etx_send(data);
	*/
	/*
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
	*/
	/*

	*/
	//write(fd,"Teste",5);


	printf("\n\r");
	return EXIT_SUCCESS;
}
