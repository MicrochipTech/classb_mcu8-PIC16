/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        MarchCMInusRamTest.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC16FXXX
* Compiler:        HI-TECH 9.81
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
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* CT                09/15/2008  First release of source file
* Mike Cahill		11/11/2010	Updated for HI-TECH v9.80
* MVL				02/22/2011	Modified for HI-TECH v9.81
*
* Version 		1.02
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/


#include "SSL_MarchCMinusRamTest.h"
    


#define MAX_MEM ( MARCHC_RAM_STOP_ADDRESS - MARCHC_RAM_START_ADDRESS +1 )
	volatile unsigned char *data = MARCHC_RAM_START_ADDRESS;
	volatile unsigned char i;

void main()
{
	volatile unsigned char testResult;

	testResult = SSL_RAMtest_MarchCMinus();

	if (testResult==MARCHC_RAM_TEST_PASS)
	{
		asm("nop");		/* Test pass */
	}
	else
	{
		asm("nop");		/* Test not pass */
	}
}
                                             

  /*******************************************************************
  * Description:
  *     This function that implements the MarchC Minus test.
  *     This test is a destructive test !!!
  * Input:
  *     data[]  :  pointer to an array declared in user memory
  *     MAX_MEM :  value indicating the number of bytes to test
  * Return Values:
  *     FAIL :  return value = 0
  *     PASS :  return value = 1
  *                                                                 
  *******************************************************************/
unsigned char  SSL_RAMtest_MarchCMinus()
{
    volatile unsigned char testResult;


//***********************************************
	for (  i=0;i<=(MAX_MEM-1);i++)
	{
		data[i]=0xAA;
	}
//***********************************************
	for (i=0; i <= (MAX_MEM-1);i++)
	{
		if(data[i] != 0xAA)
			return FAIL;
		else
			data[i]=0x55;
	}
//************************************************
	for (i=0; i <= (MAX_MEM-1);i++)
	{
		if(data[i] != 0x55)
			return FAIL;
		else
			data[i]=0xAA;
	}
//************************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		if(data[i] != 0xAA)
			return FAIL;
    	else
			data[i]=0x55;
 	}
		if(data[0] != 0xAA)
			return FAIL;
    	else
			data[0]=0x55;

//***********************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		if(data[i] != 0x55)
			return FAIL;
    	else
			data[i]=0xAA;
 	}
		if(data[0] != 0x55)
			return FAIL;
    	else
			data[0]=0xAA;
//***********************************************

	return PASS;

} // End of function



