/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         CLASSB_CRCEEPROMTest.c
* Dependencies:     CLASSB_CRCEEPROMTest.h
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains functions to test EEPROM with CRC Test.
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
* Corey Simoncic    02/21/2013  Updated for XC-8
* Willem J Smit     10/20/2014  RamMarchBTest and RamCheckerBoardTest update
*
* Version       3.01
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "CLASSB_CRCEEPROMTest.h"
#include "CLASSB_CRCbyte.h"
#include "CLASSB_Types.h"

  /********************************************************************
  * Description:
  *     This function computes the periodic checksum using the Cyclic Redundancy
  *     Check (CRC). It detects the single bit Faults in the invariable memory.
  *     This function returns the final CRC Value.
  *     CRC-16 is used as a divisor.
  *     
  *     CRC-16 = 1 1000 0000 0000 0101= 8005(hex)
  * Input:
  *     startAddress :  start Address from which the CRC needs to be calculated
  *     length       :  the number of EEPROM addresses to be tested
  *     crcSeed      :  the initial value of the CRC calculation
  *
  * Return Values:
  *     testResult   :  Returns the final CRC result.
  ********************************************************************/
uint16_t CLASSB_CRCEEPROMTest(uint8_t startAddress, size_t length, uint16_t crcSeed)
{
    volatile unsigned int testResult;
    int i;
    uint8_t dataSequence;
    crcreg = crcSeed;

    for(i=0; i<length; i++)
    {
        dataSequence = CLASSB_EEPROMRead(startAddress+i);
        crcByte(dataSequence);
    }
    testResult = crcreg;
    return testResult;
}

/******************************************************************************
* Description:
*     This function reads the EEPROM at the startAddress.
* Input:
*     addr      :  the address of the EEPROM memory to be tested
* Returns:
*     EEDAT     :  The data in the EEPROM address of addr
*
******************************************************************************/
uint8_t CLASSB_EEPROMRead(uint8_t addr)
{
    EEADR = addr;

    EECON1bits.CFGS = 0;    // Do not select Configuration Space
    EECON1bits.EEPGD = 0;   // Select EEPROM
    EECON1bits.RD = 1;      // Initiate read
    NOP();                  //
    NOP();                  //

    return EEDAT;
}