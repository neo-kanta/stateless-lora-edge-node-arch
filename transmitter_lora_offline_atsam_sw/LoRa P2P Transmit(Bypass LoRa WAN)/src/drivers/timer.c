/*
 * timer.c
 *
 * Created: 24/5/2565 9:28:59
 *  Author: parisa
 */ 
//last Update 22062022

#include "asf.h"

#include "tc.h"
#include "tc_interrupt.h"


#include "conf_tc.h"
#include "timer.h"

#include "utils.h"

/******************************** MACROS ***************************************/
#define CONF_TC1_VALUE_COUNT			(4000u) //1ms ((8MHz/DIV2))/1ms)-1 = (4MHz/1000Hz) -1 = 3999
#define CONF_TC2_VALUE_COUNT			(0u)	//2s
/************************** GLOBAL VARIABLES ***********************************/
struct tc_module tc1_module;
struct tc_module tc2_module;

timerFlag_t timerFlag;

/************************** EXTERN VARIABLES ***********************************/


/************************** FUNCTION PROTOTYPES ********************************/
static void timer1_init(void);
static void timer2_init(void);
static void timer1_cb(void);
static void timer2_cb(void);


void timer_app_init(void){
	timer1_init();
	timer2_init();
}

static void timer1_init(void){
	
	/* Create and initialize config structure */
	struct tc_config tc1_config;
	tc_get_config_defaults(&tc1_config);
	
	tc1_config.clock_source = CONF_TC1_GCLK_SOURCE;
	tc1_config.counter_size = CONF_TC1_COUNTER;
	tc1_config.clock_prescaler = CONF_TC1_PRESCALER;
	tc1_config.count_direction = CONF_TC1_DIRECTION;	
	
// 	tc1_config.counter_16_bit.compare_capture_channel\
// 	[TC_COMPARE_CAPTURE_CHANNEL_0] = 0x7FFF;
	tc1_config.counter_16_bit.value = CONF_TC1_VALUE_COUNT;

	
			
	tc_init(&tc1_module, CONF_TC1_MODULE, &tc1_config);
	
	/* Setup callbacks */
	tc_register_callback(&tc1_module, timer1_cb, TC_CALLBACK_OVERFLOW);
	//tc_register_callback(&tc1_module, timer_app_cb_1, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc1_module, TC_CALLBACK_OVERFLOW);
	//tc_enable_callback(&tc1_module, TC_CALLBACK_CC_CHANNEL0);
	
	tc_enable(&tc1_module);	
	
}

static void timer2_init(void){
	
	/* Create and initialize config structure */
	struct tc_config tc2_config;
	tc_get_config_defaults(&tc2_config);
	
	tc2_config.clock_source = CONF_TC2_GCLK_SOURCE;
	tc2_config.counter_size = CONF_TC2_COUNTER;
	tc2_config.clock_prescaler = CONF_TC2_PRESCALER;
	tc2_config.count_direction = CONF_TC2_DIRECTION;	
	
// 	tc2_config.counter_16_bit.compare_capture_channel\
// 	[TC_COMPARE_CAPTURE_CHANNEL_0] = 0x7FFF;
	tc2_config.counter_16_bit.value = CONF_TC2_VALUE_COUNT;

	
			
	tc_init(&tc2_module, CONF_TC2_MODULE, &tc2_config);
	
	/* Setup callbacks */
	tc_register_callback(&tc2_module, timer2_cb, TC_CALLBACK_OVERFLOW);
	//tc_register_callback(&tc2_module, timer2_app_cb_2, TC_CALLBACK_CC_CHANNEL0);
	tc_enable_callback(&tc2_module, TC_CALLBACK_OVERFLOW);
	//tc_enable_callback(&tc2_module, TC_CALLBACK_CC_CHANNEL0);
	
	tc_enable(&tc2_module);	
	
}

static void timer1_cb(void){
	
	/* set timer count init */
	tc_set_count_value(&tc1_module,CONF_TC1_VALUE_COUNT);
	
	timer1_app_cb();
}

static void timer2_cb(void){
	
	timer2_app_cb();
}

