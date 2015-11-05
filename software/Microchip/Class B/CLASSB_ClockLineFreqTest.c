/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         CLASSB_ClockLineFreqTest.c
* Dependencies:     CLASSB_ClockLineFreqTest.h
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
* Corey Simoncic    03/25/2013  Updated for XC-8
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "CLASSB_ClockLineFreqTest.h"

uint8_t InterruptsUser; //INTCON
uint8_t Timer1User;     //T1CON
uint8_t Timer1GateUser; //T1GCON
uint8_t PIE1User;       //PIE1
uint8_t PIR1User;       //PIR1
uint8_t CCPUser;        //CCP1CON

uint8_t TMR1RollOver;
bool FIRSTCROSS;
uint8_t lineTestEnd;
uint32_t testCount;
uint16_t captureTimer1;

uint8_t ClockLineFreqTestFlag; //Test result flag
uint32_t expectedCount = 0; //Expected number of counts
uint32_t toleranceCount = 0; //The number of counts allowed due to the tolerance

uint8_t CLASSB_Line_Freq = 0; //global line frequency variable for use in the interrupt

/******************************************************************************
* Description:
*   The Clock test implements the independent time slot monitoring H.2.18.10.4
*   defined by the IEC 60730 standard. It verifies the reliability of the system
*   clock (i.e., the system clock should be neither too fast nor too slow)
*
* Input:
*   clockFrequency      - system clock frequency.
*   referenceFrequency  - reference clock frequency.
*   tolerance           - the tolerance level of the system oscillator.
*
* Return Values:
*     None.
******************************************************************************/
void CLASSB_ClockLineFreqTest(uint32_t clockFrequency, uint8_t lineFrequency, uint8_t tolerance)
{
    uint32_t systemFrequency = clockFrequency/4;

    //The line frequency must be a global to be used in the interrupt.
    CLASSB_Line_Freq = lineFrequency;

    //Save user initialization.
    CLASSB_LineUserSave();
    
    ClockLineFreqTestFlag = CLASSB_TEST_INPROGRESS;
            
    //Initialize modules for Line Frequency Test
    LineFreqInit();

    //The test will always be 1 second.
    //This makes the expected count the system frequency divided by the line frequency.
    expectedCount = systemFrequency/CLASSB_Line_Freq;
    toleranceCount = ((uint32_t)tolerance * expectedCount)/100;
}

void CLASSB_ClockLineFreqISR (void)
{
    //The TMR1 interrupt creates a multiplication of the timer counts for rollovers above 0xFFFF
    if (TMR1IF)
    {
        TMR1RollOver++;
        TMR1IF = 0;
        if(TMR1RollOver > 10)
        {
            ClockLineFreqTestFlag = CLASSB_TEST_TIMEOUT;
        }
    }
    //The Capture module will store the TMR1 value in the CCPR when an event occurs.
    //This uses that value to compare it to the expected count found above to determine
    //if the clock is correct.
    if (CCPXIF)
    {
        TMR1 = 0;
        CCPXIF = 0;
        //This disregards the first crossover because it is unknown how long it has been in the period.
        if(!FIRSTCROSS)
        {
            TMR1RollOver = 0;
            FIRSTCROSS = 1;
        }
        else
        {
            captureTimer1 = (uint16_t)(CCPRXH << 8) + CCPRXL;
            if(!lineTestEnd)
            {
                TMR1RollOver <<= 16;
                testCount = (captureTimer1+TMR1RollOver); //This effectively adds the Rollover values to the current test count
            }
            testCount = (testCount + (captureTimer1+(0x10000 * TMR1RollOver)))/2;
            CCPRXL = 0;
            CCPRXH = 0;
            lineTestEnd++;
            TMR1RollOver = 0;
            ClockLineFreqTestFlag = CLASSB_TEST_INPROGRESS;
        }
        //The test will end when the lineTestEnd counter reaches the line frequency.
        if(lineTestEnd >= CLASSB_Line_Freq)
        {
            if (testCount < (expectedCount - toleranceCount))
            {
                CLASSB_LineUserRestore();//Restore user initialization.
                ClockLineFreqTestFlag = CLASSB_TEST_FAIL;
                if (ClockLineFreqTestFail != NULL) //used if function pointer is enabled
                {
                    ClockLineFreqTestFail();
                }

            }
            else if (testCount > (expectedCount + toleranceCount))
            {
                CLASSB_LineUserRestore();//Restore user initialization.
                ClockLineFreqTestFlag = CLASSB_TEST_FAIL;
                if (ClockLineFreqTestFail != NULL) //used if function pointer is enabled
                {
                    ClockLineFreqTestFail();
                }
            }
            else
            {
                CLASSB_LineUserRestore();//Restore user initialization.
                ClockLineFreqTestFlag = CLASSB_TEST_PASS;
            }
        }
    }
    return;
}

void LineFreqInit (void)
{
    T1CON   = 0b00000000;  /*  TIMER1 CONTROL REGISTER 
                _______|_ TMR1ON    0       Stops Timer1
                ______|__ ----      0       Unimplemented
                _____|___ T1SYNC    0       This bit is ignored.
                ____|____ T1OSCEN   0       Dedicated Timer1 oscillator circuit disabled
                __||_____ T1CKPS    00      1:1 Prescale value
                ||_______ TMR1CS    00      Timer1 clock source is instruction clock (Fosc/4) */
    T1GCON = 0;
    TMR1 = 0; //reset TMR1H:TMR1L
    TMR1IF = 0; //reset TMR1 interrupt flag
    TMR1IE = 1; //enable TMR1 interrupts

    CCPXCON = 0b00000100;  /*  CCP1 CONTROL REGISTER
                ____||||_ CCP1M     0100    Capture Mode: every falling edge
                __||_____ DC1B      00      Unused
                ||_______ P1M       00      Unused */
    CCPXIF = 0; //reset CCP1 interrupt flag
    CCPXIE = 1; //enable CCP1 interrupts

    INTCON = 0xC0; //enable global and peripheral interrupts

    TMR1ON = 1; //Start Timer1
}

void CLASSB_LineUserSave (void)
{
    InterruptsUser = INTCON;
    CCPUser = CCPXCON;
    Timer1User = T1CON;
    Timer1GateUser = T1GCON;
    PIE1User = PIE1;
    PIR1User = PIR1;
}

void CLASSB_LineUserRestore (void)
{
    INTCON = InterruptsUser;
    CCPXCON = CCPUser;
    T1CON = Timer1User;
    T1GCON = Timer1GateUser;
    PIE1 = PIE1User;
    PIR1 = PIR1User;
}