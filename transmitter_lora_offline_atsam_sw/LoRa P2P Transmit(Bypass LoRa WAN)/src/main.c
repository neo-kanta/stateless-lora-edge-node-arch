/*
* Start Transmit file
*/
#include <asf.h>
#include <stdio.h>
#include "stdio_serial.h"
#include "radio_interface.h"
#include "radio_driver_hal.h"
#include "aes_engine.h"
#include "sw_timer.h"
#include "system_init.h"
#include "lorawan.h"
#include "stdbool.h"
#include "string.h"
#include "radio_interface.h"
#include "services/eeprom/at24mac402.h"
#include "conf_wdt.h"
#include "wdt.h"
#include "wdt_callback.h"
//#include "sio2host.h"

/************************** MONITOR INCLUDE ***********************************/
#include "usart.h"
/************************** macro definition ***********************************/
#define APP_DEBOUNCE_TIME   50	// button debounce time in ms
#define BUFFER_SIZE			50
#define __MONITOR_UART__	1

// Lora MACRO
#define LORAWAN_NW_JOINED	0x01
#define LORAWAN_TX_BUFFER_SIZE		220
#define LORAWAN_RX_BUFFER_SIZE		220
#define LORAWAN_SESSIONKEY_LENGTH					(16)
//
#define STATE_RECEIVE		1
#define STATE_TRANSMIT		2

/************************** Global variables ***********************************/
#ifdef __MONITOR_UART__
struct usart_module usart1;
#endif

// Lora Declare

/*ABP Join Parameters */
uint32_t devAddr ;
static uint8_t devNwksKey[LORAWAN_SESSIONKEY_LENGTH] ;
static uint8_t devAppsKey[LORAWAN_SESSIONKEY_LENGTH] ;

/* OTAA join parameters */
uint8_t devEui[8] ;
static uint8_t devJoinEui[8] ;
static uint8_t devAppKey[LORAWAN_SESSIONKEY_LENGTH] ;

char uplinkBuffer[LORAWAN_TX_BUFFER_SIZE];
uint8_t dwlinkBuffer[LORAWAN_RX_BUFFER_SIZE];
uint32_t DutyCyclePending = 0;

#define AT_OK "OK\r\n"
#define AT_ERROR "ERROR\r\n"
#define AT_UNKNOWN "UNKNOWN_COMMAND\r\n"
uint8_t stateId;
//
uint8_t buttonPressed = 0 ;
uint8_t buttonCounter = 0 ;
uint8_t buffer[BUFFER_SIZE] ;
uint8_t transmitTimerId;

#define RX_BUFFER_SIZE			50
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_index = 0;

bool flag_sw0_press = false;
bool flag_sw1_press = false;
/****************************** PROTOTYPES *************************************/
static void init(void) ;
static void configure_led(void) ;
static void configure_radio(void) ;
static void get_attr_radio(void);
//
void configure_usart(void);
void configure_usart_callbacks(void);
static void usart_write_callback(struct usart_module *const usart_module);
static void usart_read_callback(struct usart_module *const uart_module);
void usart_print(struct usart_module *const _uart, const char *str);
void usart_printlen(struct usart_module *const _uart, const char *str, size_t len);
void uart_event(const char *command);
//
static void configure_transmit_timer(void);
static void transmit_timer_callback(void *param);
void radio_transmit_uplink(uint8_t *data, uint16_t len);
void radio_enter_receive_mode(void);
void radio_exit_receive_mode(void) ;
void print_radio_state(RadioState_t radio_state);
void print_radio_error_state(RadioError_t radio_error);
static void eui_addr_read(void);
//
SYSTEM_TaskStatus_t APP_TaskHandler(void) ;
void appData_callback(void *appHandle, appCbParams_t *appdata) ;
void print_array(uint8_t *array, uint8_t length) ;
//
void led_success(void);
void led_fail(void);
void led_test_start(void);
void led_test(void);
//
void wdt_init(void);


