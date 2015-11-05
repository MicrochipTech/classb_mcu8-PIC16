/**********************************************************************
* © 2013 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB_config.h
* Dependencies:     none
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
* Corey Simoncic    04/19/2013  Updated for XC-8
*
* Version       3.00
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/

/*************************************************************************
* CLASSB_DEVICE_FLASH_SIZE_KB
* Defines the size of the flash in K words.
* Used in CLASSB_CPUPCTest
* Valid sizes include: 2, 4, 8, 16
*************************************************************************/
//#define CLASSB_DEVICE_FLASH_SIZE_KB 8

/*************************************************************************
* MARCHCMINUS
* Defines if user wants to run the MarchCMinus test.
* Used in CLASSB_MarchCTest
* Uncomment to run redundant March C routine.
*************************************************************************/
//#define MARCHCMINUS

/*************************************************************************
* LINE_FREQ_TEST_CCP#
* Defines which CCP register to use for line frequency clock test.
* Used in CLASSB_ClockLineFreqTest
* Default to CCP1
* Please make sure the corresponding TRIS bit is set!
*************************************************************************/
//#define LINE_FREQ_TEST_CCP1
//#define LINE_FREQ_TEST_CCP2
//#define LINE_FREQ_TEST_CCP3
//#define LINE_FREQ_TEST_CCP4
//#define LINE_FREQ_TEST_CCP5

/*************************************************************************
* CLASSB test function descriptions and enables.
* Default is to enable all tests!
* Uncomment each test not used!
*************************************************************************/
/****************************************************************************
March B Test:

  Function:
    CLASSBRESULT CLASSB_RAMMarchBTest()

  Summary:
    Tests the RAM memory using March B algorithm.

  Precondition:
    Interrupts must be disabled.

  Parameters:
    CLASSB_MarchstartAddress  - the first address of the tested RAM memory,
    CLASSB_MarchLength        - the byte length of the tested RAM memory.
    CLASSB_MarchbufferAddress - the first address of the location in RAM to save user data

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts must be disabled during the test.
    If CLASSB_MarchbufferAddress is NULL the test is destructive.
    The test uses 7 bytes of RAM for global variables used in test.
  ***************************************************************************/
//#define CLASSB_MARCHB_TEST_ENABLED

/******************************************************************************
March C Test:

  Function:
    CLASSBRESULT CLASSB_RAMMarchCTest()

  Description:
    Tests the RAM memory region using March C.

  Precondition:
    Interrupts must be disabled.

  Parameters:
    CLASSB_MarchstartAddress  - the first address of the tested RAM memory,
    CLASSB_MarchLength        - the byte length of the tested RAM memory.
    CLASSB_MarchbufferAddress - the first address of the location in RAM to save user data
    MARCHCMINUS               - if the parameter is TRUE the "minus" algorithm is used.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts must be disabled during the test.
    If bufferAddress is NULL the test is destructive.
    The test uses 7 bytes of RAM for global variables used in test.
******************************************************************************/
//#define CLASSB_MARCHC_TEST_ENABLED

/****************************************************************************
March C Stack Test:

  Function:
    CLASSBRESULT CLASSB_RAMMarchCStackTest()

  Description:
    Tests the RAM stack memory using March C algorithm.

  Precondition:
    Interrupts must be disabled.

  Parameters:
    startAddress  - the first address of the tested RAM memory.
    length        - the byte length of the tested RAM memory.
    bufferAddress - the first address of the location in RAM to save user data.
    minus         - if the parameter is TRUE the "minus" algorithm is used.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts must be disabled during the test.
    To run the stack test a minimum of 33 bytes must be alocated for the bufferAddress.
***************************************************************************/
//#define CLASSB_MARCHC_STACK_TEST_ENABLED

/****************************************************************************
 Checkerboard Test:

  Function:
    CLASSBRESULT CLASSB_RAMCheckerboardTest(uint8_t* startAddress, size_t length, uint8_t* bufferAddress)

  Summary:
    Tests the RAM memory using checker board pattern.

  Precondition:
    Interrupts must be disabled.

  Parameters:
    startAddress - the first address of the tested RAM memory.
    length       - the byte length of the tested RAM memory.
    bufferAddress - the first address of the RAM memory to save user data.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    The length must be even.
    Interrupts must be disabled during the test.
    If bufferAddress is NULL the test is destructive.
***************************************************************************/
//#define CLASSB_CHECKER_TEST_ENABLED

/****************************************************************************
 CRC Flash Test:

  Function:
    uint16_t CLASSB_CRCFlashTest(const void* startAddress, size_t length, uint16_t crcSeed)

  Summary:
    Tests the flash memory using the Cyclic Redundancy Check (CRC).

  Parameters:
    startAddress  - the first address of the tested flash memory.
    length        - the number of flash locations tested.
    crcSeed       - the initial value of the CRC calculation.

  Returns:
    Returns the final CRC result.
***************************************************************************/
//#define CLASSB_CRCFLASH_TEST_ENABLED

/****************************************************************************
 CRC EEPROM Test:

  Function:
    uint16_t CLASSB_CRCEEPROMTest(uint8_t startAddress, size_t length, uint16_t crcSeed)

  Summary:
    Tests the EEPROM memory using the Cyclic Redundancy Check (CRC).

  Parameters:
    startAddress  - the first address of the tested EEPROM memory.
    length        - the number of EEPROM locations tested.
    crcSeed       - the initial value of the CRC calculation.

  Returns:
    Returns the final CRC result.
***************************************************************************/
#define CLASSB_CRCEEPROM_TEST_ENABLED

/****************************************************************************
CPU Registers Test:

  Function:
    CLASSBRESULT CLASSB_CPURegistersTest()

  Summary:
    Tests CPU registers.

  Description:
    Tests the following registers: WREG, BSR, FSR0, FSR1

  Precondition:
    Interrupts must be disabled.

  Parameters:
    None.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts must be disabled during the test.
***************************************************************************/
//#define CLASSB_CPUREG_TEST_ENABLED

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_CPUPCTest()

  Summary:
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
	Recommended use is in conjunction with watchdog timer. Software traps
	are implemented to catch runaway PC.
***************************************************************************/
//#define CLASSB_CPUPC_TEST_ENABLED

/****************************************************************************
  Function:
    void CLASSB_ClockLineFreqTest(uint32_t clockFrequency, uint8_t lineFrequency, uint8_t tolerance)

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
     This is a timing critical test. Changes to TMR1 during the progress of this test will cause the test to fail.
     This test takes one second.

***************************************************************************/
//#define CLASSB_LINE_TEST_ENABLED

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_ClockTest(uint32_t clockFrequency, uint32_t referenceFrequency, size_t msec, uint8_t tolerance)

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
//#define CLASSB_CLOCK_TEST_ENABLED