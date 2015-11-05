/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        SSL_CpuRegisterTest.c
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
*CT             09/15/2008  First release of source file
*Mike Cahill	11/11/2010	Updated for HI-TECH v9.80
*MVL			02/22/2011	Modified for HI-TECH v9.80 & v9.81
*
* Version 		1.02
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/


#include "SSL_CpuRegisterTest.h"

	volatile unsigned char tempW, tempREG;



int main(void){
   

   volatile int CpuTestResult = 0;
   CpuTestResult =SSL_8bit_CPU_RegisterTest();

   if (CpuTestResult==CPU_REGISTER_TEST_PASS)
	{
		asm("nop");		/* Test pass */
	}
	else
	{
		asm("nop");		/* Test not pass */
	}
   return 0;
	}

/********************************************************************************************
* Function Name: SSL_8bit_CPU_RegisterTest      
* 
* Description  : The "SSL_8bit_CPU_RegisterTest" function tests the CPU W register for stuck up 
*  faults, insuring that bits in the registers are not stuck at a value. This test can be done 
* by successively doing a write, read and check of 0x55 & 0xAA value into the W register.
* The Register is saved into RAM before the test is performed and restored after the test is done.
*
* Input        : None
*                
* Returns      : return value = 0 Test FAIL 
*                return value = 1 Test PASS 
*********************************************************************************************/


 int SSL_8bit_CPU_RegisterTest()
{
  	unsigned char  cpuTestResult;  


   	// save the original value of W
		asm("MOVWF _tempW");

	// Test WREG with 0x55
		asm("MOVLW 0x55");
		asm("MOVWF _tempREG");

	
    	cpuTestResult= Test_Register(tempREG,0X55);
   	 	if(cpuTestResult==CPU_REGISTER_TEST_FAIL)
     	{	
 			asm("MOVF _tempW, W");
			return CPU_REGISTER_TEST_FAIL;
	 	}


	   // Test WREG with 0xAA    
			asm("MOVLW 0xAA");
			asm("MOVWF _tempREG");

	
	    cpuTestResult= Test_Register(tempREG,0XAA);
   	 	if(cpuTestResult==CPU_REGISTER_TEST_FAIL)
     	{	
 			asm("MOVF _tempW, W");
			return CPU_REGISTER_TEST_FAIL;
	 	}	 


	// POP the W-reg before return
 		asm("MOVF _tempW, W");

	//cpuTestResult = CPU_REGISTER_TEST_PASS ;
	return CPU_REGISTER_TEST_PASS;
}


unsigned int Test_Register(unsigned char tempREG, unsigned char Value)
{
   	if (tempREG != Value)
    {
		return CPU_REGISTER_TEST_FAIL;
    } 
	else   
     {
		return  CPU_REGISTER_TEST_PASS;
	 }
}

