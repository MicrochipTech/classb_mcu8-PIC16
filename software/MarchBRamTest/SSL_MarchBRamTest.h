/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        SSL_MarchBRamTest.h
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC16FXXX
* Compiler:        HI-TECH 9.81
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
#ifndef __SSL_MARCHB_RAM_TEST_H__
#define __SSL_MARCHB_RAM_TEST_H__


#define bitset(var,bitno) ((var) |= 1UL < < (bitno))
#define bitclr(var,bitno) ((var) &= ~(1UL < < (bitno)))


/****************************************************************
* The stack Start Address from which the March C test is to be
* performed. This address must be properly aligned according to
* the data width.                                              
****************************************************************/
#define MARCHC_STACK_START_ADDRESS  0X20	

/*Stack size in bytes.*/		 
#define MARCHC_STACK_LENGTH         0X0F 




#define PASS 1

#define FAIL 0


/****************************************************************
* The Start Address from which the March B test is to be
* performed. This address must be properly aligned according to
* the data width.                                              
*****************************************************************/
#define MARCHB_RAM_START_ADDRESS 0X50     


/****************************************************************
* The Stop Address to which the March B test is to be
* performed. This address must be properly aligned according to
* the data width.                                              
*****************************************************************/                                                                                                                                    
#define MARCHB_RAM_STOP_ADDRESS  0x54     


#define MARCHB_BIT_WIDTH   8               /*The Data width is 8 bits for PIC16, PIC12*/

#define MARCHB_RAM_TEST_PASS 1             /*ONE if the Test passes.*/
#define MARCHB_RAM_TEST_FAIL 0             /*ZERO if the Test Fails.*/


//Function Declarations
unsigned char SSL_RAMtest_MarchB(void);  
unsigned char Rd0Wr1Rd1Wr0Rd0Wr1 ( volatile unsigned char *test_data );
unsigned char Rd1Wr0Wr1( volatile unsigned char *test_data);
unsigned char Rd1Wr0Wr1Wr0( volatile unsigned char *test_data);
unsigned char Rd0Wr1Wr0 (volatile unsigned char *test_data);
#endif
