/*
 * at24mac402.c
 *
 * Created: 14/1/2565 15:16:25
 *  Author: parisa
 */ 

#include <stdint.h>
#include "asf.h"

#include "drivers/i2c_com.h"
#include "at24mac402.h"

/******************************** MACROS ***************************************/
#define EEPROM_ADDRESS			0x50	//EEPROM address
#define EXTENDED_ADDRESS		0x58	
#define SLAVE_BUFFER_SIZE		64

#define EUI_ADDRESS				0x9A
#define EUI_SIZE				6U
/************************** GLOBAL VARIABLES ***********************************/

i2c_com_t EEPROM;

uint8_t	slaveAddr;
uint8_t slaveBuffer[64];
uint8_t cntSlaveBuffer;
uint8_t flag;

/************************** EXTERN VARIABLES ***********************************/

void eeprom_init(void);

/************************** FUNCTION PROTOTYPES ********************************/
void eeprom_init(void){
	
	EEPROM.addrSlave = SLAVE_BUFFER_SIZE;
	EEPROM.buffer = slaveBuffer;
	EEPROM.cntBuffer = &cntSlaveBuffer;
	EEPROM.flag = &flag;
	
}

uint8_t eeprom_eui_read(uint8_t* data){
	
	eeprom_init();
	
	EEPROM.addrSlave = EXTENDED_ADDRESS;
	read_i2c_master(EEPROM, EUI_ADDRESS, data, EUI_SIZE);
	
	return 1;
}