/****************************** FUNCTIONS *************************************/
static void init(void)
{
	system_init() ;

	delay_init() ;
	board_init() ;
	configure_usart();
	configure_usart_callbacks();
	//configure_led() ;
	INTERRUPT_GlobalInterruptEnable() ;
	system_interrupt_enable_global();
	// LoRaWAN Stack driver init
	HAL_RadioInit() ;
	AESInit() ;
	SystemTimerInit() ;
	Stack_Init() ;
	LORAWAN_Init(appData_callback, NULL) ;
}

int main (void)
{
	init() ;
	configure_radio() ;
	// Initialize and start the timer

	SwTimerCreate(&transmitTimerId);
	configure_transmit_timer();
	eui_addr_read();
	delay_ms(150);
	led_test_start();
	LED_On(LED_0_PIN);
	LED_On(LED_1_PIN);
	LED_On(LED_2_PIN);
	radio_enter_receive_mode();
	delay_ms(150);
	
	wdt_init();
	while (1)
	{
		SYSTEM_RunTasks() ;
		usart_read_buffer_job(&usart1, rx_buffer, RX_BUFFER_SIZE);
		//if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			///* Yes, so turn LED on. */
			//LED_On(LED_1_PIN);
			//if(!flag_sw0_press){
				//buffer[0] = 0x01 ;
				//radio_transmit_uplink(buffer, 1) ;
				//get_attr_radio();
				//flag_sw0_press = true;
			//}
			//port_pin_set_output_level(GPIO_0_PIN,true);
			//port_pin_set_output_level(GPIO_1_PIN,false);
		//}
		//else {
			///* No, so turn LED off. */
			//LED_Off(LED1);
			//flag_sw0_press = false;
		//}
		//
		//if (port_pin_get_input_level(BUTTON_1_PIN) == BUTTON_0_ACTIVE) {
			//LED_On(LED0);
			////if(!flag_sw1_press){
			////uint8_t data[] = ":PG\r\n";
			////radio_transmit_uplink(data, sizeof(data));
			////usart_write_buffer_job(&uart1,data , sizeof(data));
			////flag_sw1_press = true;
			////}
			//
			//port_pin_set_output_level(GPIO_0_PIN,false);
			//port_pin_set_output_level(GPIO_1_PIN,true);
		//}
		//else {
			//LED_Off(LED0);
			//flag_sw1_press = false;
		//}
		wdt_reset_count();
	}
}

static void transmit_timer_callback(void *param) {
	uint8_t data[] = ":PR\r\n";
	//radio_exit_receive_mode();
	usart_write_buffer_job(&usart1,data , sizeof(data));
	radio_exit_receive_mode();
	delay_ms(250);
	radio_transmit_uplink(data, sizeof(data));
	delay_ms(250);
	LED_Toggle(LED2);
	
	// Restart the timer
	configure_transmit_timer();
	delay_ms(250);
}

static void configure_transmit_timer(void) {
	uint32_t timerCount = 15000000;  // 10 seconds in microseconds
	SwTimerStart(transmitTimerId, timerCount, SW_TIMEOUT_RELATIVE, transmit_timer_callback, NULL);
}

SYSTEM_TaskStatus_t APP_TaskHandler(void)
{
	//buffer[0] = 0x01 ;
	//radio_transmit_uplink(buffer, 1) ;
	return SYSTEM_TASK_SUCCESS;
}

static void get_attr_radio(void) {
	char buffer_get_attr[128];
	RadioError_t ret_code;
	
	// Set Frequency 923.2 MHz
	usart_print(&usart1, "\r\n*******************\r\n");
	usart_print(&usart1, "Get Attribute\r\n");
	uint32_t frequency = 923200000;
	ret_code = RADIO_GetAttr(CHANNEL_FREQUENCY, (void *)&frequency);
	sprintf(buffer_get_attr,"Frequency %d\r\n", frequency);
	usart_print(&usart1, buffer_get_attr);
	print_radio_error_state(ret_code);

	// radio set pwr 15
	int16_t outputPwr = 15 ;
	ret_code = RADIO_GetAttr(OUTPUT_POWER,(void *)&outputPwr) ;
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_get_attr, "Configuring Radio Output Power %d\r\n", outputPwr);
	usart_print(&usart1, buffer_get_attr);

	// radio set sf sf12
	int16_t sf = SF_12 ;
	ret_code = RADIO_GetAttr(SPREADING_FACTOR,(void *)&sf) ;
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_get_attr, "Configuring Radio SF %d\r\n", sf);
	usart_print(&usart1, buffer_get_attr);

	// radio set wdt 60000
	uint32_t wdt = 60000 ;
	ret_code = RADIO_GetAttr(WATCHDOG_TIMEOUT,(void *)&wdt) ;
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_get_attr, "Configuring Radio Watch Dog Timeout %ld\r\n", wdt);
	usart_print(&usart1, buffer_get_attr);
}

