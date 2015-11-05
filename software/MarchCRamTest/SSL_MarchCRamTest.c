/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        SSL_MarchCRamTest.c
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
* MVL				02/22/2011	Modified for HI-TECH v9.81 & HI-TECH v9.80
*
* Version 		1.02
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/

#include "SSL_MarchCRamTest.h"


#define MAX_MEM ( MARCHC_RAM_STOP_ADDRESS - MARCHC_RAM_START_ADDRESS +1 )
	volatile unsigned char *data = MARCHC_RAM_START_ADDRESS;
	volatile unsigned char i;

void main()
{
	volatile unsigned char testResult;

	testResult = SSL_RAMtest_MarchC();

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
*     This function that implements the MarchC test.
*     This test is a destructive test !!!
* Input:
*     data[]  :  pointer to an array declared in user memory
*     MAX_MEM :  value indicating the number of bytes to test
* Return Values:
*     FAIL :  return value = 0
*     PASS :  return value = 1
*                                                                 
*******************************************************************/
unsigned char  SSL_RAMtest_MarchC()
{
    volatile unsigned char testResult;


//***********************************************
	for (  i=0;i<=(MAX_MEM-1);i++)
	{	
		data[i]=0x00;
	}

//***********************************************
	for (i=0; i <= (MAX_MEM-1);i++)
	{
		testResult = ReadZeroWriteOne(&data[i]);
		if ( !testResult)
             return FAIL;
	}
//************************************************
	for (i=0; i <= (MAX_MEM-1);i++)
	{
		testResult = ReadOneWriteZero(&data[i]);
		if ( !testResult)
             return FAIL;
	}
//************************************************
	for (i=0; i <= (MAX_MEM-1);i++)
	{
		testResult = ReadZero(&data[i]);
		if ( !testResult)
             return FAIL;
	}
//***********************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		testResult = ReadZeroWriteOne(&data[i]);
		if ( !testResult)
             return FAIL;
	}
		// do this once more time for zero
		testResult = ReadZeroWriteOne (&data[0]);
		if ( !testResult)
             return FAIL;
//***********************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		testResult = ReadOneWriteZero(&data[i]);
		if ( !testResult)
             return FAIL;
	}
		// do this once more time for zero
		testResult = ReadOneWriteZero (&data[0]);
		if ( !testResult)
             return FAIL;
//***********************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		testResult = ReadZero(&data[i]);
		if ( !testResult)
             return FAIL;
	}
		// do this once more time for zero
		testResult = ReadZero(&data[0]);
		if ( !testResult)
             return FAIL;
//***********************************************



	return PASS;

} // End of function



/*******************************************************************
* Description:
*     This function tests bitwise if a bit is zero and replaces it
*     with one.
* Input:
*     *test_data :  pointer to the byte to test
* Return Values:
*     FAIL :  return value = 0
*     PASS :  return value = 1
*                                                                 
*******************************************************************/
unsigned char ReadZeroWriteOne(volatile unsigned char *test_data)
{
	volatile unsigned char r;
	for (r=0; r<8;r++)
	{
		if ( (*test_data >>r ) & 0x01)
		{	
			return FAIL;
		}
		else
		{
			*test_data=*test_data | (1<<r);
		}
	}
	return PASS;
}


/******************************************************************
* Description:
*     This function tests bitwise if a bit is one and replaces it
*     with zero.
* Input:
*     *test_data :  pointer to the byte to test
* Return Values:
*     FAIL :  return value = 0
*     PASS :  return value = 1
*                                                                
******************************************************************/
unsigned char ReadOneWriteZero(volatile unsigned char *test_data) 
{
	volatile unsigned char r;
	for (r=0; r<8;r++)
	{
		if ( ((*test_data >>r ) & 0x01) != 0x01)
		{
			return FAIL;
		}
		else
		{
			*test_data=*test_data & ~(1<<r);
			//bitclr(*test_data,r);
		}
	}
	return PASS;
}


/***********************************************************
* Description:
*     This function tests bitwise if all bits are zeros .
* Input:
*     *test_data : pointer to the byte to test
* Return Values:
*     FAIL :  return value = 0
*     PASS :  return value = 1
*                                                         
***********************************************************/
unsigned char ReadZero(volatile unsigned char *test_data ) 
{
	volatile unsigned char r;
	for (r=0; r<8;r++)
	{
		if ( *test_data != 0x00)
		{	
			return FAIL;
		}
	}
	return PASS; 
}
