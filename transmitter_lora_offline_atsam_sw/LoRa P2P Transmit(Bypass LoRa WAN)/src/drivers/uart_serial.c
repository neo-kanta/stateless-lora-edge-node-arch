/*
 * uart_serial.c
 *
 * Created: 15/2/2565 15:02:08
 *  Author: parisa
 */
//last Update 22062022
 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "asf.h"
#include "conf_serial.h"
#include "drivers/uart_serial.h"

/******************************** MACROS ***************************************/


/************************** GLOBAL VARIABLES ***********************************/
struct usart_module	serial_uart_module;

/**
 * Receive buffer
 * The buffer size is defined in conf_serial.h
 */
static uint8_t uart_serial_rx_buf[SERIAL_RX_BUF_SIZE_UART];

/**
 * Receive buffer head
 */
static uint8_t uart_serial_rx_buf_head;

/**
 * Receive buffer tail
 */
static uint8_t uart_serial_rx_buf_tail;

/**
 * Number of bytes in receive buffer
 */
static uint8_t uart_serial_rx_count;

char pstrSerial[255];	


/************************** EXTERN VARIABLES ***********************************/


/************************** FUNCTION PROTOTYPES ********************************/
void serial_init(void)
{
	/* Create and initialize config structure */
	struct usart_config serial_uart_config;
	/* Configure USART for unit test output */
	usart_get_config_defaults(&serial_uart_config);
	serial_uart_config.mux_setting = SERIAL_SERCOM_MUX_SETTING;

	serial_uart_config.pinmux_pad0 = SERIAL_SERCOM_PINMUX_PAD0;
	serial_uart_config.pinmux_pad1 = SERIAL_SERCOM_PINMUX_PAD1;
	serial_uart_config.pinmux_pad2 = SERIAL_SERCOM_PINMUX_PAD2;
	serial_uart_config.pinmux_pad3 = SERIAL_SERCOM_PINMUX_PAD3;
	serial_uart_config.baudrate    = USART_SERIAL_BAUDRATE;

	/* Enable transceivers */
	usart_enable_transceiver(&serial_uart_module, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&serial_uart_module, USART_TRANSCEIVER_RX);
	
	/* Initialize and enable device with config */
	usart_serial_init(&serial_uart_module, USART_SERIAL, &serial_uart_config);
	usart_enable(&serial_uart_module);
	
	USART_SERIAL_RX_ISR_ENABLE();

}

void serial_deinit(void){
	
	usart_disable(&serial_uart_module);
		
	/* Disable transceivers */
	usart_disable_transceiver(&serial_uart_module, USART_TRANSCEIVER_TX);
	usart_disable_transceiver(&serial_uart_module, USART_TRANSCEIVER_RX);
	
}

uint8_t serial_tx(uint8_t* data, uint8_t len){
	
	status_code_genare_t status;
	
	do{
		status = usart_serial_write_packet(&serial_uart_module,(uint8_t*)data, len);
	}while(status != STATUS_OK);
	
	return len;
}


