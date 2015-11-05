/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB_ClockTest.h
* Dependencies:     none
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     Header file for Clock test.
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
* Corey Simoncic    04/19/2013  Updated for XC-8 PRO v1.12
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#ifndef _CLASSB_CLOCK_
#define _CLASSB_CLOCK_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xc.h>
#include "CLASSB_Types.h"

#define CYCLES 15 //number of instructions each count
extern uint16_t CLASSB_timerCount;
extern uint16_t CLASSB_Clockcount;

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_ClockTest(clockFrequency, referenceFrequency, msec, tolerance)

  Summary:
    Tests the system clock using an external crystal into Timer1.

  Precondition:
     An external oscillator is connected to Timer1.

  Parameters:
    clockFrequency      - system clock frequency.
    referenceFrequency  - reference clock frequency.
    msec                - the time in milliseconds to run the test.
    tolerance           - the tolerance level of the system oscillator.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
     Recommend a 20 ms test time for best results.
     Will not work with a reference frequency higher than 2MHz.

  ***************************************************************************/
//Function Declarations
CLASSBRESULT CLASSB_ClockTest(uint32_t clockFrequency, uint32_t referenceFrequency, size_t msec, uint8_t tolerance);
void CLASSB_ClockTestSetup(void);
void CLASSB_ClockTestRestore(void);
void CLASSB_ClockTestTimer(void);
#endif
