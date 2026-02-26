/*
 * i2c_com.c
 *
 * Created: 13/1/2565 16:25:06
 *  Author: parisa
 */ 

#include <stdint.h>
#include "string.h"
#include "asf.h"

#include "config/conf_i2c.h"
#include "drivers/i2c_com.h"

/******************************** MACROS ***************************************/
#define I2C_TIMEOUT						1000
#define I2C_SLAVE_WAIT_TIMEOUT			10
#define I2C_BUFFER_MAX_SIZE				256
/************************** GLOBAL VARIABLES ***********************************/
struct i2c_master_module i2c_master_instance;
struct i2c_master_packet master_packet;

/************************** EXTERN VARIABLES ***********************************/


/************************** FUNCTION PROTOTYPES ********************************/
static void configure_i2c_master(void)
{
	/* Create and initialize config structure */
	struct i2c_master_config config_i2c;
	i2c_master_get_config_defaults(&config_i2c);

	/* Change pins */
	config_i2c.pinmux_pad0  = CONF_I2C_SERCOM_PINMUX_PAD0;
	config_i2c.pinmux_pad1  = CONF_I2C_SERCOM_PINMUX_PAD1;
	config_i2c.buffer_timeout = I2C_TIMEOUT;

	/* Initialize and enable device with config */
	i2c_master_init(&i2c_master_instance, CONF_I2C_MODULE, &config_i2c);

	i2c_master_enable(&i2c_master_instance);
}


/*********************************************************************//**
\brief		Reads data i2c master
\param[in]  i2c_instance	
\param[in]  addr 			
\param[in,out] data		
\param[in]  datalen

  \return     
*************************************************************************/
 uint8_t read_i2c_master(i2c_com_t i2c_instance, uint8_t addr, uint8_t *data, int datalen)
{
	configure_i2c_master();
	uint32_t timeout = 0;
	
	/** Send the request token */
	master_packet.address         =	i2c_instance.addrSlave;
	master_packet.data_length     = 1;	
	master_packet.data            = &addr;
	master_packet.ten_bit_address = false;
	master_packet.high_speed      = false;
	master_packet.hs_master_code  = 0x0;
	while (i2c_master_write_packet_wait_no_stop(&i2c_master_instance, &master_packet) !=
			STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == I2C_TIMEOUT) {
			return false;
		}
	}
	/** Get the extension boards info */
	master_packet.data_length     = datalen;	
	master_packet.data            = i2c_instance.buffer;
	while (i2c_master_read_packet_wait(&i2c_master_instance, &master_packet) !=
			STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == I2C_TIMEOUT) {
			return false;
		}
	}

	memcpy(data, i2c_instance.buffer, datalen);
	
	//i2c_master_disable(&i2c_master_instance);
	
	
	return true;
}
   