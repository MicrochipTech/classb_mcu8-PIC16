/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:           SSL_EEPROM_CRC.h
* Dependencies:    Header (.h) files if applicable, see below
* Processor:          PIC16FXXX
* Compiler:           HI-TECH 9.81
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.*/
/**************************************************************************************/


#ifndef __SSL_EEPROM_CRC_H__
#define __SSL_EEPROM_CRC_H__

#define EEPROM_STARTADDRESS   	0x00
#define EEPROM_ENDADDRESS     	0x03

#define EEPROM_GEN_POLY 0x8005   /*The Generator Polynomial: x^16 + x^15 + x^2 + 1*/

#define	crc_Result_CONST 0x0924	 /* EE-Prom CRC result constant */

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef short          		 Word16;
typedef unsigned short	 UWord16;
typedef long          		 Word32;
typedef unsigned long  	 UWord32;

typedef union 
{
	UWord32 Val32;

	struct
	{
		UWord16 LW;
		UWord16 HW;
	} Word;

	char Val[4];
} uReg32;

#endif


//Function Declarations

unsigned int SSL_8bits_EEPROMtest_CRC16(unsigned char,unsigned char );
void EEPROM_CRC_Checksum(void);
#endif
