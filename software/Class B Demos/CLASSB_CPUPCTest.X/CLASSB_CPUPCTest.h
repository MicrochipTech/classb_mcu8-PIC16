/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB_CPUPCTest.h
* Dependencies:     none
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     Header file for functions to test program counter.
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
**************************************************************************/

#ifndef _CPUPC_H_
#define _CPUPC_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <xc.h>
#include "CLASSB_Types.h"
#include "CLASSB_config.h"

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_CPUPCTest()

  Description:
    Tests CPU program counter.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    The device flash memory size must be set using parameter
    CLASSB_DEVICE_FLASH_SIZE in CLASSB_config.h.
  ***************************************************************************/
CLASSBRESULT CLASSB_CPUPCTest();

#if   (CLASSB_DEVICE_FLASH_SIZE_KB == 2)

#define PCTestAddr1 0x555
#define PCTestAddr2 0x2AA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 4)

#define PCTestAddr1 0x555
#define PCTestAddr2 0xAAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 8)

#define PCTestAddr1 0x1555
#define PCTestAddr2 0xAAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 16)

#define PCTestAddr1 0x1555
#define PCTestAddr2 0x2AAA

#else

#error "Valid CLASSB_DEVICE_FLASH_SIZE_KB must be defined in CLASSB_config.h"

#endif

void CLASSB_CPUPCTestFunction1(void) @ PCTestAddr1;
void CLASSB_CPUPCTestFunction2(void) @ PCTestAddr2;

void CLASSB_PCTrap1 (void) @ PCTestAddr1-1;
void CLASSB_PCTrap2 (void) @ PCTestAddr2-1;

#endif