/*
 * i2c_com.h
 *
 * Created: 13/1/2565 16:25:18
 *  Author: parisa
 */ 


#ifndef I2C_COM_H_
#define I2C_COM_H_

typedef struct _i2c_com_t
{
	uint8_t addrSlave;
	uint8_t* buffer;
	uint8_t* cntBuffer;
	uint8_t* flag;
	
}i2c_com_t;

/*********************************************************************//**
\brief		Reads data i2c
\param[in]  addr - Address Register(8 bytes)
\param[in]  data -
\param[in]  datalen -
*************************************************************************/
uint8_t read_i2c_master(i2c_com_t i2c_instance, uint8_t addr, uint8_t *data, int datalen);

#endif /* I2C_COM_H_ */