/* Configure LoRa Radio for P2P */
static void configure_radio(void)
{
	char buffer_radio_param[128] = {0};
	//buffer_radio_param[strlen(buffer_radio_param)] = '\0';
	RadioError_t ret_code;

	// mac reset 923 - reset the LoRaWAN stack and initialize it with the parameters of the selected ISM band
	// Set some radio parameters manually, the others parameters are following the selected ISM band
	LORAWAN_Reset(ISM_EU868) ;	// see stack_common.h for the definition of IsmBand_t
	delay_ms(250);
	sprintf(buffer_radio_param, "\r\nism_band %d\r\n",ISM_THAI923);
	usart_print(&usart1, buffer_radio_param);
	
	// mac pause
	uint32_t time_ms = LORAWAN_Pause() ;
	delay_ms(250);
	sprintf(buffer_radio_param, "\r\nMAC Pause %ld\r\n", time_ms);
	usart_print(&usart1, buffer_radio_param);
	
	// Set Frequency 923.2 MHz
	uint32_t frequency = 923200000;
	ret_code = RADIO_SetAttr(CHANNEL_FREQUENCY, (void *)&frequency);
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_radio_param,"\r\nFrequency %d\r\n", frequency);
	usart_print(&usart1, buffer_radio_param);
	
	// radio set pwr 15
	int16_t outputPwr = 255 ; // 15 -> 47
	ret_code = RADIO_SetAttr(OUTPUT_POWER,(void *)&outputPwr) ;
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_radio_param, "\r\nConfiguring Radio Output Power %d\r\n", outputPwr);
	usart_print(&usart1, buffer_radio_param);

	// radio set sf sf12
	int16_t sf = SF_12 ;
	ret_code = RADIO_SetAttr(SPREADING_FACTOR,(void *)&sf) ;
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_radio_param, "\r\nConfiguring Radio SF %d\r\n", sf);
	usart_print(&usart1, buffer_radio_param);

	// radio set wdt 60000
	uint32_t wdt = 60000 ;
	ret_code = RADIO_SetAttr(WATCHDOG_TIMEOUT,(void *)&wdt) ;
	delay_ms(250);
	print_radio_error_state(ret_code);
	sprintf(buffer_radio_param, "\r\nConfiguring Radio Watch Dog Timeout %ld\r\n", wdt);
	usart_print(&usart1, buffer_radio_param);
}

