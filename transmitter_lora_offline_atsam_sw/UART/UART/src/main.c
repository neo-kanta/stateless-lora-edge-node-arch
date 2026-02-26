/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# Minimal main function that starts with a call to system_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

/*
* start main.c
*/
#include <asf.h>
#include <stdio.h>
#include "delay.h"
#include "stdbool.h"
#include "usart_interrupt.h"
#include "string.h"
#include "usart.h"
#include "tc.h"
/************************** macro definition ***********************************/
#define APP_DEBOUNCE_TIME   50	// button debounce time in ms
#define __MONITOR_UART__

/*
* ***************************** DECLARATIONS **********************************
*/

#define AT_OK "OK\r\n"
#define AT_ERROR "ERROR\r\n"
#define AT_UNKNOWN "UNKNOWN_COMMAND\r\n"
//
#define STX ':'
#define ETX 0x0D
#define ACK 0X06
#define NAK 0X15
//
#define TX_BUFFER_SIZE		    128
#define RX_BUFFER_SIZE			50
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_index = 0;
char SHARE_uart_DATABYTES[RX_BUFFER_SIZE];  // DATABYTES FOR ALL CHANNELS
//
//
bool MONITOR_uart_STX_COME = false;
bool MONITOR_uart_MessageComplete = false;
uint16_t MONITOR_uart_LRCBYTES[RX_BUFFER_SIZE];
uint8_t MONITOR_uart_InputIndex = 0;
bool MONITOR_ACK_Need = true;
bool MONITOR_uart_ACK = false;
uint8_t help_buff[20];
//
void configure_usart(void)
void configure_usart_callbacks(void);
static void usart_write_callback(struct usart_module *const usart_module);
static void usart_read_callback(struct usart_module *const uart_module);
void enqueue_uart(const uint8_t *data, size_t len);
void uart_event(const char *command);
//
void configure_tc(void);
void configure_tc_callbacks(void);
void tc0_callback(void);
//
void led_success(void);
void led_fail(void);
void led_test_start(void);


/*
* ************************* GLOBAL VARIABLE **********************************
*/
#ifdef __MONITOR_UART__
struct usart_module uart1;
struct tc_module tmr8bits;

uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_head = 0, tx_tail = 0;
#endif
uint8_t buttonPressed = 0 ;
uint8_t buttonCounter = 0 ;
uint8_t buffer[TX_BUFFER_SIZE] ;
uint8_t transmitTimerId;

bool flag_print_sw0 = false;
bool flag_print_sw1 = false;
/****************************** PROTOTYPES *************************************/
int main (void)
{
	system_init();
	system_board_init();
	delay_init();
	//
	system_interrupt_enable_global();
	//
	configure_usart();
	configure_usart_callbacks();
	//
	configure_tc();
	configure_tc_callbacks();
	//
	led_test_start();
	LED_On(LED_0_PIN);
	//tc_start_counter(&tmr8bits);
	while(1) {
		uint32_t status = tc_get_status(&tmr8bits);
		_usart_interrupt_handler(SERCOM0);
		//_tc_interrupt_handler(TC0);
		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* Yes, so turn LED on. */
			if (flag_print_sw0== false) {
				const uint8_t message[] = "Button Pressed 1 !\r\n";
				if(usart_get_job_status(&uart1 ,USART_TRANSCEIVER_TX ) != STATUS_BUSY) {
					usart_write_buffer_job(&uart1, message, sizeof(message));
				}
				//uart_print(message);
				flag_print_sw0= true;
			}
			LED_On(LED1);
		}
		else {
			/* No, so turn LED off. */
			flag_print_sw0 = false;
			LED_Off(LED1);
		}
		//usart_read_buffer_job(&uart1,
		//(uint8_t *)rx_buffer, RX_BUFFER_SIZE);
		//
		if (port_pin_get_input_level(BUTTON_1_PIN) == BUTTON_1_ACTIVE) {
			/* Yes, so turn LED on. */
			if (flag_print_sw1== false){
				const uint8_t message[] = "Button Pressed 1 !\r\n";
				if(usart_get_job_status(&uart1 ,USART_TRANSCEIVER_TX ) != STATUS_BUSY) {
					usart_write_buffer_job(&uart1, message, sizeof(message));
				}
				//uart_print(message);
				flag_print_sw1 = true;
			}
			LED_On(LED0);
		}
		else {
			/* No, so turn LED off. */
			flag_print_sw1 = false;
			LED_Off(LED0);
		}
		
		if (status & TC_STATUS_COUNT_OVERFLOW) {
			// Handle overflow
			LED_Toggle(LED_1_PIN);
			tc_clear_status(&tmr8bits, TC_STATUS_COUNT_OVERFLOW);
		}
		
		if (status & TC_STATUS_CHANNEL_0_MATCH) {
			// Handle channel 0
			LED_Toggle(LED_1_PIN);
			tc_clear_status(&tmr8bits, TC_STATUS_CHANNEL_0_MATCH);
		}
	}
}

