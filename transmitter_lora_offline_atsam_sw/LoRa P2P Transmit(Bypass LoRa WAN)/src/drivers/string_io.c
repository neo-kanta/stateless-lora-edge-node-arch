/*
 * string_io.c
 *
 * Created: 21/12/2564 11:46:08
 *  Author: parisa
 */ 
//last Update 22062022

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "sio2host.h"


#include "drivers/string_io.h"


#if (CONF_USB_ENABLE == 1)
#include "drivers/usb_com.h"
#endif

/******************************** MACROS ***************************************/

/************************** GLOBAL VARIABLES ***********************************/



/************************** EXTERN VARIABLES ***********************************/


/************************** FUNCTION PROTOTYPES ********************************/

/*********************************************************************//*
 \brief      
 \param[in]  
***********************************************************************/
void writeUart(StringIO_t st_instance, uint8_t* data, uint16_t size)
{
	st_instance.Flag->Bit.tx = 1;
	st_instance.putData(data,size);
	st_instance.Flag->Bit.tx = 0;
}

/*********************************************************************//*
 \brief      
 \param[in]  
***********************************************************************/
int readUart(StringIO_t st_instance)
{
	return	st_instance.getChar();
}

/*********************************************************************//*
 \brief      
 \param[in]  
************************************************************************/
void clrUart(StringIO_t st_instance){
	
	st_instance.clrChar();
}

/*********************************************************************//*
 \brief      
 \param[in]  
 ************************************************************************/
int detectStringBuffer(StringIO_t st_instance, uint8_t* bufData, uint8_t* headerData, uint8_t* enderData)
{
	//uint8_t fin = 1;
 	//uint8_t status = 0; 
	uint16_t cntBufData;
	
	int rdData;
	
	/*check free bus and get data recieved*/
	if((-1) != (rdData = st_instance.getChar()))
	{
		st_instance.Flag->Bit.rx = 1;
		
		#ifdef USB_DEBUG_ENABLE
		sprintf_usb("<stio><det> string io %u working\r\n ",st_instance.instance);
		st_instance.Flag->Bit.Busy = 1;
		#endif
		
		/*Check Buffer overflow*/
		if((*st_instance.cntRdBuffer) < (st_instance.rdSize)){

			/* Save the character */
			*(st_instance.rdBuffer+(*st_instance.cntRdBuffer))= rdData;
			(*st_instance.cntRdBuffer)++;

			#ifdef USB_DEBUG_ENABLE
			sprintf_usb("<stio><det> -- rdBuffer %d, ",(*st_instance.cntRdBuffer));
			print_usb_array(st_instance.rdBuffer,(*st_instance.cntRdBuffer));
			#endif
			
			
			if((st_instance.Flag->Bit.Header)||(strlen((char*)headerData) == 0)||(*headerData == '0')){
			
				//if(detect_ender(st_instance.rdBuffer, (*st_instance.cntRdBuffer), enderData, strlen((char*)enderData))){
				if(detectEnder(st_instance,enderData)){
					//detEnder = true;
					st_instance.Flag->Bit.Ender = 1;
					#ifdef USB_DEBUG_ENABLE
					printf_usb("<stio><det> detected ender \r\n");
					#endif
				}
			}
			else{
			
				if(detectHeader(st_instance, headerData)){
					st_instance.Flag->Bit.Header = 1;
					
					/* delete data before header data and header data*/
					clearStringBuffer(st_instance);
					
					#ifdef USB_DEBUG_ENABLE
					sprintf_usb("<stio><det> header size %u, %u\r\n",strlen((char*)headerData),*headerData);
					printf_usb("<stio><det> detected header \r\n");
					#endif
				}
			}

			if(st_instance.Flag->Bit.Ender){
				st_instance.Flag->Bit.Ender = 0;			
				st_instance.Flag->Bit.Header = 0;
				st_instance.Flag->Bit.Received = 1;
				
				/* delete ender data*/
				(*st_instance.cntRdBuffer) -= strlen((char*)enderData);

				/*add \0 for end of str data*/				
				*(st_instance.rdBuffer+(*st_instance.cntRdBuffer))= 0x00;
				*(st_instance.rdDigit+(*st_instance.cntRdDigit)) = 0x00;
				(*st_instance.cntRdBuffer)++;
				cntBufData = (*st_instance.cntRdBuffer);
			
				(*st_instance.cntRdBuffer) = 0;
				*st_instance.rdComp = 1;


				memcpy(bufData, st_instance.rdBuffer, cntBufData);
			
				#ifdef USB_DEBUG_ENABLE
				printf_usb("<stio><det> \r\n");
				printf_usb("<stio><det> -- RdDigit:");
				print_usb_array(st_instance.rdDigit,(*st_instance.cntRdDigit));
				sprintf_usb("<stio><det> -- bufData %d: ",cntBufData);
				print_usb_array(bufData,cntBufData);
				printf_usb("<stio><det> detectStringBuffer success\r\n");
				printf_usb("<stio><det> --------------------------------------------\r\n");
				#endif /*USB_DEBUG_ENABLE*/

			}
		
		}
		else{
			/*data oversize*/
			//status = 1;
			st_instance.Flag->Bit.Oversize = 1;
			#ifdef USB_DEBUG_ENABLE
			printf_usb("<stio><det> detectStringBuffer error\r\n");
			#endif
		
			clearStringBuffer(st_instance);
			st_instance.Flag->Bit.Header  = 0;
		}		
	}
	else{
		
		st_instance.Flag->Bit.rx = 0;
		
		/*no data*/
		//status = 2;
		//fin = 1;
			
 		if(st_instance.Flag->Bit.Busy){
 			st_instance.Flag->Bit.Busy = 0;	
			 
			rdData = 0xff;
				
 			if(st_instance.Flag->Bit.Received){
				st_instance.Flag->Bit.Received = 0;
				st_instance.Flag->Bit.Ender = 0; 
				#ifdef USB_DEBUG_ENABLE
				printf_usb("<stio><det> detectStringBuffer complete\r\n");
				#endif
				
				//*st_instance.rdComp = 1;
				clearStringBuffer(st_instance);
 			}
			
 		}
		else{
						
			//fin = 0;
		}
		
	}
	

	return rdData; //fin;
		
}


