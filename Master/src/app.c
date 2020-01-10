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
		uint8_t buffer_in[16] = {0};

		getchar();
		printf("\n\r");
		printf("\n\r Deseja enviar comando para o Slave? [s / n]: ");
		//scanf("%c",&input_ch);
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

			usleep(500);

			serial_read(buffer_in);

			printf("\n\r ");
			printf("\n\r Frame Recebido: ");
			for(i=0;i<16;i++)
			{
				printf("[%d]", buffer_in[i]);
			}
			printf("\n\r");

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
	//int ret = 0;
	//ret = scanf("%d",&ch);
	//printf("\n\r %d",ret);
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
