/*
 * wdt_com.c
 *
 * Created: 20/5/2565 12:13:05
 *  Author: parisa
 */ 
#include "asf.h"

#include "conf_wdt.h"
#include "wdt_com.h"

/******************************** MACROS ***************************************/

/************************** GLOBAL VARIABLES ***********************************/


/************************** EXTERN VARIABLES ***********************************/


/************************** FUNCTION PROTOTYPES ********************************/
void wdt_init(void){

	struct wdt_conf config_wdt;
	wdt_get_config_defaults(&config_wdt);
	
	/* Set the desired configuration */
	//config_wdt.always_on = true;
	config_wdt.timeout_period       = CONF_WDT_TIMEOUT_PERIOD;
	//config_wdt.early_warning_period = CONF_WDT_EARLY_WARNING_PERIOD;
	
	wdt_set_config(&config_wdt);	
	
}

