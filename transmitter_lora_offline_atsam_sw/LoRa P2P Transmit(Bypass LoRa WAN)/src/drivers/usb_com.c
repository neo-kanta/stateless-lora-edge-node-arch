/*
 * usb_com.c
 *
 * Created: 21/12/2564 11:39:55
 *  Author: parisa
 */ 

#include <string.h>
#include <stdarg.h>
#include "sys.h"

#include "conf_usb.h"
#include "usb_com.h"
/******************************** MACROS ***************************************/

/************************** GLOBAL VARIABLES ***********************************/



/*********************************************************************//**
 \brief      printf version for directing output to SAMR34 Target USB_CDC port
*************************************************************************/
void serialUSBsend(const char* str);
/************************** EXTERN VARIABLES ***********************************/


/************************** FUNCTION PROTOTYPES ********************************/
//int sprintf_usb(char * str, const char * format, ... ) __attribute__ ((format (gnu_printf, 2, 3)));


/*********************************************************************//*
 \brief       to send string thru SAMR34 Target USB port
 ************************************************************************/
// Send string thru SAMR34 Target USB port
void serialUSBsend(const char* str)
{
	udi_cdc_multi_write_buf(1,str,strlen(str));
}

void printf_usb(const char* str)
{
	udi_cdc_multi_write_buf(1,str,strlen(str)); //serialUSBsend(str);
}

/*********************************************************************//*
 \brief      sprintf version for directing output to SAMR34 Target USB_CDC port
 \param[in]  *str  - Pointer of the string 
 \param[in]   
 ************************************************************************/
int printf_usb_format(char * str, const char * format, ... )
{
	va_list args;
	va_start(args, format);
	vsprintf(str,format, args);
	va_end(args);
	udi_cdc_multi_write_buf(1,str,strlen(str));
	return 0;
}

/*********************************************************************//*
 \brief      sprintf version for directing output to SAMR34 Target USB_CDC port
 \param[in]   
 ************************************************************************/
int sprintf_usb(const char * format, ... )
{
	va_list args;
	va_start(args, format);
	vsprintf(pstrUSB,format, args);
	va_end(args);
	udi_cdc_multi_write_buf(1,pstrUSB,strlen(pstrUSB));
	return 0;	
}

/*********************************************************************//*
 \brief      Function to Print array of characters
 \param[in]  *array  - Pointer of the array to be printed
 \param[in]   length - Length of the array
 ************************************************************************/
void print_usb_array (uint8_t *array, uint8_t length)
{
    printf_usb("0x");
    for (uint8_t i =0; i < length; i++)
    {
        printf_usb_format(pstrUSB,"%02x", array[i]);
 
    }
	
	printf_usb("\r\n");
}



int usb_getchar(void){
	
	if(udi_cdc_is_rx_ready())
	{
		return  udi_cdc_getc();
	}
	else{
		return -1;
	}
	
}