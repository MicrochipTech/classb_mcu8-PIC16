/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        SSL_MarchBRamTest.c
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
* CT                08/15/2008  First release of source file
* Mike Cahill		11/11/2010	Updated for HI-TECH v9.80
* MVL				02/22/2011	Modified for HI-TECH v9.80 & v9.81
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/


#include "SSL_MarchBRamTest.h"

#define MAX_MEM ( MARCHB_RAM_STOP_ADDRESS - MARCHB_RAM_START_ADDRESS +1 )
	volatile unsigned char *data = MARCHB_RAM_START_ADDRESS;
	volatile unsigned char i;

void main()
{
	volatile unsigned char testResult;

	testResult = SSL_RAMtest_MarchB();


	if (testResult == MARCHB_RAM_TEST_PASS)
    {
		asm("nop");		/* Test pass */
	}
	else
    {
		asm("nop");		/* Test not pass */
	}
}
       

/*******************************************************************
* Description  : This function implements the March B test. It is a complete and irredundant test
* capable of detecting stuck-at faults , linked coupling Idempotent faults or Inversion Coupling
* faults. This test is of complexity 17n( Where n is the number of bits).
* Input:
*     *data  		:  pointer to an array declared in user memory
 *     MAX_MEM 	:  value indicating the number of bytes to test
* Return Values:
*     MARCHB_RAM_TEST_FAIL 	:  return value = 0
*     MARCHB_RAM_TEST_PASS 	:  return value = 1
*                                                                 
*******************************************************************/
unsigned char  SSL_RAMtest_MarchB()
{
    volatile unsigned char testResult;

//***********************************************
	for (  i=0;i<=(MAX_MEM-1);i++)
		{
			data[i]=0x00;
		}
//***********************************************
	for (  i=0;i<=(MAX_MEM-1);i++)
	{
		testResult = Rd0Wr1Rd1Wr0Rd0Wr1 (&data[i]);
		if ( !testResult)
             return MARCHB_RAM_TEST_FAIL;
	}
//***********************************************
	for (  i=0;i<=(MAX_MEM-1);i++)
	{
		testResult = Rd1Wr0Wr1 (&data[i]);
		if ( !testResult)
             return MARCHB_RAM_TEST_FAIL;
	}
//***********************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		testResult = Rd1Wr0Wr1Wr0 (&data[i]);
		if ( !testResult)
             return MARCHB_RAM_TEST_FAIL;
	}
		// do this once more time for zero
		testResult = Rd1Wr0Wr1Wr0 (&data[0]);
		if ( !testResult)
             return MARCHB_RAM_TEST_FAIL;
//***********************************************
	for (i=(MAX_MEM-1); i > 0;i--)
	{
		testResult = Rd0Wr1Wr0 (&data[i]);
		if ( !testResult)
             return MARCHB_RAM_TEST_FAIL;
	}
		// do this once more time for zero
		testResult = Rd0Wr1Wr0 (&data[0]);
		if ( !testResult)
             return MARCHB_RAM_TEST_FAIL;
//***********************************************
	return MARCHB_RAM_TEST_PASS; 
}




//***********************************************
unsigned char Rd0Wr1Rd1Wr0Rd0Wr1 ( volatile unsigned char *test_data )
{
volatile unsigned char r;
		// read zero , write 1
		for (r=0; r<8;r++)
		{
			if ( (*test_data >>r ) & 0x01)
				return FAIL;
			else
				*test_data=*test_data | (1<<r);
		}
		// read one write zero
		for (r=0; r<8;r++)
		{
			if ( ((*test_data >>r ) & 0x01) != 0x01)
				return FAIL;
			else
				*test_data=*test_data & ~(1<<r);
		}
		// read zero , write 1
		for (r=0; r<8;r++)
		{
			if ( (*test_data >>r ) & 0x01)
				return FAIL;
			else
				*test_data=*test_data | (1<<r);
		} 
		return PASS;
}




//***********************************************
unsigned char Rd1Wr0Wr1( volatile unsigned char *test_data)
{
volatile unsigned char r;
		// read one write zero write one
		for (r=0; r<8;r++)
		{
			if ( ((*test_data >>r ) & 0x01) != 0x01)
				return FAIL;
			else
			{
				*test_data=*test_data & ~(1<<r);
				*test_data=*test_data | (1<<r);
			}	
		}

	return PASS;
} 




//***********************************************
unsigned char Rd1Wr0Wr1Wr0( volatile unsigned char *test_data)
{
volatile unsigned char r;
		// read one write zero write one, write 1 , write 0
		for (r=0; r<8;r++)
		{
			if ( ((*test_data >>r ) & 0x01) != 0x01)
				return FAIL;
			else
			{
				*test_data=*test_data & ~(1<<r);
				*test_data=*test_data | (1<<r);
				*test_data=*test_data & ~(1<<r);
			}	
		}

	return PASS;
} 
unsigned char Rd0Wr1Wr0 (volatile unsigned char *test_data)
{
volatile unsigned char r;
		// read zero , write 1, write 0
		for (r=0; r<8;r++)
		{
			if ( (*test_data >>r ) & 0x01)
				return FAIL;
			else
			{	
				*test_data=*test_data | (1<<r);
				*test_data=*test_data & ~(1<<r);
			}
		}
	return PASS;
}
