/*
 * stx_etx.c
 *
 *  Created on: 27/12/2019
 *      Author: evandro
 */

#include "../Inc/stx_etx.h"

/**
 *
 */
static data_msg_t data = {0};
QueueHandle_t queue_data_rx;
QueueHandle_t queue_msg_out;
QueueHandle_t queue_msg_in;

/**
 *
 */
void stx_etx_task_rx(void *pvParameters);
void stx_etx_task_tx(void *pvParameters);
//uint8_t stx_etx_calculate_crc(data_msg_t data);
uint8_t stx_etx_calculate_checksum(data_msg_t data);
status_interpreter_t stx_etx_interpreter(data_rx_t data_in/*, data_msg_t *data_out*/);

/**
 *
 */
void stx_etx_init(void)
{
	/* Creat Queue Data RX UART */
	queue_data_rx = xQueueCreate(NUMBER_OF_ITEMS,sizeof(data_rx_t));
//	if(!queue_data_rx)
//	{
//		/* Fail in creat queue */
//	}

	/* Creat Queue MSG In */
	queue_msg_out = xQueueCreate(NUMBER_OF_ITEMS,sizeof(data_msg_t));
//	if(!queue_msg_out)
//	{
//		/* Fail in creat queue */
//	}

	/* Creat Queue MSG Out */
	queue_msg_in = xQueueCreate(NUMBER_OF_ITEMS,sizeof(data_msg_t));
//	if(!queue_msg_in)
//	{
//		/* Fail in creat queue */
//	}

	/* Creat Task Rx */
	if(xTaskCreate(stx_etx_task_rx,"TaskRX",configMINIMAL_STACK_SIZE * 2,NULL,(configMAX_PRIORITIES-3),NULL) != pdPASS)
	{
		/* Fail in creat task */
	}

	/* Creat Task Tx */
	if(xTaskCreate(stx_etx_task_tx,"TaskTX",configMINIMAL_STACK_SIZE,NULL,(configMAX_PRIORITIES-3),NULL) != pdPASS)
	{
		/* Fail in creat task */
	}
}

/**
 *
 */
void stx_etx_task_rx(void *pvParameters)
{
	char txt[32];
//	uint8_t i = 0;
	data_rx_t data_rx;
//	static data_msg_t data_tx;

	while(1)
	{
		if(xQueueReceive(queue_data_rx,&data_rx,(TickType_t)portMAX_DELAY) == pdTRUE)
		{
//			sprintf(txt,"\n\r%d %lu",data_rx.data, data_rx.time);
//			MX_USART1_UART_String(txt);

			switch( stx_etx_interpreter(data_rx/*,&data_tx*/) )
			{
			    case FAULT_START_BYTE:
			    case FAULT_TIMEOUT:
			    case FAULT_INCORRECT_CRC:
			    case FAULT_STOP_BYTE:
			    case FAULT_INTERNAL_ERROR:
			    case FAULT_NO_COMMAND:
			    	/* Notify Master of Communication Failure */
//			    	data_tx.byte_number = 0;
//			    	data_tx.command = 0;
//			    	for(i=0;i<8;i++)
//			    	{
//			    		data_tx.data[i] = 0;
//			    	}
//
//					if(xQueueSend(queue_msg_out,&data_tx,(TickType_t)10) != pdPASS)
//					{
//						/* Failed to post the message  */
//					}
			    break;
				case MSG_COMPLETED:
					/* Send Data of App*/
//					if(data_tx != NULL)
//					{
						if(xQueueSend(queue_msg_in,&data,(TickType_t)portMAX_DELAY) != pdPASS)
						{
							/* Failed to post the message  */
						}
//					}
				break;
				case IDLE:
				case PROCESSING:
				default:
				break;
			}
		}
	}
}

/**
 *
 */
