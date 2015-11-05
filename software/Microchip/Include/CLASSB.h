/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB.h
* Dependencies:     CLASSB_config.h
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     Header file for CLASS B Library.
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
* Corey Simoncic    04/19/2013  Updated for XC-
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#ifndef _CLASSB_H_
#define _CLASSB_H_

#include "CLASSB_config.h"
#include "CLASSB_Types.h"

#ifdef CLASSB_CPUPC_TEST_ENABLED
#include "CLASSB_CPUPCTest.h"
#endif
#ifdef CLASSB_MARCHC_TEST_ENABLED
#include "CLASSB_RAMMARCHCTest.h"
#endif
#ifdef CLASSB_MARCHC_STACK_TEST_ENABLED
#include "CLASSB_RAMMarchCStackTest.h"
#endif
#ifdef CLASSB_MARCHB_TEST_ENABLED
#include "CLASSB_RAMMarchBTest.h"
#endif
#ifdef CLASSB_CPUREG_TEST_ENABLED
#include "CLASSB_CPURegistersTest.h"
#endif
#ifdef CLASSB_CHECKER_TEST_ENABLED
#include "CLASSB_RAMCheckerboardTest.h"
#endif
#ifdef CLASSB_CRCFLASH_TEST_ENABLED
#include "CLASSB_CRCFlashTest.h"
#endif
#ifdef CLASSB_CRCEEPROM_TEST_ENABLED
#include "CLASSB_CRCEEPROMTest.h"
#endif
#ifdef CLASSB_CLOCK_TEST_ENABLED
#include "CLASSB_ClockTest.h"
#endif
#ifdef CLASSB_LINE_TEST_ENABLED
#include "CLASSB_ClockLineFreqTest.h"
#endif

#endif