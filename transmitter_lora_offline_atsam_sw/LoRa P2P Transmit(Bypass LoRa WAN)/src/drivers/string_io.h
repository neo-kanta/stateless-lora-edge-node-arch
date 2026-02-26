/*
 * string_io.h
 *
 * Created: 21/12/2564 11:46:16
 *  Author: parisa
 */ 
//last Update 22062022

#ifndef STRING_IO_H_
#define STRING_IO_H_

#include <stdint.h>

/****************************** DEFINES ***************************************/
typedef union _StringFlag{
	struct BitString{
		//uint8_t rdComp:1; 
		uint8_t Received:1;
		uint8_t Header:1;
		uint8_t Ender:1;
		uint8_t Busy:1;
		uint8_t Oversize:1;		
		uint8_t tx:1;
		uint8_t rx:1;
		
	}Bit;
	uint8_t Byte;	
}StringFlag;

typedef struct _StringIO_t
{
	StringFlag* Flag;
	
	uint8_t* rdBuffer;
	uint8_t* rdDigit;
	
	uint8_t* cntRdBuffer;	uint8_t rdSize;	uint8_t* cntRdDigit;
	
	uint8_t* rdComp;
	uint8_t instance;

	int (*getChar)(void);
	void (*putChar)(uint8_t data);
	void (*putData)(uint8_t*, uint16_t Size);
	void (*clrChar)(void);

}StringIO_t;

 /*********************************************************************//*
  example string io initialize 
 **********************************************************************//*

 * \note The following takes SAM3X configuration for example, other devices have similar
 * configuration, but their parameters may be different, refer to corresponding header files.
 *
 *\code
 *	StringIO_t Uart;
 *  
 *	uint8_t serialDataBuffer[255];
 *	uint8_t serialDigit[10];
 *	uint8_t serialCntBuffer;
 *	uint8_t serialCntDigit;
 *	uint8_t serialSize;
 *	uint8_t serialComp;
 *	StringFlag serialFlag;
 * 
 * uint8_t usbDataBuffer[64];
 \endcode
 *
 *
 * Add to application initialization:
 *
 *\ code	
 *	Uart.rdBuffer = serialDataBuffer;
 *	Uart.rdDigit = serialDigit;
 *	Uart.cntRdBuffer = &serialCntBuffer;
 *	Uart.cntRdDigit = &serialCntDigit;
 *	Uart.rdSize = 255;
 *	Uart.rdComp = &serialComp;
 *	Uart.Flag = &serialFlag;
 *	
 *	Uart.getChar = sio2host_getchar_nowait;
 *	Uart.instance = 0;
 *	
 *	clearStringBuffer(Uart);
 \endcode
 * 
 */
 
 /*********************************************************************//*
 \brief      
 \param[in]  
 ************************************************************************/
void writeUart(StringIO_t st_instance, uint8_t* data, uint16_t size);

/*********************************************************************//*
 \brief      
 \param[in]  
***********************************************************************/
int readUart(StringIO_t st_instance);

/*********************************************************************//*
 \brief      
 \param[in]  
************************************************************************/
void clrUart(StringIO_t st_instance);

/*********************************************************************//*
 \brief      
 \param[in]  
 ************************************************************************/
int detectStringBuffer(StringIO_t st_instance, uint8_t* bufData, uint8_t* headerData, uint8_t* enderData);

/*********************************************************************//*
 \brief     Clear String io Buffer
 \param[in]  
 ************************************************************************/
void clearStringBuffer(StringIO_t st_instance);

/*********************************************************************//*
 \brief		Detect Ender string      
 \param[in]  
 ************************************************************************/
uint8_t detectEnder(StringIO_t st_instance, uint8_t* ender);

/*********************************************************************//*
 \brief		Detect Header string      
 \param[in]  
 ************************************************************************/
uint8_t detectHeader(StringIO_t st_instance, uint8_t* header);

/*********************************************************************//*
 \brief    
 \param[in]  
 ************************************************************************/
uint8_t detectFreeBus(StringIO_t st_instance, uint8_t delay_ms, uint8_t* buffer);

/*********************************************************************//*
 \brief      Function to Print array of characters
 \param[in]  *array  - Pointer of the array to be printed
 \param[in]   length - Length of the array
 ************************************************************************/
void print_array (uint8_t *data, uint8_t length);

#endif /* STRING_IO_H_s */