/* Transmit LoRa Radio Uplink */
void radio_transmit_uplink(uint8_t *data, uint16_t len)
{
	//rintf("[Transmit Uplink] ") ;
	//print_array(data, len) ;
	RadioError_t radioStatus ;
	RadioTransmitParam_t RadioTransmitParam ;
	RadioTransmitParam.bufferLen = len ;
	RadioTransmitParam.bufferPtr = data ;
	radioStatus = RADIO_Transmit(&RadioTransmitParam) ;
	while (RADIO_Transmit(&RadioTransmitParam) == ERR_NONE) {
	}
	switch(radioStatus)
	{
		case ERR_NONE:
		usart_print(&usart1, "Radio Transmit Success \r\n") ;
		led_success();
		break;
		case ERR_DATA_SIZE:
		usart_print(&usart1, "Error Data Size \r\n") ;
		break;
		default:
		usart_print(&usart1, "Radio busy \r\n") ;
		break;
	}
}
/* LoRa Radio exit from Receive Mode */
void radio_exit_receive_mode(void)
{
	RadioReceiveParam_t radioReceiveParam ;
	radioReceiveParam.action = RECEIVE_STOP ;
	if (RADIO_Receive(&radioReceiveParam) == 0) {
		usart_print(&usart1, "Radio Exit Receive mode\r\n");
		LED_On(LED0);
		delay_ms(250);
	}
	RadioState_t ret_code = RADIO_GetState();
	print_radio_state(ret_code);
}
/* LoRa Radio enter into Receive Mode */
void radio_enter_receive_mode(void)
{
	RadioReceiveParam_t radioReceiveParam ;
	uint32_t rxTimeout = 0 ;	// forever
	radioReceiveParam.action = RECEIVE_START ;
	radioReceiveParam.rxWindowSize = rxTimeout ;
	if (RADIO_Receive(&radioReceiveParam) == 0) {
		usart_print(&usart1, "Radio in Receive mode\r\n");
	}
	RadioState_t ret_code = RADIO_GetState();
	print_radio_state(ret_code);
	wdt_reset_count();
}

/* Uplink/Downlink Callback */
void appData_callback(void *appHandle, appCbParams_t *appdata)
{
	StackRetStatus_t status = LORAWAN_INVALID_REQUEST ;
	char buffer_radio_receive[128];
	if (appdata->evt == LORAWAN_EVT_RX_DATA_AVAILABLE)
	{
		// Downlink Event
		status = appdata->param.rxData.status ;
		switch(status)
		{
			case LORAWAN_RADIO_SUCCESS:
			{
				uint8_t dataLength = appdata->param.rxData.dataLength ;
				uint8_t *pData = appdata->param.rxData.pData ;
				if((dataLength > 0U) && (NULL != pData))
				{
					int8_t rssi_value, snr_value ;
					RADIO_GetAttr(PACKET_RSSI_VALUE, &rssi_value) ;
					RADIO_GetAttr(PACKET_SNR, &snr_value) ;
					usart_print(&usart1, ">> Payload received: ");
					usart_printlen(&usart1, pData, dataLength);
					//print_array(pData, dataLength) ;
					usart_write_buffer_job(&usart1, pData, dataLength);
					
					sprintf(buffer_radio_receive,"RSSI Value: %d\r\n", rssi_value);
					usart_print(&usart1, buffer_radio_receive);
					
					sprintf(buffer_radio_receive,"SNR Value: %d",snr_value);
					usart_print(&usart1, buffer_radio_receive);
					usart_print(&usart1, "\r\n*******************\r\n");
					
					if (dataLength == 5 && strncmp((char *)pData, ":PR\r\n", 5) == 0) {
						led_success();
					}
					else {
						led_fail();
					}
					//radio_enter_receive_mode() ;
				}
			}
			break ;
			case LORAWAN_RADIO_NO_DATA:
			{
				usart_print(&usart1, "\n\rRADIO_NO_DATA \n\r");
			}
			break;
			case LORAWAN_RADIO_DATA_SIZE:
			usart_print(&usart1, "\n\rRADIO_DATA_SIZE \n\r");
			break;
			case LORAWAN_RADIO_INVALID_REQ:
			usart_print(&usart1, "\n\rRADIO_INVALID_REQ \n\r");
			break;
			case LORAWAN_RADIO_BUSY:
			usart_print(&usart1, "\n\rRADIO_BUSY \n\r");
			break;
			case LORAWAN_RADIO_OUT_OF_RANGE:
			usart_print(&usart1, "\n\rRADIO_OUT_OF_RANGE \n\r");
			break;
			case LORAWAN_RADIO_UNSUPPORTED_ATTR:
			usart_print(&usart1, "\n\rRADIO_UNSUPPORTED_ATTR \n\r");
			break;
			case LORAWAN_RADIO_CHANNEL_BUSY:
			usart_print(&usart1, "\n\rRADIO_CHANNEL_BUSY \n\r");
			break;
			case LORAWAN_INVALID_PARAMETER:
			usart_print(&usart1, "\n\rINVALID_PARAMETER \n\r");
			break;
			default:
			sprintf(buffer_radio_receive,"UNKNOWN ERROR %d\r\n",status );
			usart_print(&usart1, buffer_radio_receive);
			break ;
		}
	}
	else if(appdata->evt == LORAWAN_EVT_TRANSACTION_COMPLETE)
	{
		// Uplink Event
		switch(status = appdata->param.transCmpl.status)
		{
			case LORAWAN_SUCCESS:
			case LORAWAN_RADIO_SUCCESS:
			usart_print(&usart1, "Transmission success\r\n");
			break ;
			case LORAWAN_RADIO_NO_DATA:
			usart_print(&usart1, "\r\nRADIO_NO_DATA\r\n");
			break ;
			case LORAWAN_RADIO_BUSY:
			usart_print(&usart1, "\r\nRADIO_BUSY\r\n");
			break ;
			default:
			break ;
		}
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
	LED_Off(LED_0_PIN);
	LED_Off(LED_1_PIN);
	LED_Off(LED_2_PIN);
	delay_ms(1000);
	LED_On(LED_0_PIN);
	LED_On(LED_1_PIN);
	LED_On(LED_2_PIN);
	delay_ms(250);
	LED_Off(LED_0_PIN);
	LED_Off(LED_1_PIN);
	LED_Off(LED_2_PIN);
	delay_ms(1000);
	LED_On(LED_0_PIN);
	LED_On(LED_1_PIN);
	LED_On(LED_2_PIN);
}

void led_test(void) {
	LED_Off(LED_0_PIN);
	delay_ms(1000);
	LED_On(LED_0_PIN);
	delay_ms(250);
	LED_Off(LED_0_PIN);
	delay_ms(1000);
	LED_On(LED_0_PIN);
}
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

	while (usart_init(&usart1,SERCOM0, &config_usart) != STATUS_OK) {
	}
	usart_enable(&usart1);
}

