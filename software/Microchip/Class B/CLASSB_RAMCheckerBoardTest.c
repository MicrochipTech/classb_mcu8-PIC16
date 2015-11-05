/**********************************************************************
 * © 2013 - 2014 Microchip Technology Inc.
 *
 * Project Name:     Class B Library
 * FileName:         CLASSB_RAMCheckerBoardTest.c
 * Dependencies:     CLASSB_RAMCheckerBoardTest.h
 * Processor:        PIC16F1xxx
 * Compiler:         XC8
 * IDE:              MPLAB® IDE or MPLAB® X
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Description:     This file contains functions to RAM with Checker Board Test.
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
 * EA                03/05/2012	Renamed file as Checkerboard RAM Test
 * Corey Simoncic    04/19/2013  Updated for XC-8
 * EA                09/04/2014  Added Global test for memory locations used by test
 * Willem J Smit     09/29/2014  Substituted C variables with XXXX_SHAD registers. Improved test status reporting. Added ability to test all banks.
 *
 * Version 		3.01
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ADDITIONAL NOTES:
 *
 **********************************************************************/
#include "CLASSB_RAMCheckerBoardTest.h"


#ifndef CHECKER_VARS
#define CHECKER_VARS
unsigned char* CLASSB_CheckerboardStartAddress @ 0x20;
unsigned char* CLASSB_CheckerboardBufferAddress @0x22;
unsigned int CLASSB_CheckerboardIndex @ 0x24 = 0;
unsigned char CLASSB_CheckerboardLength @ 0x26 = 0;

#endif

/*****************************************************************************
 * Description:
 *     This function that implements a CheckerBoard RAM test.
 *     If bufferAddress is NULL the test will be destructive!!!
 * Input:
 *     *startAddress  	:  pointer to an array declared in user memory for test
 *     length 		:  value indicating the number of bytes to test
 *     *bufferAddress    :  pointer to location in memory where array will be stored during test
 * Return Values:
 *     CLASSB_TEST_PASS 	:  return value = 0
 *     CLASSB_TEST_FAIL 	:  return value = 1
 *
 * Modifed to include self check of used memories
 *
 ** Input:
 *     CLASSB_CheckerboardStartAddress       :  pointer to an array declared in user memory for test
 *     CLASSB_CheckerboardLength             :  value indicating the number of bytes to test
 *     CLASSB_CheckerboardBufferAddres       :  pointer to location in memory where array will be stored during test
 ******************************************************************************/
//CLASSBRESULT CLASSB_RAMCheckerboardTest(uint8_t* startAddress, uint8_t length, uint8_t* bufferAddress)

CLASSBRESULT CLASSB_RAMCheckerboardTest() {

    //Reset FSR1x Shadow Registers
    FSR1L_SHAD = 0x00;
    FSR1H_SHAD = 0x00;

    /******************************************************************************
     * Save memory for test in a known location designated by *CLASSB_CheckerboardBufferAddress
    ******************************************************************************/
    if (CLASSB_CheckerboardBufferAddress != NULL)
        memcpy(CLASSB_CheckerboardBufferAddress, CLASSB_CheckerboardStartAddress, CLASSB_CheckerboardLength); //save user Memory

#asm
                MOVLB 0x00 //Bank 0
                MOVF BANKMASK(_CLASSB_CheckerboardStartAddress), W
                MOVLB 0x1F //Bank 31
                MOVWF FSR1L_SHAD
                MOVLB 0x00 //Bank 0
                MOVF 0x21, W
                MOVLB 0x1F
                MOVWF FSR1H_SHAD
#endasm

      /***************************************************************************
        * Run the Global Checkerboard test
     **************************************************************************/
    FSR0H_SHAD = 0x00; //Init condition
    FSR0H_SHAD = CheckerGlobalTest();
    if (FSR0H_SHAD) {
        return CLASSB_TEST_FAIL;
    }

      /***************************************************************************
     * Run the Checkerboard test
     **************************************************************************/

    WREG_SHAD = 0x55; //checker
    for (BSR_SHAD = 0; BSR_SHAD < 2; BSR_SHAD++) {
        //write checker pattern in RAM
        for (FSR0L_SHAD = 0; FSR0L_SHAD < (CLASSB_CheckerboardLength); FSR0L_SHAD++) {
            (*CLASSB_CheckerboardStartAddress) = WREG_SHAD;
            CLASSB_CheckerboardStartAddress++;
            WREG_SHAD = ~WREG_SHAD;
        }

        //Restore FSR1L & FSR1H
        RestoreFSR1x ();

        //read checker pattern in RAM
        for (FSR0L_SHAD = 0; FSR0L_SHAD < CLASSB_CheckerboardLength; FSR0L_SHAD++) {
            if ((*CLASSB_CheckerboardStartAddress) != WREG_SHAD) {
                if (CLASSB_CheckerboardBufferAddress != NULL)
                    memcpy(CLASSB_CheckerboardStartAddress, CLASSB_CheckerboardBufferAddress, CLASSB_CheckerboardLength); //restore user memory
                return CLASSB_TEST_FAIL;
            }
            CLASSB_CheckerboardStartAddress++;
            WREG_SHAD = ~WREG_SHAD;

        }
        //invert checker pattern
        WREG_SHAD = ~WREG_SHAD;

        //Restore FSR1L & FSR1H
        RestoreFSR1x ();

    }
    if (CLASSB_CheckerboardBufferAddress != NULL)
        memcpy(CLASSB_CheckerboardStartAddress, CLASSB_CheckerboardBufferAddress, CLASSB_CheckerboardLength); //restore user memory
    return CLASSB_TEST_PASS;

} // End of function