/************************* FUNCTION DECLARATIONS *******************************/
void configure_usart(void){
	uint8_t massage ;
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);

	config_usart.transfer_mode = USART_TRANSFER_ASYNCHRONOUSLY;
	config_usart.baudrate	   = 19200;
	config_usart.parity		   = USART_PARITY_NONE;
	config_usart.stopbits      = USART_STOPBITS_1;
	config_usart.character_size= USART_CHARACTER_SIZE_8BIT;
	config_usart.mux_setting   = USART_RX_1_TX_0_XCK_1;
	config_usart.pinmux_pad0   = PINMUX_PA05D_SERCOM0_PAD1; // RX
	config_usart.pinmux_pad1   = PINMUX_PA04D_SERCOM0_PAD0; // TX
	config_usart.pinmux_pad2   = PINMUX_UNUSED;
	config_usart.pinmux_pad3   = PINMUX_UNUSED;

	while (usart_init(&uart1,SERCOM0, &config_usart) != STATUS_OK) { }
	usart_enable(&uart1);
}

void configure_usart_callbacks(void) {
	//usart_register_callback(&uart1, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&uart1, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	//usart_enable_callback(&uart1, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&uart1, USART_CALLBACK_BUFFER_RECEIVED);
}

void configure_tc(void) {
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config_tc.counter_8_bit.period = 200;
	config_tc.counter_8_bit.compare_capture_channel[0] = 50;
	config_tc.counter_8_bit.compare_capture_channel[1] = 54;
	//config_tc.counter_8_bit.value = 0x01;
	//tc_set_count_value(&tmr8bits, 0x01); // 5 seconds
	
	while (tc_init(&tmr8bits, TC0, &config_tc) != STATUS_OK) { }
	
	tc_enable(&tmr8bits);
}

void configure_tc_callbacks(void) {
	tc_register_callback(&tmr8bits,tc0_callback ,TC_CALLBACK_OVERFLOW);
	//tc_register_callback(&tmr8bits,tc0_callback ,TC_CALLBACK_CC_CHANNEL0);
	//tc_register_callback(&tmr8bits, tc0_callback, TC_CALLBACK_CC_CHANNEL1);
	
	tc_enable_callback(&tmr8bits, TC_CALLBACK_OVERFLOW);
	//tc_enable_callback(&tmr8bits,TC_CALLBACK_CC_CHANNEL0);
	//tc_enable_callback(&tmr8bits,TC_CALLBACK_CC_CHANNEL1);
}

void tc0_callback(){
	LED_Toggle(LED0);
	delay_ms(500);
	//const char* unknown_msg = AT_UNKNOWN;
	//usart_write_buffer_job(&uart1, (uint8_t *)unknown_msg, strlen(unknown_msg));
}

static void usart_write_callback(struct usart_module *const usart_module) {
	LED_Toggle(LED2);
	delay_ms(500);
}

static void usart_read_callback(struct usart_module *const uart_module) {
	
	rx_buffer[rx_index] = '\0';
	rx_index = 0;
	//usart_write_buffer_job(&uart1, (uint8_t *)AT_UNKNOWN, strlen(AT_UNKNOWN));
	usart_read_buffer_job(&uart1, rx_buffer, RX_BUFFER_SIZE);
	// Execute the AT command
	uart_event((const char *)rx_buffer);
}


void uart_event(const char *command) {
	if (strcmp(command, "AT") == 0 || strcmp(command, "AT\r\n") == 0) {
		usart_write_buffer_job(&uart1, (uint8_t *)AT_OK, strlen(AT_OK));
	} 
	else {
	usart_write_buffer_job(&uart1, (uint8_t *)AT_ERROR, strlen(AT_ERROR));
	}

}

void enqueue_uart(const uint8_t *data, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		tx_buffer[tx_head] = data[i];
		tx_head = (tx_head + 1) % TX_BUFFER_SIZE;
	}

	// PRIORITY
	// 1ST UART_PRIORITY
	if (usart_get_job_status(&uart1, USART_TRANSCEIVER_TX) == STATUS_OK) {
		usart_write_buffer_job(&uart1, &tx_buffer[tx_tail], 1);
		tx_tail = (tx_tail + 1) % TX_BUFFER_SIZE;
	}
}

void Clear_SHARE_uart_DATABYTES() {
	uint8_t i;
	for (i = 0; i < RX_BUFFER_SIZE ; i++) {
		SHARE_uart_DATABYTES[i] = 0;
	}
}



void led_success(void) {
	LED_Off(LED_2_PIN);
	delay_ms(300);
	LED_On(LED_2_PIN);
	delay_ms(250);
	LED_Off(LED_2_PIN);
	delay_ms(300);
	LED_On(LED_2_PIN);
}

void led_fail(void) {
	LED_Off(LED_2_PIN);
	delay_ms(1000);
	LED_On(LED_2_PIN);
	delay_ms(250);
	LED_Off(LED_2_PIN);
	delay_ms(1000);
	LED_On(LED_2_PIN);
}

void led_test_start(void) {
	LED_Off(LED_1_PIN);
	delay_ms(1000);
	LED_On(LED_1_PIN);
	delay_ms(250);
	LED_Off(LED_1_PIN);
	delay_ms(1000);
	LED_On(LED_1_PIN);
}
/*
* end main.c
*/