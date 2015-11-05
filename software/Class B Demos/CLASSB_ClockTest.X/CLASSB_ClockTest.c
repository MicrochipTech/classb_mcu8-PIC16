/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         CLASSB_ClockTest.c
* Dependencies:     CLASSB_ClockTest.h
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains functions to check the MCU clock
*                  source.
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
* Corey Simoncic    03/25/2013  Updated for XC-8 PRO v1.12
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "CLASSB_ClockTest.h"

uint16_t CLASSB_timerCount = 0;
uint16_t CLASSB_Clockcount = 0;

uint8_t CLASSB_TRISC_temp;
uint8_t CLASSB_INTCON_temp;
uint8_t CLASSB_T1CON_temp;
uint8_t CLASSB_PIE1_temp;

/******************************************************************************
* Description:
*   The Clock test implements the independent time slot monitoring H.2.18.10.4
*   defined by the IEC 60730 standard. It verifies the reliability of the system
*   clock (i.e., the system clock should be neither too fast nor too slow)
*
* Input:
*   clockFrequency      - system clock frequency.
*   referenceFrequency  - reference clock frequency.
*   msec                - the time in milliseconds to run the test.
*   tolerance           - the tolerance level of the system oscillator.
*
* Return Values:
*     CLASSB_TEST_PASS 	:  return value = 0
*     CLASSB_TEST_FAIL 	:  return value = 1
******************************************************************************/
CLASSBRESULT CLASSB_ClockTest(uint32_t clockFrequency, uint32_t referenceFrequency, size_t msec, uint8_t tolerance)
{
    uint32_t Clock_expectedCount;
    uint32_t Clock_toleranceCount;
    uint32_t systemFreq = clockFrequency/4;

    //SETUP
    CLASSB_ClockTestSetup();

    CLASSB_timerCount = ((uint32_t)msec*referenceFrequency)/1000; //number of expected reference counts

    if(CLASSB_timerCount == 0)
    {
        CLASSB_timerCount = 1;
    }

    Clock_expectedCount = 0;

    Clock_expectedCount = (uint16_t)(((systemFreq*msec)/1000)/CYCLES); //number of expected system counts

    Clock_toleranceCount = ((Clock_expectedCount*tolerance)/100);///CYCLES);

    TMR1 = 0;
    CLASSB_ClockTestTimer();

    TMR1ON = 0;

    if(CLASSB_Clockcount == 0)
    {
        CLASSB_ClockTestRestore();
        return CLASSB_TEST_TIMEOUT;
    }

    if(CLASSB_Clockcount < (Clock_expectedCount - Clock_toleranceCount))
    {
        CLASSB_ClockTestRestore();
        return CLASSB_TEST_FAIL;
    }

    if(CLASSB_Clockcount > (Clock_expectedCount + Clock_toleranceCount))
    {
        CLASSB_ClockTestRestore();
        return CLASSB_TEST_FAIL;
    }

    CLASSB_ClockTestRestore();
    return CLASSB_TEST_PASS;

}

void CLASSB_ClockTestTimer (void)
{
#asm
    TimerTest:
        BANKSEL (_CLASSB_timerCount)
        MOVF BANKMASK(_CLASSB_timerCount)+1,W
        BANKSEL TMR1H
        NOP
        SUBWF TMR1H,W
        BTFSS STATUS, 0x2
        GOTO IncCounter
        NOP
        NOP
        NOP
        NOP
//        NOP
        BANKSEL _CLASSB_timerCount
        MOVF BANKMASK(_CLASSB_timerCount),W
        BANKSEL TMR1L
        SUBWF TMR1L, W
        BTFSC STATUS, 0x0
        GOTO TimerTestEnd
    IncCounter:
        BANKSEL (_CLASSB_Clockcount)
        INCF BANKMASK(_CLASSB_Clockcount),F
        BTFSC STATUS, 0x2
        INCF BANKMASK(_CLASSB_Clockcount)+1,F
        GOTO TimerTest
    TimerTestEnd:
        RETURN
#endasm
}

void CLASSB_ClockTestSetup(void)
{
    CLASSB_TRISC_temp = TRISC;
    CLASSB_INTCON_temp = INTCON;
    CLASSB_T1CON_temp = T1CON;
    CLASSB_PIE1_temp = PIE1;
    
    TRISC0 = 1;
    TRISC1 = 1;
    INTCON = 0;
    T1CON = 0b10001000;

    TMR1IF = 0;
    TMR1IE = 1;

    TMR1 = 0xFE00;
    TMR1ON = 1;

    while(!TMR1IF);
    
    TMR1IE = 0;
    TMR1IF = 0;
}

void CLASSB_ClockTestRestore(void)
{
    TRISC = CLASSB_TRISC_temp;
    INTCON = CLASSB_INTCON_temp;
    T1CON = CLASSB_T1CON_temp;
    PIE1 = CLASSB_PIE1_temp;
}
