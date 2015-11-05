/**********************************************************************
* © 2013 Microchip Technology Inc.
*
* Project Name:     Class B Library
* FileName:         main.c
* Dependencies:     CLASSB_ClockLineFreqTest.h, CLASSB_ClockLineFreqTest.c
* Processor:        PIC16F1xxx
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     This file contains the main function to demonstrate
*                  the use of the CLASSB Clock Line Frequency Test.
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
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = ON      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

/****************************************************************************
*    Variable definitions
*****************************************************************************/
uint8_t TMR1RollOver = 0;
bool FIRSTCROSS = 0;
uint8_t lineTestEnd = 0;
uint32_t testCount = 0;
uint16_t captureTimer1;
//uint8_t lineFrequency = 96;

uint8_t ClockTestResult = 0;

CLASSBRESULT CheckClockLineTestFlag(void);
void OnClockError (void); //Example error function delaration.

void Init (void)
{
    OSCCON = 0b01111000; //16 MHz
    TRISA = 0b00000000;
    TRISB = 0;
    TRISC = 0b00000100;
    TRISD = 0;
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    //T1CON = 0b10001000; //T1OSC, dedicated osc enabled.
}

void main(void)
{
    Init();
    //volatile unsigned char testResult;
    uint32_t clockFrequency = 16000000;
    uint8_t lineFrequency = 60;

    uint8_t tolerance = 5;

    //For Function Pointer
    //ClockLineFreqTestFail = &OnClockError;

    //LATA4 = 1;

    CLASSB_ClockLineFreqTest(clockFrequency, lineFrequency, tolerance);

    // CustomerCode(){
    while(1)
    {
        ClockTestResult = CheckClockLineTestFlag();
        if (ClockTestResult == CLASSB_TEST_FAIL)
        {
            OnClockError();
        }
        //Customer Code
    }
}

CLASSBRESULT CheckClockLineTestFlag(void)
{
    if (ClockLineFreqTestFlag == CLASSB_TEST_FAIL)
    {
        LATA0 = 1;
        return CLASSB_TEST_FAIL;
    }
    if (ClockLineFreqTestFlag == CLASSB_TEST_PASS)
    {
        LATA1 = 1;
        return CLASSB_TEST_PASS;
    }
    else
    {
        LATA2 ^= 1;
        return CLASSB_TEST_INPROGRESS;
    }
}

void OnClockError (void)
{
    SLEEP();
    NOP();
}

void interrupt ISR (void)
{
    if(ClockLineFreqTestFlag == CLASSB_TEST_INPROGRESS)
    {
        LATA4 ^= 1;
        CLASSB_ClockLineFreqISR();
    }
}
