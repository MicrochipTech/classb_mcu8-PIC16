/**********************************************************************
 * © 2013 Microchip Technology Inc.
 *
 * Project Name:     Class B Library
 * FileName:         main.c
 * Dependencies:     CLASSB_CRCFlashTest.h, CLASSB_CRCFlashTest.c
 * Processor:        PIC16F1xxx
 * Compiler:         XC8
 * IDE:              MPLAB® IDE or MPLAB® X
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Description:     This file contains the main function to demonstrate
 *                  the use of the CLASSB CRC Flash Test.
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
 * Corey Simoncic    04/19/2013  Updated for XC-8
 *
 * Version       3.00
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ADDITIONAL NOTES:
 *
 **********************************************************************/
#include "CLASSB_config.h"
#include "CLASSB.h"


// PIC12LF1822 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

const uint8_t myFlashVariable @ 0x054 = 0x01; //Demo Flash data
const uint8_t myHiddenVariable1 @ 0x055 = 0x23; //Demo Flash data
const uint8_t myHiddenVariable2 @ 0x056 = 0x45; //Demo Flash data
const uint8_t myHiddenVariable3 @ 0x057 = 0x67; //Demo Flash data
const uint8_t myHiddenVariable4 @ 0x058 = 0x89; //Demo Flash data
const uint8_t myHiddenVariable5 @ 0x059 = 0x01; //Demo Flash data
const uint8_t myHiddenVariable6 @ 0x05A = 0x23; //Demo Flash data
const uint8_t myHiddenVariable7 @ 0x05B = 0x45; //Demo Flash data
const uint8_t myHiddenVariable8 @ 0x05C = 0x67; //Demo Flash data
const uint8_t myHiddenVariable9 @ 0x05D = 0x89; //Demo Flash data

#define _XTAL_FREQ   16000000  //Used by the XC8 delay_ms(x) macro
#define __delay_us(t) _delay((unsigned long)((t)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(t) _delay((unsigned long)((t)*(_XTAL_FREQ/4000.0)))

void long_delay_ms(long count);

void main(void) {
    //    const uint16_t* myAddress = (uint16_t*)&myFlashVariable;
    OSCCON = 0b01111000;
    uint16_t myAddress;
    myAddress = 0x00;

    uint16_t length = 0x2048;
    uint16_t crcSeed = 0xFFFF;
    uint16_t testResult;

    TRISA = 0;
    ANSELA = 0;
    LATA = 0xFF;

    testResult = CLASSB_CRCFlashTest(myAddress, length, crcSeed);

    if (testResult) {
        while (1) {
            LATA2 ^= 1;
            for(uint8_t i = 0; i < 60; i++){
            long_delay_ms(i);
            LATA5 ^= 1; /* Test pass */
        }
            for(uint8_t i = 60; i > 0; i--){
            long_delay_ms(i);
            LATA5 ^= 1; /* Test pass */
        }
        }
    }
    else
        while (1) {
            __delay_ms(50);
            LATA2 ^= 1; /* Test not pass */
        }
}

void long_delay_ms(long count)
{
long k;

for(k = 0; k < count; k++)
    __delay_us(500);
}