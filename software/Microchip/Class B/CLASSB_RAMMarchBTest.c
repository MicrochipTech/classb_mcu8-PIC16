/**********************************************************************
 * © 2013 - 2014 Microchip Technology Inc.
 *
 * Project Name:     Class B Library
 * FileName:         CLASSB_RAMMarchBTest.c
 * Dependencies:     CLASSB_RAMMarchBTest.h
 * Processor:        PIC16F1xxx
 * Compiler:         XC8
 * IDE:              MPLAB® IDE or MPLAB® X
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Description:     This file contains functions to test RAM with MarchB Test.
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
 * CT                08/15/2008  First release of source file
 * Mike Cahill       11/11/2010	Updated for HI-TECH v9.80
 * MVL               02/22/2011	Modified for HI-TECH v9.81
 * Corey Simoncic    04/19/2013  Updated for XC-8
 * Willem J Smit     09/25/2014  Fixed compiler related bug (XC8 1.20+), Added capability of test all banks (was limited to bank 1 & 2)
 *
 * Version       3.01
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ADDITIONAL NOTES:
 *
 **********************************************************************/
#include "CLASSB_RAMMarchBTest.h"

#ifndef MARCH_VARS
#define MARCH_VARS
unsigned char* CLASSB_MarchstartAddress @ 0x20;
unsigned char* CLASSB_MarchbufferAddress @ 0x22;
unsigned char* CLASSB_MARCHtempAddress @ 0x24;
unsigned char CLASSB_MarchLength @ 0x26 = 0;
#endif

/******************************************************************************
 * Description  : This function implements the March B test. It is a complete and irredundant test
 * capable of detecting stuck-at faults , linked coupling Idempotent faults or Inversion Coupling
 * faults. This test is of complexity 17n( Where n is the number of bits).
 *
 * This test is a destructive test!!! *CLASSB_MarchbufferAddress is used as a an array to save user
 * memory during the test!
 *
 * Input:
 *     CLASSB_MarchstartAddress     :  pointer to an array declared in user memory for test
 *     CLASSB_MarchLength            :  value indicating the number of bytes to test
 *     CLASSB_MarchbufferAddress    :  pointer to location in memory where array will be stored during test
 * Return Values:
 *     CLASSB_TEST_PASS 	:  return value = 0
 *     CLASSB_TEST_FAIL 	:  return value = 1
 *
 ******************************************************************************/
