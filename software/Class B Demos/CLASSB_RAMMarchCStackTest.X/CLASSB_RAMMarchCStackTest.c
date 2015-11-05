/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         CLASSB_RAMMarchCTest.c
* Dependencies:     CLASSB_RAMMarchCTest.h
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains functions to RAM with MarchC Test.
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
* CT                09/15/2008  First release of source file
* Mike Cahill       11/11/2010	Updated for HI-TECH v9.80
* MVL               02/22/2011	Modified for HI-TECH v9.81
* EA                02/18/2012	Made corrections to March C Minus routine
* Corey Simoncic    04/19/2013  Updated for XC-8
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/

#include "CLASSB_RAMMarchCStackTest.h"

//Comment if used in conjunction with the March C test!

#ifndef MARCH_VARS
#define MARCH_VARS
unsigned char* CLASSB_MarchstartAddress @ 0x20;
unsigned char* CLASSB_MarchbufferAddress @ 0x22;
unsigned char* CLASSB_MARCHtempAddress @ 0x24;
unsigned char CLASSB_MarchLength @ 0x26 = 0;
#endif

unsigned char stackTestResult;

/***********************************************************
 *
 * Description:
 *    This function performs the March C test on the stack.
 *    If the March C Minus test is desired set minus = 1
 * Inputs:
 *    *CLASSB_MarchCbufferAddress    :   pointer to the location to store memory during test
 *    MARCHCMINUS                    :   designates the test is a March C Minus test.
 *
 * ********************************************************/
