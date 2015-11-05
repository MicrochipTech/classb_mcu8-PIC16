/**********************************************************************
* © 2013-2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB_CRCbyte.c
* Dependencies:     CLASSB_CRCbyte.h
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains functions for implementing a CRC test.
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
 ************************************************************************
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date        Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Corey Simoncic    04/19/2013  Updated for XC-8
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "CLASSB_CRCbyte.h"
 
uint16_t crcreg; //the current remainder of the CRC calculation
uint8_t parity; //the parity bit for CRC calculation
uint8_t dataXorCrcHigh; //Data XOR'd with the current CRC High byte
 
/******************************************************************************
* Description:
*     This function does the CRC calculation of a single byte.
* Input:
*     dataSequence :  the byte to be tested
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

C is current crc value (seed or previous iteration)
D is the current data sequence
S is the data sequence XOR'd with the high byte of the current crc value
P is the parity bit

S[i] = C8^D0, C9^D1, C10^D2, C11^D3... etc

parity = C8^C9^C10^C11^C12^C13^C14^C15 ^ D7^D6^D5^D4^D3^D2^D1^D0

crcreg[0]  =      P
crcreg[1]  =      S0 ^ P
crcreg[2]  =      S1 ^ S0
crcreg[3]  =      S2 ^ S1
crcreg[4]  =      S3 ^ S2
crcreg[5]  =      S4 ^ S3
crcreg[6]  =      S5 ^ S4
crcreg[7]  =      S6 ^ S5
crcreg[8]  = C0 ^ S7 ^ S6
crcreg[9]  = C1      ^ S7
crcreg[10] = C2
crcreg[11] = C3
crcreg[12] = C4
crcreg[13] = C5
crcreg[14] = C6
crcreg[15] = C7 ^ P
******************************************************************************/
void crcByte(uint8_t dataSequence)
{
    WREG = dataSequence;

#asm
    banksel _crcreg
    xorwf   BANKMASK(_crcreg)+1,w           //XOR the upper byte of CRCREG
                                            //with the input data sequence
                                            //This creates S[i]

    movwf   BANKMASK(_dataXorCrcHigh)
    movwf   BANKMASK(_parity)

    //The following section finds the parity of the CRCREG and the Data sequence
    swapf   BANKMASK(_parity),w
    xorwf   BANKMASK(_parity),f         //S0^S4, S1^S5... etc
    lsrf    BANKMASK(_parity),w         //the upper 4 bits are unnecessary
    lsrf    WREG
    xorwf   BANKMASK(_parity),f         //S0^S4^S1^S5, S2^S6^S3^S7
    lsrf    BANKMASK(_parity),w         //the upper 6 bits are unnecessary
    xorwf   BANKMASK(_parity),f         //P = S0^S4^S1^S5^S2^S6^S3^S7
                                        //the upper 7 bits are unnecessary

    lslf    BANKMASK(_dataXorCrcHigh),w
    xorwf   BANKMASK(_dataXorCrcHigh),f     //S0,S1^S0,S2^S1...etc

    movf    BANKMASK(_crcreg),w         //Puts the lower byte of the CRC into W
                                        //use this to solve for the upper byte
    btfsc   BANKMASK(_parity),0
    xorlw   80h                         //0x80 is used to position the parity bit
                                        //on C7 if it is high. Solves for C15

    btfsc   STATUS,0                    //The Carry bit is currently S7
    xorlw   02h                         //0x02 positions S7 on C1 if it is high
                                        //Solves for C9

    btfsc   BANKMASK(_dataXorCrcHigh),7 //S7 currently holds S7^S6
    xorlw   01h                         //0x01 positions S7^S6 on C0
                                        //Solves for C8

    movwf   BANKMASK(_crcreg)+1         //Saves the high byte of CRCREG

    lslf    BANKMASK(_dataXorCrcHigh),w //This shifts S[i] over to position it
                                        //on C1:C7
    btfsc   BANKMASK(_parity),0
    xorlw   03h                         //0x03 is used to position the parity bit
                                        //on C0 and C1. Solves for C0:C7

    movwf   BANKMASK(_crcreg)           //Saves the low byte of CRCREG

#endasm
}