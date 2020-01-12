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
	app(); // Aplicação de Teste Protocolo STX / ETX

	printf("\n\r");
	return EXIT_SUCCESS;
}
