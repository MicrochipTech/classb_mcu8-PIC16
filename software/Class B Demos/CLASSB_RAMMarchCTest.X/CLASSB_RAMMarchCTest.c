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

#include "CLASSB_RAMMarchCTest.h"

#ifndef MARCH_VARS
#define MARCH_VARS
unsigned char* CLASSB_MarchstartAddress @ 0x20;
unsigned char* CLASSB_MarchbufferAddress @ 0x22;
unsigned char* CLASSB_MARCHtempAddress @ 0x24;
unsigned char CLASSB_MarchLength @ 0x26 = 0;
#endif

/******************************************************************************
* Description  : This function implements the March C or March C Minus test.
* It is a complete and irredundant test capable of detecting stuck-at faults,
* addressing faults, transition faults, and coupling faults. This test is of complexity
* 11n (Where n is the number of bits), 10n for March C Minus test.
*
* This test is a destructive test!!! CLASSB_MarchbufferAddress is used as a an array to save user
* memory during the test!
*
* Input:
*     CLASSB_MarchstartAddress     :  pointer to an array declared in user memory for test
*     CLASSB_MarchLength            :  value indicating the number of bytes to test
*     CLASSB_MarchbufferAddress    :  pointer to location in memory where array will be stored during test
*     MARCHCMINUS                   :  designates the test is a March C Minus test.
* Return Values:
*     CLASSB_TEST_PASS 	:  return value = 0
*     CLASSB_TEST_FAIL 	:  return value = 1
*
******************************************************************************/
CLASSBRESULT CLASSB_RAMMarchCTest()
{
    volatile unsigned char testResult;

/******************************************************************************
 * Run March C Test on global RAM used by March C test.
 ******************************************************************************/
    if (CLASSB_MarchstartAddress < (char*)0x27)
    {
        testResult = MarchCGlobalTest();
        if (testResult)
        {
//            MarchCFailure();
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchLength -= (0x28 - (char)CLASSB_MarchstartAddress);
        CLASSB_MarchstartAddress = (char*)0x27;
    }

/******************************************************************************
 * Save memory for test in a known location designated by *CLASSB_MarchbufferAddress
 * The ClassB_March... variables will not be saved.
 ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
    {
        CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
        for (WREG_SHAD = CLASSB_MarchLength; WREG_SHAD != 0; WREG_SHAD--)
        {
            *CLASSB_MarchbufferAddress++ = *CLASSB_MARCHtempAddress++;
        }
    }
    
/******************************************************************************
 * Wr0 - write zero to all memory for test
 ******************************************************************************/
    CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
    for (WREG_SHAD=0; WREG_SHAD<CLASSB_MarchLength; WREG_SHAD++)
    {
        *CLASSB_MarchstartAddress=0x00;
        CLASSB_MarchstartAddress++;
    }

    FSR1L_SHAD = 0;

/******************************************************************************
 * Rd0Wr1 - read zero, write one - ascending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD=0; WREG_SHAD<CLASSB_MarchLength; WREG_SHAD++)
    {
        FSR0H_SHAD = 0x00;
        for (FSR0L_SHAD=0; FSR0L_SHAD<8; FSR0L_SHAD++)
        {
            #asm
            MOVLB   0x00
            MOVF    BANKMASK(_CLASSB_MarchstartAddress), W
            MOVWF   FSR1L
            MOVF    INDF1, W
            MOVLB   0x1F
            XORWF   FSR0H_SHAD, W       //read zero
            MOVLW   0X01
            BTFSS   STATUS, 0x02
            MOVWF   FSR1L_SHAD

            MOVF    INDF1, W
            BSF     STATUS, 0x00
            RLF     FSR0H_SHAD, F       //rotate a 1 into bitMask
            IORWF   FSR0H_SHAD, W
            MOVWF   INDF1               //write 1
            #endasm
            if (FSR1L_SHAD)
                return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * Rd1Wr0 - read one, write zero - ascending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD=0; WREG_SHAD<CLASSB_MarchLength; WREG_SHAD++)
    {
        FSR0H_SHAD = 0xFF;
        for (FSR0L_SHAD=0; FSR0L_SHAD<8; FSR0L_SHAD++)
        {
            #asm
            MOVLB   0x00
            MOVF    BANKMASK(_CLASSB_MarchstartAddress), W
            MOVWF   FSR1L
            MOVF    INDF1, W
            MOVLB   0x1F
            XORWF   FSR0H_SHAD, W
            MOVLW   0X01
            BTFSS   STATUS, 0x02
            MOVWF   FSR1L_SHAD

            MOVF    INDF1, W
            LSLF    FSR0H_SHAD, W
            MOVWF   INDF1

            LSLF    FSR0H_SHAD, F
            #endasm
            if (FSR1L_SHAD)
                return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * For March C Minus test this section is skipped
 * Rd0 - read zero - descending
 ******************************************************************************/
#ifndef MARCHCMINUS

    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD=0; WREG_SHAD < CLASSB_MarchLength; WREG_SHAD++)
    {
        FSR0H_SHAD = 0x00;
        for (FSR0L_SHAD=0; FSR0L_SHAD<8; FSR0L_SHAD++)
        {
            #asm
            MOVLB   0x00
            MOVF    BANKMASK(_CLASSB_MarchstartAddress), W
            MOVWF   FSR1L
            MOVF    INDF1, W
            MOVLB   0x1F
            XORWF   FSR0H_SHAD, W       //Read 0
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   FSR1L_SHAD
            #endasm
            if (FSR1L_SHAD)
                return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

#endif

/******************************************************************************
 * Rd0Wr1 - read zero, write one - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress--;
    CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
    for (WREG_SHAD=CLASSB_MarchLength; WREG_SHAD>0; WREG_SHAD--)
    {
        FSR0H_SHAD = 0x00;
        for (FSR0L_SHAD=0; FSR0L_SHAD<8; FSR0L_SHAD++)
        {
            #asm
            MOVLB   0x00
            MOVF    BANKMASK(_CLASSB_MarchstartAddress), W
            MOVWF   FSR1L
            MOVF    INDF1, W
            MOVLB   0x1F
            XORWF   FSR0H_SHAD, W       //Read 0
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   FSR1L_SHAD

            MOVF    INDF1, W
            BSF     STATUS, 0x00
            RRF     FSR0H_SHAD, F       //rotate a 1 into bitMask
            IORWF   FSR0H_SHAD, W
            MOVWF   INDF1               //write 1
            #endasm
            if (FSR1L_SHAD)
                return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * Rd1Wr0 - read one, write zero - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD=CLASSB_MarchLength; WREG_SHAD>0; WREG_SHAD--)
    {
        FSR0H_SHAD = 0xFF;
        for (FSR0L_SHAD=0; FSR0L_SHAD<8; FSR0L_SHAD++)
        {
            #asm
            MOVLB   0x00
            MOVF    BANKMASK(_CLASSB_MarchstartAddress), W
            MOVWF   FSR1L
            MOVF    INDF1, W
            MOVLB   0x1F
            XORWF   FSR0H_SHAD, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   FSR1L_SHAD

            MOVF    INDF1, W
            LSRF    FSR0H_SHAD, W
            MOVWF   INDF1

            LSRF    FSR0H_SHAD, F
            #endasm
            if (FSR1L_SHAD)
                return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * Rd0 - read zero - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD=(CLASSB_MarchLength-1); WREG_SHAD>0; WREG_SHAD--)
    {
        FSR0H_SHAD = 0x00;
        for (FSR0L_SHAD=0; FSR0L_SHAD<8; FSR0L_SHAD++)
        {
            #asm
            MOVLB   0x00
            MOVF    BANKMASK(_CLASSB_MarchstartAddress), W
            MOVWF   FSR1L
            MOVF    INDF1, W
            MOVLB   0x1F
            XORWF   FSR0H_SHAD, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   FSR1L_SHAD
            #endasm
            if (FSR1L_SHAD)
                return 1;
        }
        CLASSB_MarchstartAddress--;
    }
    
/******************************************************************************
 * March C (Minus) Test Passed
 * Restore User data from the buffer.
 ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
    {
        CLASSB_MARCHtempAddress = (CLASSB_MarchbufferAddress - CLASSB_MarchLength);
        for (WREG_SHAD = CLASSB_MarchLength; WREG_SHAD != 0; WREG_SHAD--)
        {
            *CLASSB_MarchstartAddress++ = *CLASSB_MARCHtempAddress++;
        }
    }
    return CLASSB_TEST_PASS;

} // End of function

/***********************************************************
* Description:
*     This function runs the March C test on the 7 RAM 
*     addresses used to run the rest of the March C test.
***********************************************************/
CLASSBRESULT MarchCGlobalTest (void)
{

#asm
    MOVLB 0x00
    MOVF 0x20, W
    MOVWF FSR0L         //CLASSB_MarchstartAddress
    MOVF 0x22, W
    MOVWF FSR0H         //CLASSB_MarchbufferAddress
    MOVF 0x26, W
    MOVLB 0x1F
    MOVWF WREG_SHAD     //CLASSB_MarchLength
    MOVF FSR0L, W
    MOVWF FSR0L_SHAD    //CLASSB_MarchstartAddress
    MOVF FSR0H, W
    MOVWF FSR0H_SHAD    //CLASSB_MarchbufferAddress
#endasm

    uint8_t* tempAddrPTR;
    uint8_t r;
    uint8_t i;
    uint8_t bitMask;

    tempAddrPTR = (char*)0x20;

    //(^w0)(^r0,w1)(^r1,w0)
    //(!M^r0)(/r0,w1)(/r1,w0)(^r0)

    //write 0 - ascending
    for(r = 0; r < 7; r++)
    {
        *tempAddrPTR = 0;
        tempAddrPTR++;
    }

    //read zero, write one - ascending
    tempAddrPTR = (char*)0x20;
    for(r = 0; r < 7; r++)
    {
        bitMask = 0x01;
        for(i = 0; i < 8; i++)
        {
            if(*tempAddrPTR & bitMask)
            {
                return CLASSB_TEST_FAIL;
            }
            else
            {
                *tempAddrPTR = *tempAddrPTR | bitMask;
            }
            bitMask <<= 1;
        }
        tempAddrPTR++;
    }

    //read one, write zero - ascending
    tempAddrPTR = (char*)0x20;
    for(r = 0; r < 7; r++)
    {
        bitMask = 0x01;
        for(i = 0; i < 8; i++)
        {
            if(*tempAddrPTR & bitMask)
            {
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            bitMask <<= 1;
        }
        tempAddrPTR++;
    }

    //NOT MINUS test
    //read zero - ascending
#ifndef MARCHCMINUS
    tempAddrPTR = (char*)0x20;
    for(r = 0; r < 7; r++)
    {
        if (*tempAddrPTR != 0)
        {
            return CLASSB_TEST_FAIL;
        }
        tempAddrPTR++;
    }
#endif

    //read zero, write one - descending
    tempAddrPTR = (char*)0x26;
    for (r = 0; r < 7; r++)
    {
        bitMask = 0x80;
        for (i=0; i<8; i++)
        {
            if(*tempAddrPTR & bitMask)
            {
                return CLASSB_TEST_FAIL;
            }
            else
            {
                *tempAddrPTR = *tempAddrPTR | bitMask;
            }
            bitMask>>=1;
        }
        tempAddrPTR--;
    }

    //read one, write zero - descending
    tempAddrPTR = (char*)0x26;
    for (r = 0; r < 7; r++)
    {
        bitMask = 0x80;
        for(i = 0; i < 8; i++)
        {
            if(*tempAddrPTR & bitMask)
            {
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            bitMask >>= 1;
        }
        tempAddrPTR--;
    }

    //read zero - ascending
    tempAddrPTR = (char*)0x20;
    for(r = 0; r < 7; r++)
    {
        if (*tempAddrPTR != 0)
        {
            return CLASSB_TEST_FAIL;
        }
        tempAddrPTR++;
    }

#asm
    MOVLB 0x1F
    MOVF FSR0L_SHAD, W
    MOVWF FSR0L         //CLASSB_MarchstartAddress
    MOVF FSR0H_SHAD, W
    MOVWF FSR0H         //CLASSB_MarchbufferAddress
    MOVF WREG_SHAD, W
    MOVLB 0x00
    MOVWF 0x26     //CLASSB_MarchLength
    MOVF FSR0L, W
    MOVWF 0x20    //CLASSB_MarchstartAddress
    MOVF FSR0H, W
    MOVWF 0x22    //CLASSB_MarchbufferAddress
#endasm

    return CLASSB_TEST_PASS;
}