/*********************************************************************//*
 \brief      
 \param[in]  
 ************************************************************************/
uint8_t detectEnder(StringIO_t st_instance, uint8_t* ender){
	
	uint8_t status = 0;
	uint8_t lenEnder = strlen((char*)ender);
	uint8_t cntEnder = 0;
	
	if((*st_instance.cntRdBuffer) >= lenEnder){
		status = 1;
				
		/*check the ender*/
		for(cntEnder = lenEnder; (cntEnder > 0U)&& status; cntEnder--){
		#ifdef USB_DEBUG_ENABLE
			sprintf_usb("<stio><rsf> -- %02x, %02x,lenEnder %d, cntEnder %d\r\n",*(st_instance.rdBuffer+(*st_instance.cntRdBuffer-1)),ender[lenEnder-1],lenEnder, cntEnder);
		#endif /*USB_DEBUG_ENABLE*/

			if( (*(st_instance.rdBuffer+(*st_instance.cntRdBuffer)-cntEnder) != ender[lenEnder-cntEnder])){
				status = 0;
			}
		}
	}	

	
	return status;
}

/*********************************************************************//*
 \brief      
 \param[in]  
 ************************************************************************/
uint8_t detectHeader(StringIO_t st_instance, uint8_t* header){
	
	uint8_t status = 0;
	uint8_t lenHeader = strlen((char*)header);
	uint8_t cntHeader = 0;
	
	#ifdef USB_DEBUG_ENABLE
	sprintf_usb("<stio><hdr> -- lenHeader %u, %u\r\n", lenHeader,*header);
	#endif
	
	if((*header == '0')||(lenHeader==0)){
		#ifdef USB_DEBUG_ENABLE
			sprintf_usb("<stio><hdr> -- no data %u\r\n", lenHeader);
		#endif /*USB_DEBUG_ENABLE*/
		status = 1;
	}	
	else{

		if((*st_instance.cntRdBuffer) >= lenHeader){
			status = 1;
		
			/*check the ender*/
			for(cntHeader = lenHeader; (cntHeader > 0U)&& status; cntHeader--){
				#ifdef USB_DEBUG_ENABLE
				sprintf_usb("<stio><rsf> -- %02x, %02x,lenEnder %d, cntEnder %d\r\n",*(st_instance.rdBuffer+(*st_instance.cntRdBuffer-1)),header[lenHeader-1],lenHeader, cntHeader);
				#endif /*USB_DEBUG_ENABLE*/

				if( (*(st_instance.rdBuffer+(*st_instance.cntRdBuffer)-cntHeader) != header[lenHeader-cntHeader])){
					status = 0;
				}
			}
		}		
	}
	return status;
}


/*********************************************************************//*
 \brief      
 \param[in]  
 ************************************************************************/
void clearStringBuffer(StringIO_t st_instance){
	
	*(st_instance.rdBuffer) = 0;
	*(st_instance.rdDigit) = 0;
	*st_instance.cntRdBuffer = 0;
	*st_instance.cntRdDigit = 0;

// 	#ifdef USB_DEBUG_ENABLE
// 	sprintf_usb("<stio><rsf> string io %u rdBuffer clear\r\n",st_instance.instance);
// 	#endif

}

//-------------------------------------------------------------------------
/*********************************************************************//*
 \brief    
 \param[in]  
 ************************************************************************/
uint8_t detectFreeBus(StringIO_t st_instance, uint8_t delay_ms, uint8_t* buffer){
	
	uint8_t size = 0;
	int rdData;
	bool over = 0;
	static uint8_t cntDelay = 0;
	
	
	if(((-1)!=(rdData = st_instance.getChar()))&&(!over)){
		
		st_instance.Flag->Bit.rx = 1;
		
		st_instance.Flag->Bit.Busy = 1;
		
		/*Check Buffer overflow*/
		if((*st_instance.cntRdBuffer) < st_instance.rdSize){
			/* Save the character */
			*(st_instance.rdBuffer+(*st_instance.cntRdBuffer)) = rdData;
			(*st_instance.cntRdBuffer)++;
		}
		else{
			st_instance.Flag->Bit.Oversize = 1;
			clearStringBuffer(st_instance);
			over = true;
			
		}
		
		cntDelay = 0;
		size = 0;
	}
	else{
		st_instance.Flag->Bit.rx = 0;
		
		if((*st_instance.cntRdBuffer)>0){
			if(cntDelay <= delay_ms){
				cntDelay++;
			}
			else{
				cntDelay = 0;
				size = (*st_instance.cntRdBuffer);
				memcpy(buffer, st_instance.rdBuffer, size);
				clearStringBuffer(st_instance);
			
			}
		}
		
	}

	
	return size;
}
/*********************************************************************//*
 \brief      Function to Print array of characters
 \param[in]  *array  - Pointer of the array to be printed
 \param[in]   length - Length of the array
 ************************************************************************/
void print_array (uint8_t *array, uint8_t length)
{
    printf("0x");
    for (uint8_t i =0; i < length; i++)
    {
        printf("%02x", *array);
        array++;
    }
    printf("\n\r");
}

