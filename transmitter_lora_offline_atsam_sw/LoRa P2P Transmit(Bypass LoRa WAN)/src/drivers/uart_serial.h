/*
 * uart_serial.h
 *
 * Created: 15/2/2565 15:02:27
 *  Author: parisa
 */ 
//last Update 22062022

#ifndef UART_SERIAL_H_
#define UART_SERIAL_H_

#include <stdint.h>
/**
 * \brief Initializes the Serial IO Module
 * \return STATUS_OK for successful initialization and FAILURE incase the IO is
 * not initialized
 */
void serial_init(void);

/**
 * \brief DeInitializes the Serial IO Module
 */
void serial_deinit(void);

/**
 * \brief Transmits data via UART
 * \param data Pointer to the buffer where the data to be transmitted is present
 * \param length Number of bytes to be transmitted
 *
 * \return Number of bytes actually transmitted
 */
uint8_t serial_tx(uint8_t* data, uint8_t len);

/**
 * \brief Receives data from UART
 *
 * \param data pointer to the buffer where the received data is to be stored
 * \param max_length maximum length of data to be received
 *
 * \return actual number of bytes received
 */
uint8_t serial_rx(uint8_t* data, uint8_t max_len);

/**
 * \brief This function performs a blocking character receive functionality
 * \return returns the data which is received
 */
uint8_t serial_getchar(void);

/**
 * \brief This function prints the character to the host
 * \return void
 */

void serial_putchar(uint8_t);

/**
 * \brief This function performs a non-blocking character receive functionality
 * \return '-1' if no data is recieved or returns the data if a character is
 * received
 */
int serial_getchar_nowait(void);

/**
 * \brief Disables the Serial IO Module
 * \return void
 */
void serial_disable(void);

/**
 * \brief Enables the Serial IO Module
 * \return void
 */
void serial_enable(void);

/**
 * \brief Clear all receive parameters
 * \return void
 */
void serial_clr_buffer(void);

void USART_SERIAL_ISR_VECT(uint8_t instance);

//--------------------------------------------------------------------------

/*********************************************************************//*
 \brief       to send string thru SAMR34 Uart Serial
 ************************************************************************/
// Send string thru SAMR34 Target Uart Serial
void printf_serial(const char* str);


/*********************************************************************//*
 \brief      sprintf version for directing output to SAMR34 Uart Serial port
 \param[in]   
 ************************************************************************/
int sprintf_serial(const char * format, ... ) __attribute__ ((format (gnu_printf, 1, 2)));

#endif /* UART_SERIAL_H_ */