CLASSBRESULT CLASSB_RAMMarchBTest() {
    volatile unsigned char testResult;
    /******************************************************************************
     * Save memory for test in a known location designated by *CLASSB_MarchbufferAddress
     ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
        memcpy(CLASSB_MarchbufferAddress, CLASSB_MarchstartAddress, CLASSB_MarchLength);

    /******************************************************************************
     * Run March B Test on global RAM used by March B test.
     ******************************************************************************/
    if (CLASSB_MarchstartAddress < (char*) 0x27) {
        testResult = MarchBGlobalTest();
        if (testResult) {
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchLength -= (0x28 - (char) CLASSB_MarchstartAddress);
        CLASSB_MarchstartAddress = (char*) 0x27;
    }

    /******************************************************************************
     * Wr0 - write zero to all memory for test
     ******************************************************************************/
    CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
    for (WREG_SHAD = 0; WREG_SHAD < CLASSB_MarchLength; WREG_SHAD++) {
        *CLASSB_MarchstartAddress = 0x00;
        CLASSB_MarchstartAddress++;
    }
#pragma warning disable 1352


    /******************************************************************************
     * Rd0Wr1Rd1Wr0Rd0Wr1 - read zero, write one, read 1, write zero, read 0, write 1
     ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD = 0; WREG_SHAD < CLASSB_MarchLength; WREG_SHAD++) {
        FSR0H_SHAD = 0x00;
        FSR1L_SHAD = 0x00;
        for (FSR0L_SHAD = 0; FSR0L_SHAD < 8; FSR0L_SHAD++) {
#asm
                    MOVLB 0x00
                    MOVF BANKMASK(_CLASSB_MarchstartAddress), W
                    MOVWF FSR1L
                    MOVF 0x21, W
                    MOVWF FSR1H
                    MOVF INDF1, W
                    MOVLB 0x1F
                    XORWF FSR0H_SHAD, W
                    MOVLW 0x01
                    BTFSS STATUS, 0x02 //read zero
                    MOVWF FSR1L_SHAD


                    MOVF INDF1, W
                    BSF STATUS, 0x00
                    RLF FSR0H_SHAD, F
                    IORWF FSR0H_SHAD, W
                    MOVWF INDF1 //write one

                    MOVF INDF1, W
                    XORWF FSR0H_SHAD, W //read one
                    MOVLW 0x01
                    BTFSS STATUS, 0x02
                    MOVWF FSR1L_SHAD

                    MOVF INDF1, W
                    LSRF FSR0H_SHAD, W
                    LSRF FSR0H_SHAD, F
                    MOVWF INDF1 //write zero

                    MOVF INDF1, W
                    XORWF FSR0H_SHAD, W
                    MOVLW 0X01
                    BTFSS STATUS, 0X02 //read zero
                    MOVWF FSR1L_SHAD

                    MOVF INDF1, W
                    BSF STATUS, 0X00
                    RLF FSR0H_SHAD, F
                    IORWF FSR0H_SHAD, W
                    MOVWF INDF1 //write one
#endasm

            if (FSR1L_SHAD) 
                return CLASSB_TEST_FAIL;          
        }
        CLASSB_MarchstartAddress++;
    }

    /******************************************************************************
     * Rd1Wr0Wr1 - read one, write zero, write one
     ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD = 0; WREG_SHAD < CLASSB_MarchLength; WREG_SHAD++) {
        FSR0H_SHAD = 0xFF;
        FSR1H_SHAD = 0XFE;
        for (FSR0L_SHAD = 0; FSR0L_SHAD < 8; FSR0L_SHAD++) {
#asm
                    MOVLB 0x00;
                    MOVF BANKMASK(_CLASSB_MarchstartAddress), W
                    MOVWF FSR1L
                    MOVF INDF1, W
                    MOVF 0x21, W
                    MOVWF FSR1H
                    MOVF INDF1, W
                    MOVLB 0x1F
                    XORWF FSR0H_SHAD, W //read one
                    MOVLW 0x01
                    BTFSS STATUS, 0x02
                    MOVWF FSR1L_SHAD

                    MOVF FSR1H_SHAD, W
                    MOVWF INDF1 //write zero
                    BSF STATUS, 0x00
                    RLF FSR1H_SHAD, F

                    MOVF FSR0H_SHAD, W
                    MOVWF INDF1 //write one
#endasm
                    if (FSR1L_SHAD) 
                return CLASSB_TEST_FAIL;           
        }
        CLASSB_MarchstartAddress++;
    }

    /******************************************************************************
     * Rd1Wr0Wr1Wr0 - read one, write zero, write one, write zero
     ******************************************************************************/
    //CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress + CLASSB_MarchLength - 1;
    CLASSB_MarchstartAddress--;
    CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
    for (WREG_SHAD = CLASSB_MarchLength; WREG_SHAD > 0; WREG_SHAD--) {
        FSR0H_SHAD = 0xFF;
        FSR1H_SHAD = 0X7F;
        for (FSR0L_SHAD = 0; FSR0L_SHAD < 8; FSR0L_SHAD++) {
#asm
                    MOVLB 0x00;
                    MOVF BANKMASK(_CLASSB_MarchstartAddress), W
                    MOVWF FSR1L
                    MOVF INDF1, W
                    MOVF 0x21, W
                    MOVWF FSR1H
                    MOVF INDF1, W
                    MOVLB 0x1F
                    XORWF FSR0H_SHAD, W //read one
                    MOVLW 0x01
                    BTFSS STATUS, 0x02
                    MOVWF FSR1L_SHAD

                    MOVF FSR1H_SHAD, W
                    MOVWF INDF1 //write zero


                    MOVF FSR0H_SHAD, W
                    MOVWF INDF1 //write one

                    MOVF FSR1H_SHAD, W
                    MOVWF INDF1 //write zero

                    LSRF FSR1H_SHAD, F
                    LSRF FSR0H_SHAD, F
#endasm
                    if (FSR1L_SHAD) 
                return CLASSB_TEST_FAIL;          
        }
        CLASSB_MarchstartAddress--;
    }

    /******************************************************************************
     * Rd0Wr1Wr0 - read zero, write one, write zero
     ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (WREG_SHAD = CLASSB_MarchLength; WREG_SHAD > 0; WREG_SHAD--) {
        FSR0H_SHAD = 0x00;
        FSR1H_SHAD = 0x80;
        for (FSR0L_SHAD = 0; FSR0L_SHAD < 8; FSR0L_SHAD++) {
#asm
                    MOVLB 0x00
                    MOVF BANKMASK(_CLASSB_MarchstartAddress), W
                    MOVWF FSR1L
                    MOVF INDF1, W
                    MOVF 0x21, W
                    MOVWF FSR1H
                    MOVF INDF1, W
                    MOVLB 0x1F
                    XORWF FSR0H_SHAD, W
                    MOVLW 0x01
                    BTFSS STATUS, 0x02 //read zero
                    MOVWF FSR1L_SHAD

                    MOVF FSR1H_SHAD, W
                    MOVWF INDF1 //write one
                    LSRF FSR1H_SHAD, F

                    MOVF FSR0H_SHAD, W
                    MOVWF INDF1 //write zero
#endasm
                    if (FSR1L_SHAD) 
                return CLASSB_TEST_FAIL;           
        }
        CLASSB_MarchstartAddress--;
    }
    CLASSB_MarchstartAddress++;
    /******************************************************************************
     * Restore user memory
     * All tests passed
     ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
        memcpy(CLASSB_MarchstartAddress, CLASSB_MarchbufferAddress, CLASSB_MarchLength); //restore user memory

    return CLASSB_TEST_PASS;
}

/******************************************************************************
 * Description:
 *     This function tests the global variables used in the March Test
 * Return Values:
 *     CLASSB_TEST_PASS :  return value = 0
 *     CLASSB_TEST_FAIL :  return value = 1
 *
 ******************************************************************************/
CLASSBRESULT MarchBGlobalTest() {
    uint8_t* tempAddrPTR;
    uint8_t r;
    //uint8_t globalResult;
    uint8_t bitMask;
    uint8_t i;

#asm
    MOVLB 0x00
            MOVF 0x20, W
            MOVWF FSR0L //CLASSB_MarchstartAddress
            MOVF 0x22, W
            MOVWF FSR0H //CLASSB_MarchbufferAddress
            MOVF 0x26, W
            MOVLB 0x1F
            MOVWF WREG_SHAD //CLASSB_MarchLength
            MOVF FSR0L, W
            MOVWF FSR0L_SHAD //CLASSB_MarchstartAddress
            MOVF FSR0H, W
            MOVWF FSR0H_SHAD //CLASSB_MarchbufferAddress
#endasm

            tempAddrPTR = (char*) 0x20;

    //(^w0)(^r0,w1,r1,w0,r0,w1)(^r1,w0,w1)
    //(/r1,w0,w1,w0)(/r0,w1,w0)

    //write 0 - ascending
    for (r = 0; r < 7; r++) {
        *tempAddrPTR = 0;
        tempAddrPTR++;
    }

    //read 0, write 1, read 1, write 0, read 0, write 1 - ascending
    tempAddrPTR = (char*) 0x20;
    for (r = 0; r < 7; r++) {
        bitMask = 0x01;
        for (i = 0; i < 8; i++) {
            if (*tempAddrPTR & bitMask) //read zero
            {
                return CLASSB_TEST_FAIL;
            } else {
                *tempAddrPTR = *tempAddrPTR | bitMask; //write one
            }
            if (*tempAddrPTR & bitMask) //read one
            {
                *tempAddrPTR = *tempAddrPTR & ~bitMask; //write zero
            } else {
                return CLASSB_TEST_FAIL;
            }
            if (*tempAddrPTR & bitMask) //read zero
            {
                return CLASSB_TEST_FAIL;
            } else {
                *tempAddrPTR = *tempAddrPTR | bitMask; //write one
            }
            bitMask <<= 1;
        }
        tempAddrPTR++;
    }

    //read 1, write 0, write 1 - ascending
    tempAddrPTR = (char*) 0x20;
    for (r = 0; r < 7; r++) {
        bitMask = 0x01;
        for (i = 0; i < 8; i++) {
            if (*tempAddrPTR & bitMask) {
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
                *tempAddrPTR = *tempAddrPTR | bitMask;
            } else {
                return CLASSB_TEST_FAIL;
            }
            bitMask <<= 1;
        }
        tempAddrPTR++;
    }

    //read 1, write 0, write 1, write 0 - descending
    tempAddrPTR = (char*) 0x26;
    for (r = 0; r < 7; r++) {
        bitMask = 0x80;
        for (i = 0; i < 8; i++) {
            if (*tempAddrPTR & bitMask) {
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
                *tempAddrPTR = *tempAddrPTR | bitMask;
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
            } else {
                return CLASSB_TEST_FAIL;
            }
            bitMask >>= 1;
        }
        tempAddrPTR--;
    }

    //read 0, write 1, write 0 - descending
    tempAddrPTR = (char*) 0x26;
    for (r = 0; r < 7; r++) {
        bitMask = 0x80;
        for (i = 0; i < 8; i++) {
            if (*tempAddrPTR & bitMask)
                return CLASSB_TEST_FAIL;
            else {
                *tempAddrPTR = *tempAddrPTR | bitMask;
                *tempAddrPTR = *tempAddrPTR & ~bitMask;
            }
            bitMask >>= 1;
        }
        tempAddrPTR--;
    }

#asm
    MOVLB 0x1F
            MOVF FSR0L_SHAD, W
            MOVWF FSR0L //CLASSB_MarchstartAddress
            MOVF FSR0H_SHAD, W
            MOVWF FSR0H //CLASSB_MarchbufferAddress
            MOVF WREG_SHAD, W
            MOVLB 0x00
            MOVWF 0x26 //CLASSB_MarchLength
            MOVF FSR0L, W
            MOVWF 0x20 //CLASSB_MarchstartAddress
            MOVF FSR0H, W
            MOVWF 0x22 //CLASSB_MarchbufferAddress
#endasm

    return CLASSB_TEST_PASS;
}