void configure_usart_callbacks(void) {
	//usart_register_callback(&usart1, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart1, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	//usart_enable_callback(&usart1, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart1, USART_CALLBACK_BUFFER_RECEIVED);
}

static void usart_write_callback(struct usart_module *const usart_module) {
	LED_Toggle(LED_1_PIN);
}

static void usart_read_callback(struct usart_module *const uart_module) {
	
	rx_buffer[rx_index] = '\0';
	rx_index = 0;
	//usart_write_buffer_job(&uart1, (uint8_t *)AT_UNKNOWN, strlen(AT_UNKNOWN));
	usart_read_buffer_job(&usart1, rx_buffer, RX_BUFFER_SIZE);
	// Execute the AT command
	uart_event((const char *)rx_buffer);
}

void uart_event(const char *command) {
	if (strcmp(command, "at+send=lorap2p:3A5052DC0D\r\n") || strcmp(command, "at+send=lorap2p:3A5052DC0D")) {
		usart_print(&usart1, ":PR\r\n");
	}
	else if (strcmp(command, "AT") == 0 || strcmp(command, "AT\r\n") == 0 ) {
		usart_write_buffer_job(&usart1, (uint8_t *)AT_OK, strlen(AT_OK));
	}
	else {
		usart_write_buffer_job(&usart1, (uint8_t *)AT_ERROR, strlen(AT_ERROR));
	}
	usart_print(&usart1, "5555\r\n");
}

void usart_print(struct usart_module *const _uart, const char *str) {
	uint8_t i;
	if (_uart == NULL || str == NULL) {
		return;
	}
	
	for ( i = 0; str[i] != '\0'; i++) {
		uint16_t current_char = str[i];
		while (usart_write_job(_uart, &current_char) == STATUS_BUSY) {
		}
	}
}

void usart_printlen(struct usart_module *const _uart, const char *str, size_t len) {
	uint8_t i;
	if (_uart == NULL || str == NULL) {
		return;
	}
	for ( i = 0; i < len; ++i) {
		if (str[i] == '\0') {
			break;
		}
		uint8_t current_char = (uint8_t)str[i];
		while (usart_write_job(_uart, &current_char) == STATUS_BUSY) {
		}
	}
}

