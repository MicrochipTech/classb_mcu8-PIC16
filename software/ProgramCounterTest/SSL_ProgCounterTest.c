/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        SSL_ProgCounterTest.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC16FXXX
* Compiler:        HI-TECH 9.81
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
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
* CT                09/27/2007  First release of source file
* Mike Cahill		11/11/2010	Updated for HI-TECH v9.80
* MVL				02/22/2011	Tested with HI-TECH v9.81
*
* Version 		1.02
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/


#include "SSL_ProgCounterTest.h"

unsigned int returnAddress,tempAddress;
int  ProgramCounterResult;


int main(void)
{
   	ProgramCounterResult = SSL_PCtest();
	if (ProgramCounterResult==PROGRAM_COUNTER_TEST_PASS)
	{
   		asm("nop");		/* Test pass */
	}
	else
	{
   		asm("nop");		/* Test not pass */
	}

   return 0;
}

unsigned int SSL_PCtest()
{
    //store Address of SSL_TestFunction1
 	tempAddress= (unsigned int  )&SSL_TestFunction1;
    // Branch to SSL_TestFunction1
    returnAddress=SSL_TestFunction1();
    // Test if the addresses match
	if( tempAddress != returnAddress)
  		 return	PROGRAM_COUNTER_TEST_FAIL;



   // store Address of SSL_TestFunction2
 	tempAddress= (long )&SSL_TestFunction2;

   // Branch to SSL_TestFunction2
 	returnAddress=SSL_TestFunction2();

    // Test if the addresses match
	if( tempAddress != returnAddress)
  		 return	PROGRAM_COUNTER_TEST_FAIL;


	return PROGRAM_COUNTER_TEST_PASS;
}



