/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

#ifdef __ROLLING_EXIT_SIGN__
	
	// Configuration Output of Rolling Exit Sign
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_0_MOTOR_PIN, &pin_conf);
	port_pin_set_output_level(LED_0_MOTOR_PIN, LED_0_MOTOR_INACTIVE);

	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_1_ALARM_PIN, &pin_conf);
	port_pin_set_output_level(LED_1_ALARM_PIN, LED_1_ALARM_INACTIVE);
	
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(GPIO_4_PIC_PIN, &pin_conf);
	port_pin_set_output_level(GPIO_4_PIC_PIN, GPIO_4_PIC_INACTIVE);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(GPIO_5_IA_PIN, &pin_conf);
	port_pin_set_output_level(GPIO_5_IA_PIN, GPIO_5_IA_INACTIVE);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(GPIO_6_IB_PIN, &pin_conf);
	port_pin_set_output_level(GPIO_6_IB_PIN, GPIO_6_IB_INACTIVE);
	
	// Configuraiton Input of Rolling Exit Sign
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(SWITCH_0_TEST_MOTOR_PIN, &pin_conf);
	
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(SWITCH_1_CW_PIN, &pin_conf);
	
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(SWITCH_2_CCW_PIN, &pin_conf);
	
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(GPIO_IN_7_ALARM_PIN, &pin_conf);

#endif /* __ROLLING_EXIT_SIGN__ */

	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(DIP_1_PIN, &pin_conf);
	
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(DIP_2_PIN, &pin_conf);
	
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(DIP_4_PIN, &pin_conf);
	
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(DIP_8_PIN, &pin_conf);
	
#ifdef RFSWITCH_ENABLE
	/* Configure RFSWITCH as output */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(RF_SWITCH_PIN, &pin_conf);
	port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);
#endif

#ifdef TCXO_ENABLE
	/* Configure TXPO PWR as output */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(TCXO_PWR_PIN, &pin_conf);
	port_pin_set_output_level(TCXO_PWR_PIN, TCXO_PWR_INACTIVE);
#endif
}