CLASSBRESULT CLASSB_RAMMarchCStackTest(void)
{
        volatile unsigned char i;

/*****************************************************************************
* Save Stack here.
* ***************************************************************************/
        *CLASSB_MarchbufferAddress = STKPTR;

        CLASSB_MarchbufferAddress++;

        for (WREG_SHAD = 0; WREG_SHAD <= 15; WREG_SHAD++)
        {
            STKPTR = WREG_SHAD;
            *CLASSB_MarchbufferAddress = TOSL;
            CLASSB_MarchbufferAddress++;
            *CLASSB_MarchbufferAddress = TOSH;
            CLASSB_MarchbufferAddress++;
        }

/*****************************************************************************
* Run Stack Test here.
* ***************************************************************************/

//Write zero into stack
//        STKPTR = 0;
        #asm
        MOVLB   0x1F
        MOVLW   0x1F
        MOVWF   STKPTR
        Wr0:
        INCF    STKPTR
        CLRF    TOSL
        CLRF    TOSH
        MOVF    STKPTR, W
        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Wr0

//Read zero, Write one - ascending
        MOVLW   0x1F
        MOVWF   STKPTR
        Rd0Wr1ASC:
        INCF    STKPTR
        BTFSC   TOSL,0
        GOTO    STACKFAIL
        BSF     TOSL,0
        BTFSC   TOSL,1
        GOTO    STACKFAIL
        BSF     TOSL,1
        BTFSC   TOSL,2
        GOTO    STACKFAIL
        BSF     TOSL,2
        BTFSC   TOSL,3
        GOTO    STACKFAIL
        BSF     TOSL,3
        BTFSC   TOSL,4
        GOTO    STACKFAIL
        BSF     TOSL,4
        BTFSC   TOSL,5
        GOTO    STACKFAIL
        BSF     TOSL,5
        BTFSC   TOSL,6
        GOTO    STACKFAIL
        BSF     TOSL,6
        BTFSC   TOSL,7
        GOTO    STACKFAIL
        BSF     TOSL,7
        BTFSC   TOSH,0
        GOTO    STACKFAIL
        BSF     TOSH,0
        BTFSC   TOSH,1
        GOTO    STACKFAIL
        BSF     TOSH,1
        BTFSC   TOSH,2
        GOTO    STACKFAIL
        BSF     TOSH,2
        BTFSC   TOSH,3
        GOTO    STACKFAIL
        BSF     TOSH,3
        BTFSC   TOSH,4
        GOTO    STACKFAIL
        BSF     TOSH,4
        BTFSC   TOSH,5
        GOTO    STACKFAIL
        BSF     TOSH,5
        BTFSC   TOSH,6
        GOTO    STACKFAIL
        BSF     TOSH,6
        MOVF    STKPTR, W
        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Rd0Wr1ASC

//Read one, Write zero - ascending
        MOVLW   0x1F
        MOVWF   STKPTR
        Rd1Wr0ASC:
        INCF    STKPTR
        BTFSS   TOSL,0
        GOTO    STACKFAIL
        BCF     TOSL,0
        BTFSS   TOSL,1
        GOTO    STACKFAIL
        BCF     TOSL,1
        BTFSS   TOSL,2
        GOTO    STACKFAIL
        BCF     TOSL,2
        BTFSS   TOSL,3
        GOTO    STACKFAIL
        BCF     TOSL,3
        BTFSS   TOSL,4
        GOTO    STACKFAIL
        BCF     TOSL,4
        BTFSS   TOSL,5
        GOTO    STACKFAIL
        BCF     TOSL,5
        BTFSS   TOSL,6
        GOTO    STACKFAIL
        BCF     TOSL,6
        BTFSS   TOSL,7
        GOTO    STACKFAIL
        BCF     TOSL,7
        BTFSS   TOSH,0
        GOTO    STACKFAIL
        BCF     TOSH,0
        BTFSS   TOSH,1
        GOTO    STACKFAIL
        BCF     TOSH,1
        BTFSS   TOSH,2
        GOTO    STACKFAIL
        BCF     TOSH,2
        BTFSS   TOSH,3
        GOTO    STACKFAIL
        BCF     TOSH,3
        BTFSS   TOSH,4
        GOTO    STACKFAIL
        BCF     TOSH,4
        BTFSS   TOSH,5
        GOTO    STACKFAIL
        BCF     TOSH,5
        BTFSS   TOSH,6
        GOTO    STACKFAIL
        BCF     TOSH,6
        MOVF    STKPTR, W
        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Rd1Wr0ASC
        
//March C Minus Test will skip this section.
//Read zero - descending
#ifndef MARCHCMINUS
        MOVLW   0x1F
        MOVWF   STKPTR
        Rd0NMINUS:
        INCF    STKPTR
        MOVLW   0x00
        XORWF   TOSL
        BTFSS   STATUS,2
        GOTO    STACKFAIL
        MOVLW   0X00
        XORWF   TOSH
        BTFSS   STATUS,2
        GOTO    STACKFAIL
        MOVF    STKPTR, W
        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Rd0NMINUS
#endif

//Read zero, Write 1 - descending
        MOVLW   0x10
        MOVWF   STKPTR
        Rd0Wr1DES:
        DECF    STKPTR
        BTFSC   TOSH,6
        GOTO    STACKFAIL
        BSF     TOSH,6
        BTFSC   TOSH,5
        GOTO    STACKFAIL
        BSF     TOSH,5
        BTFSC   TOSH,4
        GOTO    STACKFAIL
        BSF     TOSH,4
        BTFSC   TOSH,3
        GOTO    STACKFAIL
        BSF     TOSH,3
        BTFSC   TOSH,2
        GOTO    STACKFAIL
        BSF     TOSH,2
        BTFSC   TOSH,1
        GOTO    STACKFAIL
        BSF     TOSH,1
        BTFSC   TOSH,0
        GOTO    STACKFAIL
        BSF     TOSH,0
        BTFSC   TOSL,7
        GOTO    STACKFAIL
        BSF     TOSL,7
        BTFSC   TOSL,6
        GOTO    STACKFAIL
        BSF     TOSL,6
        BTFSC   TOSL,5
        GOTO    STACKFAIL
        BSF     TOSL,5
        BTFSC   TOSL,4
        GOTO    STACKFAIL
        BSF     TOSL,4
        BTFSC   TOSL,3
        GOTO    STACKFAIL
        BSF     TOSL,3
        BTFSC   TOSL,2
        GOTO    STACKFAIL
        BSF     TOSL,2
        BTFSC   TOSL,1
        GOTO    STACKFAIL
        BSF     TOSL,1
        BTFSC   TOSL,0
        GOTO    STACKFAIL
        BSF     TOSL,0
        MOVF    STKPTR, F
//        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Rd0Wr1DES

//Read one, Write zero - descending
        MOVLW   0x10
        MOVWF   STKPTR
        Rd1Wr0DES:
        DECF    STKPTR
        BTFSS   TOSH,6
        GOTO    STACKFAIL
        BCF     TOSH,6
        BTFSS   TOSH,5
        GOTO    STACKFAIL
        BCF     TOSH,5
        BTFSS   TOSH,4
        GOTO    STACKFAIL
        BCF     TOSH,4
        BTFSS   TOSH,3
        GOTO    STACKFAIL
        BCF     TOSH,3
        BTFSS   TOSH,2
        GOTO    STACKFAIL
        BCF     TOSH,2
        BTFSS   TOSH,1
        GOTO    STACKFAIL
        BCF     TOSH,1
        BTFSS   TOSH,0
        GOTO    STACKFAIL
        BCF     TOSH,0
        BTFSS   TOSL,7
        GOTO    STACKFAIL
        BCF     TOSL,7
        BTFSS   TOSL,6
        GOTO    STACKFAIL
        BCF     TOSL,6
        BTFSS   TOSL,5
        GOTO    STACKFAIL
        BCF     TOSL,5
        BTFSS   TOSL,4
        GOTO    STACKFAIL
        BCF     TOSL,4
        BTFSS   TOSL,3
        GOTO    STACKFAIL
        BCF     TOSL,3
        BTFSS   TOSL,2
        GOTO    STACKFAIL
        BCF     TOSL,2
        BTFSS   TOSL,1
        GOTO    STACKFAIL
        BCF     TOSL,1
        BTFSS   TOSL,0
        GOTO    STACKFAIL
        BCF     TOSL,0
        MOVF    STKPTR, F
//        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Rd1Wr0DES

//Read one - ascending
        MOVLW   0x1F
        MOVWF   STKPTR
        Rd0ASC:
        INCF    STKPTR
        MOVLW   0x00
        XORWF   TOSL
        BTFSS   STATUS,2
        GOTO    STACKFAIL
        MOVLW   0X00
        XORWF   TOSH
        BTFSS   STATUS,2
        GOTO    STACKFAIL
        MOVF    STKPTR, W
        XORLW   0x0F
        BTFSS   STATUS, 0x02
        GOTO    Rd0ASC

//The Stack test passed!
        GOTO    STACKEND

//The Stack test failed!
        STACKFAIL:
        BANKSEL (_stackTestResult)
        movlw   0xFF
        movwf   BANKMASK(_stackTestResult)
        //GOTO    STACKEND

        STACKEND:
        NOP
        #endasm


/*****************************************************************************
* Restore Stack here.
* ***************************************************************************/
        CLASSB_MarchbufferAddress--;
        for (WREG_SHAD = 16; WREG_SHAD >= 1; WREG_SHAD--)
        {
            STKPTR = WREG_SHAD - 1;
            TOSH = *CLASSB_MarchbufferAddress;
            CLASSB_MarchbufferAddress--;
            TOSL = *CLASSB_MarchbufferAddress;
            CLASSB_MarchbufferAddress--;
        }

        STKPTR = *CLASSB_MarchbufferAddress;

        if (stackTestResult == 0xFF)
        {
            return CLASSB_TEST_FAIL;
        }
        else
        {
            return CLASSB_TEST_PASS;
        }
}