void stx_etx_task_tx(void *pvParameters)
{
	data_msg_t data_tx = {0};
	uint8_t data[16] = {0};
	uint8_t index = 0;
	uint8_t i = 0;

	while(1)
	{
		if(xQueueReceive(queue_msg_out,&data_tx,(TickType_t)portMAX_DELAY) == pdTRUE)
		{
			/* Prepares data to be transmitted by UART */
			index = 0;
			data[index++] = VALUE_SYN;
			data[index++] = VALUE_SYN;
			data[index++] = VALUE_STX;
			data[index++] = data_tx.byte_number;
			data[index++] = data_tx.command;
			for(i=0;i<data_tx.byte_number;i++)
			{
				data[index++] = data_tx.data[i];
			}
			data[index++] = stx_etx_calculate_checksum(data_tx);
			data[index++] = VALUE_ETX;

			/* Transmit data by UART */
			MX_USART1_UART_Transmit(data,index);
		}
	}
}

/**
 *
 */
void stx_etx_queue_get_data(uint8_t data)
{
	data_rx_t data_rx;
	data_rx.data = data;
	data_rx.time = HAL_GetTick();
	xQueueSendFromISR(queue_data_rx,&data_rx,pdFALSE );
}

/**
 *
 */
bool stx_etx_queue_receive(data_msg_t *data_out, uint32_t tick)
{
	bool ret = false;
	static data_msg_t data = {0};

	if(xQueueReceive(queue_msg_in,&data,(TickType_t)tick) == pdTRUE)
	{
		*data_out = data;
		//data_out = &data;
		ret = true;
	}
	return ret;
}

/**
 *
 */
bool stx_etx_queue_send(data_msg_t data,uint32_t tick)
{
	bool ret = false;

	if(xQueueSend(queue_msg_out,&data,(TickType_t)tick) == pdPASS)
	{
		ret = true;
	}

	return ret;
}

/**
 * note:
 * http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
 * https://crccalc.com/
 */
//uint8_t stx_etx_calculate_crc(data_msg_t data)
//{
//	uint8_t buffer[16] = {0};
//	uint8_t index = 0;
//	uint8_t ret = 0;
//	uint8_t i = 0;
//
//	buffer[index++] = data.byte_number;
//	buffer[index++] = data.command;
//
//	for(i=0;i<data.byte_number;i++)
//	{
//		buffer[index++] = data.data[i];
//	}
//
//	ret = MX_CRC_Calculate((uint32_t*)buffer,index);
//
//	return ret;
//}

/**
 * @brief
 * @note: http://blog.datek.com.br/2019/10/ccomo-calcular-checksum/
 */
uint8_t stx_etx_calculate_checksum(data_msg_t data)
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

/**
 *
 */