uint8_t serial_rx(uint8_t* data, uint8_t max_len){
	
	uint8_t data_received = 0;
	
	if(uart_serial_rx_buf_tail >= uart_serial_rx_buf_head)
	{
		uart_serial_rx_count = uart_serial_rx_buf_tail - uart_serial_rx_buf_head;
	}
	else
	{
		uart_serial_rx_count = uart_serial_rx_buf_tail + (SERIAL_RX_BUF_SIZE_UART - uart_serial_rx_buf_head);
	}
		
	if (0 == uart_serial_rx_count) {
		return 0;
	}

	if (SERIAL_RX_BUF_SIZE_UART <= uart_serial_rx_count) {
		/*
		 * Bytes between head and tail are overwritten by new data.
		 * The oldest data in buffer is the one to which the tail is
		 * pointing. So reading operation should start from the tail.
		 */
		uart_serial_rx_buf_head = uart_serial_rx_buf_tail;

		/*
		 * This is a buffer overflow case. But still only the number of
		 * bytes equivalent to
		 * full buffer size are useful.
		 */
		uart_serial_rx_count = SERIAL_RX_BUF_SIZE_UART;

		/* Bytes received is more than or equal to buffer. */
		if (SERIAL_RX_BUF_SIZE_UART <= max_len) {
			/*
			 * Requested receive length (max_length) is more than
			 * the
			 * max size of receive buffer, but at max the full
			 * buffer can be read.
			 */
			max_len = SERIAL_RX_BUF_SIZE_UART;
		}
	} else {
		/* Bytes received is less than receive buffer maximum length. */
		if (max_len > uart_serial_rx_count) {
			/*
			 * Requested receive length (max_length) is more than
			 * the data
			 * present in receive buffer. Hence only the number of
			 * bytes
			 * present in receive buffer are read.
			 */
			max_len = uart_serial_rx_count;
		}
	}

	data_received = max_len;
	
	while (max_len > 0) {
		/* Start to copy from head. */
		*data = uart_serial_rx_buf[uart_serial_rx_buf_head];
		data++;
		max_len--;
		if ((SERIAL_RX_BUF_SIZE_UART - 1) == uart_serial_rx_buf_head) {
			uart_serial_rx_buf_head = 0;
		}
		else
		{
			uart_serial_rx_buf_head++;
		}
		
// 		if(uart_serial_rx_buf_head == uart_serial_rx_buf_tail){
// 			uart_serial_rx_buf_tail = 0;
// 			uart_serial_rx_buf_head = 0;
// 		}


	}
	return data_received;
		
}

uint8_t serial_getchar(void)
{
	uint8_t data;
	while (0 == serial_rx(&data, 1)) {
	}
	return data;
}

void serial_putchar(uint8_t data)
{
	serial_tx(&data, 1);
}

int serial_getchar_nowait(void)
{
	uint8_t c;
	int back = serial_rx(&c, 1);
	if (back >= 1) {
		return c;
	} else {
		return (-1);
	}
}

void USART_SERIAL_ISR_VECT(uint8_t instance)
{
	uint8_t temp;

	usart_serial_read_packet(&serial_uart_module, &temp, 1);

	/* Introducing critical section to avoid buffer corruption. */
	cpu_irq_disable();

	/* The number of data in the receive buffer is incremented and the
	 * buffer is updated. */

	uart_serial_rx_buf[uart_serial_rx_buf_tail] = temp;

	if ((SERIAL_RX_BUF_SIZE_UART - 1) == uart_serial_rx_buf_tail) {
		/* Reached the end of buffer, revert back to beginning of
		 * buffer. */
		uart_serial_rx_buf_tail = 0x00;
	} else {
		uart_serial_rx_buf_tail++;
	}

	cpu_irq_enable();
}

void serial_disable(void)
{
	usart_disable(&serial_uart_module);
}

void serial_enable(void)
{
	usart_enable(&serial_uart_module);
}

void serial_clr_buffer(void){
	
	uart_serial_rx_buf_head = 0;
	uart_serial_rx_buf_tail = 0;
	uart_serial_rx_count = 0;
	
}
/*********************************************************************//*
 \brief       to send string thru SAMR34 Uart Serial port
  \param[in]  str - pointer of constant char
 ************************************************************************/
// Send string thru SAMR34 Target Uart Serial
void printf_serial(const char* str)
{
	serial_tx((uint8_t*)str,strlen(str));
}

/*********************************************************************//*
 \brief      sprintf version for directing output to SAMR34 Uart Serial port
 \param[in]   
 ************************************************************************/
int sprintf_serial(const char * format, ... )
{
	va_list args;
	va_start(args, format);
	vsprintf(pstrSerial,format, args);
	va_end(args);
	serial_tx((uint8_t*)pstrSerial,strlen(pstrSerial));
	return 0;	
}