/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         CLASSB_CPUPCRegisters.c
* Dependencies:     CLASSB_CPUPCRegisters.h
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains functions to check CPU registers.
*
**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and
 * any derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors
 * retain all ownership and intellectual property rights in the
 * accompanying software and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products. You
 * agree that you are solely responsible for testing the software and
 * determining its suitability. Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE,
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *************************************************************************
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*CT                 09/15/2008  First release of source file
*Mike Cahill        11/11/2010	Updated for HI-TECH v9.80
*MVL                02/22/2011	Modified for HI-TECH v9.80 & v9.81
*EA                 01/20/2013	Updated to XC-8
*Corey Simoncic     06/17/2013  Modified for XC8
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version 		3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "CLASSB_CPURegistersTest.h"

unsigned char tempBSR;

/********************************************************************************************
* Function Name: SSL_8bit_CPU_RegisterTest      
* 
* Description  : The "SSL_8bit_CPU_RegisterTest" function tests the CPU W register for stuck at 
* faults, insuring that bits in the registers are not stuck at a value. This test can be done 
* by successively doing a write, read and check of 0x55 & 0xAA value into the W register.
* The Register is saved into RAM before the test is performed and restored after the test is done.
*
* Input        : None
*                
* Returns      : return value = 0 Test FAIL 
*                return value = 1 Test PASS 
*********************************************************************************************/


CLASSBRESULT CLASSB_CPURegistersTest()
{
    tempBSR = BSR;

/*****************************************************************************
*   Test WREG
******************************************************************************/
    // Test WREG with 0x55
    asm("MOVLW 0x55");
    asm("XORLW 0x55");

    if (WREG != 0)
    {
        return CLASSB_TEST_FAIL;
    }

    // Test WREG with 0xAA
    asm("MOVLW 0xAA");
    asm("XORLW 0xAA");

    if (WREG != 0)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test WREG_SHAD
******************************************************************************/
    //Test WREG_SHAD with 0xAA
    WREG_SHAD = 0xAA;
    if (WREG_SHAD != 0xAA)
    {
        return CLASSB_TEST_FAIL;
    }
    //Test WREG_SHAD with 0x55
    WREG_SHAD = 0x55;
    if (WREG_SHAD != 0x55)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test BSR_SHAD
******************************************************************************/
    //Test BSR_SHAD with 0x0A
    BSR_SHAD = 0x0A;
    if (BSR_SHAD != 0x0A)
    {
        return CLASSB_TEST_FAIL;
    }
    //Test BSR_SHAD with 0x15
    BSR_SHAD = 0x15;
    if (BSR_SHAD != 0x15)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test FSR0L_SHAD
******************************************************************************/
    //Test FSR0L_SHAD with 0xAA
    FSR0L_SHAD = 0xAA;
    if (FSR0L_SHAD != 0xAA)
    {
        return CLASSB_TEST_FAIL;
    }
    //Test FSR0L_SHAD with 0x55
    FSR0L_SHAD = 0x55;
    if (FSR0L_SHAD != 0x55)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test FSR0H_SHAD
******************************************************************************/
    //Test FSR0H_SHAD with 0xAA
    FSR0H_SHAD = 0xAA;
    if (FSR0H_SHAD != 0xAA)
    {
        return CLASSB_TEST_FAIL;
    }
    //Test FSR0H_SHAD with 0x55
    FSR0H_SHAD = 0x55;
    if (FSR0H_SHAD != 0x55)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test FSR1L_SHAD
******************************************************************************/
    //Test FSR1L_SHAD with 0xAA
    FSR1L_SHAD = 0xAA;
    if (FSR1L_SHAD != 0xAA)
    {
        return CLASSB_TEST_FAIL;
    }
    //Test FSR1L_SHAD with 0x55
    FSR1L_SHAD = 0x55;
    if (FSR1L_SHAD != 0x55)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test FSR1H_SHAD
******************************************************************************/
    //Test FSR1H_SHAD with 0xAA
    FSR1H_SHAD = 0xAA;
    if (FSR1H_SHAD != 0xAA)
    {
        return CLASSB_TEST_FAIL;
    }
    //Test FSR1H_SHAD with 0x55
    FSR1H_SHAD = 0x55;
    if (FSR1H_SHAD != 0x55)
    {
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Save User Data in shadow registers
******************************************************************************/
    BSR_SHAD = tempBSR;
    FSR0L_SHAD = FSR0L;
    FSR0H_SHAD = FSR0H;
    FSR1L_SHAD = FSR1L;
    FSR1H_SHAD = FSR1H;

/*****************************************************************************
*   Test BSR
******************************************************************************/
    //Only BSR<4:0> are R/W
    //Test BSR with 0x0A
    asm("MOVLB 0x0A");
    asm("MOVLW 0x0A");
    asm("SUBWF BSR, W");
    if (WREG != 0x00)
    {
        UserRestore();
        return CLASSB_TEST_FAIL;
    }
    //Test BSR with 0x15
    asm("MOVLB 0x15");
    asm("MOVLW 0x15");
    asm("SUBWF BSR, W");
    if (WREG != 0x00)
    {
        UserRestore();
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   Test FSR
******************************************************************************/
    //Clear FSR
    FSR0 = 0;
    FSR1 = 0;
    //Test FSR0 with 0x5555
    FSR0 = 0x5555;
    if (FSR0 != 0x5555)
    {
        UserRestore();
        return CLASSB_TEST_FAIL;
    }
    //Test FSR0 with 0xAAAA
    FSR0 = 0xAAAA;
    if (FSR0 != 0xAAAA)
    {
        UserRestore();
        return CLASSB_TEST_FAIL;
    }
    //Test FSR1 with 0x5555
    FSR1 = 0x5555;
    if (FSR1 != 0x5555)
    {
        UserRestore();
        return CLASSB_TEST_FAIL;
    }
    //Test FSR1 with 0xAAAA
    FSR1 = 0xAAAA;
    if (FSR1 != 0xAAAA)
    {
        UserRestore();
        return CLASSB_TEST_FAIL;
    }

/*****************************************************************************
*   All tests passed.
*   Restore user data.
******************************************************************************/
    UserRestore();
    return CLASSB_TEST_PASS;
}

void UserRestore (void)
{
    FSR0L = FSR0L_SHAD;
    FSR0H = FSR0H_SHAD;
    FSR1L = FSR1L_SHAD;
    FSR1H = FSR1H_SHAD;
    BSR = BSR_SHAD;
}