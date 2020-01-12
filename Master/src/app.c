/*
 * app.c
 *
 *  Created on: 9 de jan de 2020
 *      Author: evandro
 */

#include "app.h"

/**
 *
 */
int app_get_int(uint8_t *dt);
int app_interpreter(uint8_t *buffer, msg_t *data);

/**
 *
 */
void app(void)
{
	static uint8_t app_control = 0;
	uint8_t i = 0;
	char port[16] = {0};
	char input_ch = 0;

	printf("\e[H\e[2J");
	printf("\n\r");

	for (i=0;i<50;i++)
	{
		printf("*");
	}


	printf("\n\r      Aplicacao de Teste Protocolo STX ETX ");
	printf("\n\r");

	for (i=0;i<50;i++)
	{
		printf("*");
	}

	printf("\n\r");
	printf("\n\r Digite a porta Serial a ser utilizada: ");
	scanf("%s",port);

	if(serial_init(port,B115200,0) == FAIL)
	{
		return;
	}

	do
	{
		msg_t data = {0};
		uint8_t x = 0;
		uint8_t buffer_in[16] = {0};

		getchar();
		printf("\n\r");
		printf("\n\r Deseja enviar comando para o Slave? [s / n]: ");
		input_ch = getchar();

		if((input_ch == 's') || (input_ch == 'S'))
		{
			app_control = 1;

			NUMBER_BYTE:
			printf("\n\r Digite o numero de byte a ser transmitido: ");
			if( app_get_int(&data.byte_number) == FAIL)
			{
				getchar();
				goto NUMBER_BYTE;
			}

			COMMAND:
			printf("\n\r Digite o valor do Command: ");
			if( app_get_int(&data.command) == FAIL)
			{
				getchar();
				goto COMMAND;
			}

			for(i=0;i<data.byte_number;i++)
			{
				if(i<NUMBER_OF_ITEMS)
				{
					DATA:
					printf("\n\r Digite o valor do Data[%d]: ",i);
					if( app_get_int(&data.data[i]) == FAIL)
					{
						getchar();
						goto DATA;
					}
				}
			}

			stx_etx_send(data);

			usleep(2000);

			serial_read(buffer_in);

			printf("\n\r ");
			printf("\n\r Frame Recebido: ");

			i = 0;
			printf("[%d]", buffer_in[i++]); // SYN 			0
			printf("[%d]", buffer_in[i++]); // SYN 			1
			printf("[%d]", buffer_in[i++]); // STX 			2
			printf("[%d]", buffer_in[i++]); // Number Byte	3
			printf("[%d]", buffer_in[i++]); // Command		4
			if(buffer_in[3] < NUMBER_OF_ITEMS)
			{
				for(x=0;x < buffer_in[3];x++)
				{
					printf("[%d]", buffer_in[i++]); // Data
				}
			}
			printf("[%d]", buffer_in[i++]); // CheckSum
			printf("[%d]", buffer_in[i++]); // ETX
			printf("\n\r");

			/* Interprets the received data */
			{
				msg_t input = {0};

				if(app_interpreter(buffer_in,&input) == OK)
				{
					printf("\n\r Numero de Byte: %d \n\r",input.byte_number);
					printf("\n\r Command / Status: %d \n\r",input.command);
					for(i=0;i<input.byte_number;i++)
					{
						printf("\n\r Data[%d]: %d\n\r",i,input.data[i]);
					}
				}
			}

			printf("\n\r");
			printf("\n\r");
			for (i=0;i<50;i++)
			{
				printf("*");
			}

		}
		else
		{
			app_control = 0;
		}
	}while(app_control == 1);

	printf("\n\r Fim da Aplicação ");
	printf("\n\r");
}

/**
 *
 */
int app_get_int(uint8_t *dt)
{
	int ch = 0;
	if( scanf("%d",&ch) == 1 )
	{
		*dt = ch;
		return OK;
	}
	else
	{
		return FAIL;
	}
}

/**
 *
 */
int app_interpreter(uint8_t *buffer, msg_t *data)
{
	uint8_t i = 0, x = 0;

	if(buffer[i++] != VALUE_SYN)
	{
		printf("\n\r Byte 0 de sincronismo divergente ");
		return FAIL;
	}
	if(buffer[i++] != VALUE_SYN)
	{
		printf("\n\r Byte 1 de sincronismo divergente ");
		return FAIL;
	}
	if(buffer[i++] != VALUE_STX)
	{
		printf("\n\r Byte STX divergente ");
		return FAIL;
	}

	data->byte_number = buffer[i++];
	if(data->byte_number >= NUMBER_OF_ITEMS)
	{
		printf("\n\r Numero de byte fora dos limites ");
		return FAIL;
	}

	data->command = buffer[i++];

	for(x=0;x < data->byte_number;x++)
	{
		data->data[x] = buffer[i++];
	}

	if(stx_etx_calculate_checksum(*data) != buffer[i++])
	{
		printf("\n\r CheckSun divergente ");
		return FAIL;
	}

	if(buffer[i++] != VALUE_ETX)
	{
		printf("\n\r Byte ETX divergente ");
		return FAIL;
	}

	printf("\n\r Dados recebidos com sucesso \n\r");
	return OK;
}
