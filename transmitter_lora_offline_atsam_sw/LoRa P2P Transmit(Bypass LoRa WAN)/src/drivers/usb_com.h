/*
 * usb_com.h
 *
 * Created: 21/12/2564 11:40:20
 *  Author: parisa
 */ 


#ifndef USB_COM_H_
#define USB_COM_H_

char pstrUSB[255];	

/*********************************************************************//*
 \brief       to send string thru SAMR34 Target USB port
 ************************************************************************/
void printf_usb(const char* str);

/*********************************************************************//*
 \brief      sprintf version for directing output to SAMR34 Target USB_CDC port
 \param[in]  *str  - Pointer of the string
 \param[in]
  ************************************************************************/
int printf_usb_format(char * str, const char * format, ... ) __attribute__ ((format (gnu_printf, 2, 3)));

/*********************************************************************//*
 \brief      sprintf version for directing output to SAMR34 Target USB_CDC port
 \param[in]
  ************************************************************************/
int sprintf_usb(const char * format, ... ) __attribute__ ((format (gnu_printf, 1, 2)));


/*********************************************************************//*
 \brief      Function to Print array of characters
 \param[in]  *array  - Pointer of the array to be printed
 \param[in]   length - Length of the array
 ************************************************************************/
void print_usb_array (uint8_t *array, uint8_t length);

int usb_getchar(void);

#endif /* USB_COM_H_ */