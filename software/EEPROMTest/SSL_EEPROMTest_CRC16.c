/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        	SSL_EEPROMTest_CRC16.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       	PIC16FXXX
* Compiler:        	HI-TECH 9.81
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip PIC® microcontroller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author           	Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* CT		      	09/15/2008		Initial Revision
* Mike Cahill		11/11/10		Updated for HI-TECH v9.80
* MVL				02/22/2011		Updated with HI-TECH v9.81
*
* Version 		1.02
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "htc.h"
#include "SSL_EEPROM_CRC.h"

	unsigned int crc_Result=0;
	unsigned int partialCRC_Result=0;
    unsigned int tempCRC=0;
  	unsigned char Buffer;

int main(void)
{
    volatile unsigned char  startAddress;
    volatile unsigned char  endAddress;

    startAddress = EEPROM_STARTADDRESS;
    endAddress  =  EEPROM_ENDADDRESS;
	crc_Result=0;

    crc_Result = SSL_8bits_EEPROMtest_CRC16(startAddress,endAddress);
	if (crc_Result==crc_Result_CONST)
	{
	    asm("nop");		/* Test pass */
	}
	else
	{
	    asm("nop");		/* Test not pass */
	}
}

  /********************************************************************
  * Description:
  *     The " SSL_8bits_EEPROMtest_CRC16 " function Calculates
  *     the CRC of the data starting fromnAddress
  *     "EEPROM_STARTADDRESS" to "EEPROM_ENDADDRESS". This function
  *     \returns the final CRC Value.
  *     CRC-16 is used as a divisor.
  *     
  *     CRC-16 = 1 1000 0000 0000 0101= 8005(hex)
  * Input:
  *     			startAddress :  start Address from which the CRC needs to be
  *                    				 calculated
  *     			endAddress :    This address indicates the Final address upto
  *                     				which the CRC is calculated
  * Return Values:
  *     			crc_Result :  	Returns the final CRC result.
  *                                                                  
  ********************************************************************/

unsigned int SSL_8bits_EEPROMtest_CRC16(unsigned char startAddress,unsigned char endAddress)
{

  int i;
	
  for( i=0; endAddress != startAddress ; i++)
  	{
        partialCRC_Result=0;

		Buffer= eeprom_read(i);
          
        EEPROM_CRC_Checksum(); 

        crc_Result= crc_Result + partialCRC_Result;
        
        startAddress++;

 	}

 return crc_Result;

}
  /********************************************************************************************
  * Description  : Calculates the Partial Checksum for EEDATA_BYTES (One EEPROM location)
  *                                                
  *********************************************************************************************/
void EEPROM_CRC_Checksum()
{  
   int j;

   tempCRC=0;
		    
    for ( j = 8 ; j > 0 ; j-- )  {
        if ( (Buffer ^ (tempCRC)) & 0x80 ) {
           (tempCRC) = ((tempCRC)<< 1) ^ EEPROM_GEN_POLY ;
        }
        else {
            (tempCRC) <<= 1 ;
        }
        Buffer <<= 1 ;
    }
   		partialCRC_Result= partialCRC_Result+tempCRC;

}


