/**********************************************************************
* © 2013 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         AllTestsMain.c
* Dependencies:     
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains the main function to demonstrate
*		   the use of the Microchip CLASSB library.
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
*
* Version       3.00
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "CLASSB_config.h"
#include "CLASSB.h"

// PIC16F1937 Configuration Bit Settings

#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

void ErrorMode(void);

void interrupt ISR (void)
{
    if(ClockLineFreqTestFlag == CLASSB_TEST_INPROGRESS)
    {
        CLASSB_ClockLineFreqISR();
    }
}

void Init()
{
    OSCCON = 0b01111000; //16 MHz
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    TRISC2 = 1; //This is for the zero-cross on CCP1
    TRISC0 = 1; //These are for T1OSC
    TRISC1 = 1;
    T1CON = 0b10001000; //T1OSC, dedicated osc enabled.
}

void main()
{
    Init();

    volatile unsigned char testResult;

/******************************************************************************
*   Program Counter Test.
******************************************************************************/
    CLASSBRESULT  ProgramCounterResult = CLASSB_TEST_INPROGRESS;
    ProgramCounterResult = CLASSB_CPUPCTest();

    if (ProgramCounterResult == CLASSB_TEST_PASS)
    {
        asm("nop");		/* Test pass */
    }
    else
    {
        ErrorMode();		/* Test not pass */
    }

/******************************************************************************
*   CPU Registers Test.
******************************************************************************/
    //Do a check to see if the Program Counter did not error.
    if (ProgramCounterResult == CLASSB_TEST_INPROGRESS)
        ErrorMode();

    testResult = CLASSB_CPURegistersTest();

    if (testResult == CLASSB_TEST_PASS)
    {
        asm("nop");		/* Test pass */
    }
    else
    {
        ErrorMode();		/* Test not pass */
    }

/******************************************************************************
*   Clock Line Frequency Test.
******************************************************************************/
    uint32_t clockFrequency = 16000000; //system clock: 16MHz
    uint32_t lineFrequency = 60; //reference clock: 60Hz zero-cross
    uint8_t tolerance = 5; //Tolerance level: 5%

    CLASSB_ClockLineFreqTest(clockFrequency, lineFrequency, tolerance);

//For this demo the code will wait for the test to finish.
//In application, code can be placed here as long as it is ok to be interrupted!
    while (ClockLineFreqTestFlag == CLASSB_TEST_INPROGRESS);

    if (ClockLineFreqTestFlag == CLASSB_TEST_PASS)
    {
        asm("nop");     /* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

/******************************************************************************
*   Clock Test.
******************************************************************************/
    uint32_t clockFrequency = 16000000; //system clock: 16MHz
    uint32_t referenceFrequency = 32768; //reference clock: 32.768kHz
//    uint32_t referenceFrequency = 29491; //reference clock: 32.768kHz; -10% error
    uint8_t msec = 20; //Test time: 20ms
    uint8_t tolerance = 5; //Tolerance level: 5%

    testResult = CLASSB_ClockTest(clockFrequency, referenceFrequency, msec, tolerance);

    if (testResult == CLASSB_TEST_PASS)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

/******************************************************************************
*   March C Test. (This example tests Bank 0)
******************************************************************************/
    CLASSB_MarchstartAddress = (char*)0x20;
    CLASSB_MarchbufferAddress = (char*)0xA0;
    CLASSB_MarchLength = 0x51;

    testResult = CLASSB_RAMMarchCTest();

    if (testResult==CLASSB_TEST_PASS)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

/******************************************************************************
*   March C Stack Test.
******************************************************************************/
    CLASSB_MarchbufferAddress = (char*)0x27;
    CLASSB_MarchLength = 33;

    testResult = CLASSB_RAMMarchCStackTest();

    if (testResult==CLASSB_TEST_PASS)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

/******************************************************************************
*   March B Test. (This example tests BANK 0)
******************************************************************************/
    CLASSB_MarchstartAddress = (char*)0x20;
    CLASSB_MarchbufferAddress = (char*)0xA0;
    CLASSB_MarchLength = 0x51;

    testResult = CLASSB_RAMMarchBTest();

    if (testResult==CLASSB_TEST_PASS)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

/******************************************************************************
*   Initial EEPROM CRC test.
******************************************************************************/    
    uint8_t EEPROMAddress = 0x00;
    size_t length = 255;
    uint16_t crcSeed = 0xFFFF;
    volatile unsigned int CLASSB_EEPROMtestResult;
    volatile uint16_t newCLASSB_EEPROMtestResult;

    CLASSB_EEPROMtestResult = CLASSB_CRCEEPROMTest(EEPROMAddress,length,crcSeed);
    
    EEPROMAddress = 0x00;
    length = 255;
    crcSeed = 0xFFFF;
    newCLASSB_EEPROMtestResult = CLASSB_CRCEEPROMTest(EEPROMAddress,length,crcSeed);

    if (CLASSB_EEPROMtestResult == newCLASSB_EEPROMtestResult)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }
    
    
/******************************************************************************
*   Initial Flash CRC test.
******************************************************************************/  
    uint16_t FlashAddress = 0x00;
    uint16_t Flashlength = 0x1000;
    crcSeed = 0xFFFF;
    volatile unsigned int CLASSB_FlashtestResult;
    volatile uint16_t newCLASSB_FlashtestResult;
    
    CLASSB_FlashtestResult = CLASSB_CRCFlashTest(FlashAddress,Flashlength,crcSeed);

    FlashAddress = 0x00;
    Flashlength = 0x1000;
    crcSeed = 0xFFFF;
    newCLASSB_FlashtestResult = CLASSB_CRCFlashTest(FlashAddress,Flashlength,crcSeed);

    if (CLASSB_FlashtestResult == newCLASSB_FlashtestResult)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

//The Check Values for the Flash and EEPROM can now be saved for future
//comparison or compared to a known constant, depending on the application

/******************************************************************************
*   Checkerboard Test
******************************************************************************/
    uint8_t* CheckerStartAddress;
    uint8_t* CheckerBufferAddress;
    uint8_t CheckerLength;

//In this example we show the checkerboard test used on RAM locations
//0x50 - 0x57. The Checkerboard should be used as a faster alternative
//to the March Tests during run-time. Safety specific RAM locations
//should be tested with this method.
    CheckerStartAddress = (char*)0x50;
    CheckerBufferAddress = (char*)0x58;
    CheckerLength = 8;

    testResult = CLASSB_RAMCheckerboardTest(CheckerStartAddress, CheckerLength, CheckerBufferAddress);

    if (testResult==CLASSB_TEST_PASS)
    {
        asm("nop");	/* Test pass */
    }
    else
    {
        ErrorMode();	/* Test not pass */
    }

/******************************************************************************
*   Application Code here! (All tests passed!)
******************************************************************************/
    while(1)
    {
        NOP();
    }

}

void ErrorMode (void)
{
/******************************************************************************
*   Put the device into a safe state here!
******************************************************************************/
    INTCON = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0xF;
    while(1);
}