/***********************************************************
 * Description:
 *     This function runs the Checkerboard test on the 7 RAM
 *     addresses used to run the rest of the March C test.
 ***********************************************************/
CLASSBRESULT CheckerGlobalTest(void) {

    //Save essential values to safe locations
#asm
                MOVLB 0x00 //Bank 0
                MOVF BANKMASK(_CLASSB_CheckerboardStartAddress), W
                MOVWF FSR1L
                MOVF 0x21, W   //CLASSB_CheckerboardStartAddress MSB
                MOVWF FSR1H
                MOVF BANKMASK(_CLASSB_CheckerboardBufferAddress), W
                MOVWF FSR0H
                MOVF BANKMASK(_CLASSB_CheckerboardLength), W
                MOVLB 0x1F // Bank 31
                MOVWF FSR0L_SHAD //CLASSB_CheckerboardLength
                MOVF FSR1L, W
                MOVWF FSR1L_SHAD //CLASSB_CheckerboardStartAddres LSB
                MOVF FSR1H, W
                MOVWF FSR1H_SHAD //CLASSB_CheckerboardStartAddres MSB
                MOVF FSR0H, W
                MOVWF FSR0H_SHAD //CLASSB_CheckerboardBufferAddress
#endasm

    // Write 0x55, 0xAA
    FSR1 = 0x20;
    for (BSR_SHAD = 0; BSR_SHAD < 5; BSR_SHAD++) {
        INDF1 = 0x55;
        FSR1++;
        INDF1 = 0xAA;
        FSR1++;
    }

    // Read back, check and write inverse checkerboard
    FSR1 = 0x20;
    for (BSR_SHAD = 0; BSR_SHAD < 5; BSR_SHAD++) {
        WREG_SHAD = 0x55;
        if (INDF1 ^ WREG_SHAD) {
            return CLASSB_TEST_FAIL;
        } else {
            INDF1 = ~WREG_SHAD;
        }
        FSR1++;

        WREG_SHAD = 0xAA;
        if (INDF1 ^ WREG_SHAD) {
            return CLASSB_TEST_FAIL;
        } else {
            INDF1 = ~WREG_SHAD;
        }
        FSR1++;
    }

    // Read back, and check
    FSR1 = 0x20;
    for (BSR_SHAD = 0; BSR_SHAD < 5; BSR_SHAD++) {
        WREG_SHAD = 0xAA;
        if (INDF1 ^ WREG_SHAD) {
            return CLASSB_TEST_FAIL;
        } else {
            INDF1 = ~WREG_SHAD;
        }
        FSR1++;
        WREG_SHAD = 0x55;
        if (INDF1 ^ WREG_SHAD) {
            return CLASSB_TEST_FAIL;
        } else {
            INDF1 = ~WREG_SHAD;
        }
        FSR1++;
    }

    //Clear test values as a precaution
    FSR1 = 0x20;
    for (BSR_SHAD = 0; BSR_SHAD < 10; BSR_SHAD++) {
        INDF1 = 0x00;
        FSR1++;
    }

    // Restore values to original locations
#asm
                MOVLB 0x1F //Bank 31
                MOVF FSR1L_SHAD, W //CLASSB_CheckerboardStartAddress
                MOVWF FSR1L
                MOVF FSR1H_SHAD, W //CLASSB_CheckerboardStartAddress
                MOVWF FSR1H
                MOVF FSR0H_SHAD, W //CLASSB_CheckerboardBufferAddress
                MOVWF FSR0H
                MOVF FSR0L_SHAD, W
                MOVLB 0x00 //Bank 0
                MOVWF 0x26 //CLASSB_CheckerboardLength
                MOVF FSR1L, W
                MOVWF BANKMASK(_CLASSB_CheckerboardStartAddress) //LSB
                MOVF FSR1H, W
                MOVWF 0x21 //CLASSB_CheckerboardStartAddress MSB
                MOVF FSR0H, W
                MOVWF 0x22 //CLASSB_CheckerboardBufferAddress
#endasm
    return CLASSB_TEST_PASS;

                
}

void RestoreFSR1x (void){
#asm
                MOVLB 0x1F
                MOVF FSR1L_SHAD, W
                MOVLB 0x00
                MOVWF BANKMASK(_CLASSB_CheckerboardStartAddress) //LSB
                MOVLB 0x1F
                MOVF FSR1H_SHAD, W
                MOVLB 0x00
                MOVWF 0x21 //CLASSB_CheckerboardStartAddress MSB
#endasm
}
