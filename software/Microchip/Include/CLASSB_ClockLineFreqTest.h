/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB_ClockLineFreqTest.h
* Dependencies:     none
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     Header file for Clock Line Frequency test.
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
#ifndef _CLASSB_LINE_
#define _CLASSB_LINE_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xc.h>
#include "CLASSB_Types.h"
#include "CLASSB_config.h"

/****************************************************************************
*    Affected Registers save variables:
*****************************************************************************/
extern uint8_t ClockLineFreqTestFlag; //Test result flag
extern uint32_t expectedCount; //Expected number of counts
extern uint32_t toleranceCount; //The number of counts allowed due to the tolerance

void (*ClockLineFreqTestFail) (void) = NULL; //Function Pointer declaration.

extern uint8_t TMR1RollOver;
extern bool FIRSTCROSS;
extern uint8_t lineTestEnd;
extern uint32_t testCount;
extern uint16_t captureTimer1;
extern uint8_t lineFrequency;

//Used to add customization for user
#ifdef LINE_FREQ_TEST_CCP1
#define CCPRXL CCPR1L
#define CCPRXH CCPR1H
#define CCPXIF CCP1IF
#define CCPXIE CCP1IE
#define CCPXCON CCP1CON
#endif
#ifdef LINE_FREQ_TEST_CCP2
#define CCPRXL CCPR2L
#define CCPRXH CCPR2H
#define CCPXIF CCP2IF
#define CCPXIE CCP2IE
#define CCPXCON CCP2CON
#endif
#ifdef LINE_FREQ_TEST_CCP3
#define CCPRXL CCPR3L
#define CCPRXH CCPR3H
#define CCPXIF CCP3IF
#define CCPXIE CCP3IE
#define CCPXCON CCP3CON
#endif
#ifdef LINE_FREQ_TEST_CCP4
#define CCPRXL CCPR4L
#define CCPRXH CCPR4H
#define CCPXIF CCP4IF
#define CCPXIE CCP4IE
#define CCPXCON CCP4CON
#endif
#ifdef LINE_FREQ_TEST_CCP5
#define CCPRXL CCPR5L
#define CCPRXH CCPR5H
#define CCPXIF CCP5IF
#define CCPXIE CCP5IE
#define CCPXCON CCP5CON
#endif

/****************************************************************************
  Function:
    void CLASSB_ClockLineFreqTest(clockFrequency, lineFrequency, tolerance)

  Summary:
    Tests the system clock using the line frequency into the Capture Module (CCP).

  Precondition:
     Zero Cross detection circuit is the input to one of the CCP inputs.

  Parameters:
    clockFrequency      - system clock frequency.
    referenceFrequency  - reference clock frequency.
    tolerance           - the tolerance level of the system oscillator.

  Returns:
     None

  Result:
     The result of this test can be accessed two ways:
     1. Through a Function pointer declared above as *ClockLineFreqTestFail or...
     2. Using the ClockLineFreqTestFlag which will follow the definition of CLASSBRESULT

  Remarks:
     This is a timing critical test. 
     Changes to TMR1 during the progress of this test will cause the test to fail.
     This test takes one second.

  ***************************************************************************/
//Function Declarations
void CLASSB_ClockLineFreqTest(uint32_t clockFrequency, uint8_t lineFrequency, uint8_t tolerance);
//void interrupt ClockTestInt (void);
void CLASSB_LineUserSave (void);
void CLASSB_LineUserRestore (void);
void LineFreqInit (void);
void CLASSB_ClockLineFreqISR (void);
#endif