void print_radio_state(RadioState_t radio_state) {
	usart_print(&usart1, "\r\n***** Radio State *****\r\n");
	switch(radio_state) {
		case RADIO_STATE_SLEEP :
		usart_print(&usart1, "Radio State : Sleep\r\n");
		break;
		case RADIO_STATE_IDLE :
		usart_print(&usart1, "Radio State : Idle\r\n");
		break;
		case RADIO_STATE_TX :
		usart_print(&usart1, "Radio State : TX\r\n");
		break;
		case RADIO_STATE_SCAN :
		usart_print(&usart1, "Radio State : Scan\r\n");
		break;
		case RADIO_STATE_RX :
		usart_print(&usart1, "Radio State : RX\r\n");
		break;
		case RADIO_STATE_RX_BUSY :
		usart_print(&usart1, "Radio State : RX Busy\r\n");
		break;
		case RADIO_STATE_CAD :
		usart_print(&usart1, "Radio State : CAD\r\n");
		break;
		default:
		usart_print(&usart1, "Radio State : Unknown\r\n");
		break;
	}
}

void print_radio_error_state(RadioError_t radio_error) {
	usart_print(&usart1, "\r\n***** Radio Error *****\r\n");
	switch(radio_error) {
		case ERR_NONE :
		usart_print(&usart1, "RadioError_t : OK\r\n");
		break;
		case ERR_NO_DATA :
		usart_print(&usart1, "RadioError_t : No Data\r\n");
		break;
		case ERR_DATA_SIZE :
		usart_print(&usart1, "RadioError_t : Data Size\r\n");
		break;
		case ERR_INVALID_REQ :
		usart_print(&usart1, "RadioError_t : Invalid REQ\r\n");
		break;
		case ERR_RADIO_BUSY :
		usart_print(&usart1, "RadioError_t : Radio Busy\r\n");
		break;
		case ERR_OUT_OF_RANGE :
		usart_print(&usart1, "RadioError_t : Out Of Range\r\n");
		break;
		case ERR_UNSUPPORTED_ATTR :
		usart_print(&usart1, "RadioError_t : Unsupported Param\r\n");
		break;
		case ERR_CHANNEL_BUSY :
		usart_print(&usart1, "RadioError_t : Channel Busy\r\n");
		break;
		default:
		usart_print(&usart1, "RadioError_t : Unknown State\r\n");
		break;
	}
}

static void eui_addr_read(void)
{
	uint8_t data[6]; ///48bit
	uint8_t status = 0;
	char hex_str[16] = {0};
	char buffer_eui[128]  = {0};
	status = eeprom_eui_read((uint8_t*)data);
	
	for (int i = 0; i < sizeof(data); i++) {
		sprintf(&hex_str[i * 2], "%02X", data[i]);
	}

	usart_print(&usart1, "EUI in Hex: ");
	usart_print(&usart1, hex_str); // Print the hex string
	usart_print(&usart1, "\r\n");
	
	/* If EDBG doesnot have DEV EUI, the read value will be of all 0xFF,
	Set devEUI in conf_app.h in that case */
	if(status)
	{
		
		memset(devEui,0,sizeof(devEui));
		/* Set EUI Addr in EDBG if there */
		//first 2byte value is zero + last 6 bytes value is EUI Address
		memcpy(devEui+2, data, sizeof(data));
		
		/* Set Dev Addr in EDGG of EUI)*/
		//last 4 bytes(32bits) value of EUI Address
		memcpy(&devAddr, data+2, 4);
		devAddr = swap32(devAddr);

	}
}

void wdt_init(void) {
	struct wdt_conf config_wdt;
	wdt_get_config_defaults(&config_wdt);
	config_wdt.timeout_period = CONF_WDT_TIMEOUT_PERIOD;
	wdt_set_config(&config_wdt);
}

/*
* End Transmit file
*/