status_interpreter_t stx_etx_interpreter(data_rx_t data_in /*,data_msg_t *data_out*/)
{
	static state_data_t state_data = BYTE_NUMBER;
	static state_interpreter_t state_interpreter = SYN1;
	static data_rx_t data_old = {0};
	//static msg_t msg = {0};
	//static data_msg_t data = {0};
	static uint8_t index_data = 0;
	status_interpreter_t ret = IDLE;
	//data_out = NULL;

	/* Check the time delta between bytes */
	if((data_in.time - data_old.time) > TIMEOUT)
	{
		/* returns to initial state */
		state_interpreter = SYN1;
	}

	switch(state_interpreter)
	{
		case SYN1: /* Start Byte 0 */
			/* Check byte value SYN */
			if(data_in.data == VALUE_SYN)
			{
				//msg.syn1 = data_in.data;
				state_interpreter = SYN2;
				ret = PROCESSING;
			}
			else
			{
				/* signals failure and returns to initial state */
				ret = FAULT_START_BYTE;
			}
		break;
		case SYN2: /* Start Byte 1 */
			/* Check the time delta between bytes */
//			if((data_in.time - data_old.time) < TIMEOUT)
//			{
				/* Check byte value SYN */
				if(data_in.data == VALUE_SYN)
				{
					//msg.syn2 = data_in.data;
					state_interpreter = STX;
					ret = PROCESSING;
				}
				else
				{
					/* signals failure and returns to initial state */
					state_interpreter = SYN1;
					ret = FAULT_START_BYTE;
				}
//			}
//			else
//			{
//				/* signals failure and returns to initial state */
//				state_interpreter = SYN1;
//				ret = FAULT_TIMEOUT;
//			}
		break;
		case STX: /* Start Byte 2 */
			/* Check the time delta between bytes */
//			if((data_in.time - data_old.time) < TIMEOUT)
//			{
				/* Check byte value STX */
				if(data_in.data == VALUE_STX)
				{
					//msg.stx = data_in.data;
					state_interpreter = DATA;
					state_data = BYTE_NUMBER;
					ret = PROCESSING;
				}
				else
				{
					/* signals failure and returns to initial state */
					state_interpreter = SYN1;
					ret = FAULT_START_BYTE;
				}
//			}
//			else
//			{
//				/* signals failure and returns to initial state */
//				state_interpreter = SYN1;
//				ret = FAULT_TIMEOUT;
//			}
		break;
		case DATA: /* Message assembler */
			/* Check the time delta between bytes */
//			if((data_in.time - data_old.time) < TIMEOUT)
//			{
				ret = PROCESSING;

				switch(state_data)
				{
					case BYTE_NUMBER:
						state_data = COMMAND;
						//msg.data.byte_number = data_in.data;
						data.byte_number = data_in.data;
					break;
					case COMMAND:
						state_data = DATA_ASSEMBLER;
						//msg.data.command = data_in.data;
						data.command = data_in.data;
						index_data=0;
					break;
					case DATA_ASSEMBLER:
						//msg.data.data[index_data++] = data_in.data;
						data.data[index_data++] = data_in.data;
						if(index_data >= data.byte_number)
						{
							state_data = BYTE_NUMBER;
							state_interpreter = CHK_CRC;
						}
					break;
					case MAX_STATE_DATA:
					default:
						/* signals failure and returns to initial state */
						state_interpreter = SYN1;
						state_data = BYTE_NUMBER;
						ret = FAULT_INTERNAL_ERROR;
					break;
				}
//			}
//			else
//			{
//				/* signals failure and returns to initial state */
//				state_interpreter = SYN1;
//				ret = FAULT_TIMEOUT;
//			}
		break;
		case CHK_CRC:
			/* Check the time delta between bytes */
//			if((data_in.time - data_old.time) < TIMEOUT)
//			{
				/* computes the received message's CRC value
				 * and compares the transmitted CRC value */
				//if(data.data == stx_etx_calculate_crc(msg.data))
				if(data_in.data == stx_etx_calculate_checksum(data))
				{
					state_interpreter = ETX;
				//	*data_out = msg.data;
				}
				else
				{
					/* signals failure and returns to initial state */
					state_interpreter = SYN1;
					ret = FAULT_INCORRECT_CRC;
				}
//			}
//			else
//			{
//				/* signals failure and returns to initial state */
//				state_interpreter = SYN1;
//				ret = FAULT_TIMEOUT;
//			}
		break;
		case ETX: /* Stop Byte  */
//			if((data_in.time - data_old.time) < TIMEOUT)
//			{
				if(data_in.data == VALUE_ETX)
				{
					//data_msg_t dt = {0};
					//data_out = &(data_msg_t)msg.data;
					//dt = msg.data;
					//data_out = &data;
					//*data_out = data;
					state_interpreter = SYN1;
					ret = MSG_COMPLETED;
				}
				else
				{
					/* signals failure and returns to initial state */
					state_interpreter = SYN1;
					ret = FAULT_STOP_BYTE;
				}
//			}
//			else
//			{
//				/* signals failure and returns to initial state */
//				state_interpreter = SYN1;
//				ret = FAULT_TIMEOUT;
//			}
		break;
		case MAX_STATE_INTERPRETER:
		default:
			/* signals failure and returns to initial state */
			state_interpreter = SYN1;
			ret = FAULT_INTERNAL_ERROR;
		break;
	}

	/* update variable that stores last data */
	data_old = data_in;

	return ret;
}

