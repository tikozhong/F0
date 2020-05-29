/***************************************************************************//**
 *   @file   Main.c
 *   @brief  Implementation of the program's main function.
 *   @author Bancisor Mihai
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: 684
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "Communication.h"
#include "ADXL345.h"
#include "Console.h"
/******************************************************************************/
/*********************** Device configuration words ***************************/
/******************************************************************************/
_FOSCSEL(FNOSC_PRIPLL);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_XT);    /*!< Clock Switching Enabled and Fail Safe Clock Monitor is disabled*/
                                                    /*!< OSC2 Pin Function: OSC2 is Clock Output*/
                                                    /*!  Primary Oscillator Mode: XT Crystal*/
_FWDT(FWDTEN_OFF); 			            /*!< Watchdog Timer Enabled/disabled by user software*/

/******************************************************************************/
/************************ Constants Definitions *******************************/
/******************************************************************************/
/*! List of available commands */
const char* commandsList[] = {"help?",
                              "communication=",
                              "communication?",
                              "acceleration?",
                              "interrupts?"
                               };
const char* commandsDescription[] = {
    "  -  Displays all available commands.",
    "  -  Selects the communication interface. Accepted values: 0(I2C),1(SPI).",
    "  -  Displays the selected communication interface.",
    "  -  Displays the acceleration on XYZ axis.",
    "  -  Displays the state of the interrupts."};

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/
unsigned short	x	    = 0;    /*!< X-axis's output data.*/
unsigned short	y	    = 0;    /*!< Y-axis's output data.*/
unsigned short	z 	    = 0;    /*!< Z-axis's output data.*/
unsigned char	intSource   = 0;    /*!< Value of the ADXL345_INT_SOURCE register.*/

unsigned char commandsNumber = (sizeof(commandsList) / sizeof(const char*));
unsigned char receivedCommand[20];
unsigned char invalidCommand = 0;
unsigned char commandType = 0;
unsigned char command = 0;
unsigned char displayCommand = 0;
double        commandParam = 0;

extern unsigned char ADXL345_COMMUNICATION;
/***************************************************************************//**
 * @brief Main function.
 *
 * @return None.
*******************************************************************************/
int main(void)
{
    /*! Configure PLL prescaler, PLL postscaler, PLL divisor. */
    /*! Fosc = 80MHz, Fcy = 40MHz, 40MIPS. */
    PLLFBD = 38;            // M = PLLFBD + 2
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    CLKDIVbits.PLLPRE = 0;  // N1 = 2

    /*! Temporary variables */
    char  tempString [10]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /*! Wait for PLL to lock. */
    while(OSCCONbits.COSC != 0b011);
    while(OSCCONbits.LOCK != 1);

    /*! Initialize the UART communication peripheral. */
    UART_Init(9600);

    while(1)
    {
        CONSOLE_GetCommand(receivedCommand);
        invalidCommand = 0;
        for(command = 0; command < commandsNumber; command++)
        {
            commandType = CONSOLE_CheckCommands(receivedCommand,
                                                commandsList[command],
                                                (double*)&commandParam);
            if(commandType == 0)
            {
                invalidCommand++;
            }
            if((command == 0) && (commandType != 0))    /*!< "help?" command*/
            {
                CONSOLE_WriteString("Available commands:");
                UART_Write(0x0D);
                for(displayCommand = 0; displayCommand < commandsNumber;
                    displayCommand++)
                {
                    CONSOLE_WriteString(commandsList[displayCommand]);
                    CONSOLE_WriteString(commandsDescription[displayCommand]);
                    UART_Write(0x0D);
                }
            }
            if((command == 1) && (commandType != 0))   /*!< "communication=" command*/
            {
                if(commandParam == 0)
                {
                    ADXL345_COMMUNICATION = I2C_COMMUNICATION;
                }
                else if(commandParam == 1)
                {
                    ADXL345_COMMUNICATION = SPI_COMMUNICATION;
                }
                else
                {
                    if(commandParam < 0)
                    {
                        ADXL345_COMMUNICATION = I2C_COMMUNICATION;
                    }
                    else
                    {
                        ADXL345_COMMUNICATION = SPI_COMMUNICATION;
                    }
                }
                /*! Send feedback to user */
                CONSOLE_WriteString(commandsList[command]);
                if(ADXL345_COMMUNICATION == I2C_COMMUNICATION)
                {
                    CONSOLE_WriteString("I2C");
                }
                else
                {
                    CONSOLE_WriteString("SPI");
                }
                UART_Write(0xD);
                /*! Initialize device*/
                if(ADXL345_Init()) CONSOLE_WriteString("ADXL345 OK");
                else CONSOLE_WriteString("ADXL345 Error");
                UART_Write(0xD);
                /*! Configure device*/
                ADXL345_SetTapDetection(ADXL345_SINGLE_TAP |
                                        ADXL345_DOUBLE_TAP,	/*!< Tap type. */
                                        ADXL345_TAP_X_EN,	/*!< Axis control. */
                                        0x64,		/*!< Tap duration.--10 */
                                        0x20,		/*!< Tap latency.--10 */
                                        0x40,		/*!< Tap window. */
                                        0x10,		/*!< Tap threshold. */
                                        0x00);		/*!< Interrupt Pin. */
                ADXL345_SetFreeFallDetection(0x01,	/*!< Free-fall detection enabled. */
                                             0x05,	/*!< Free-fall threshold. */
                                             0x14,	/*!< Time value for free-fall detection. */
                                             0x00);	/*!< Interrupt Pin. */
                ADXL345_SetPowerMode(0x1);		/*!< Measure mode. */

            }
            if((command == 2) && (commandType != 0))   /*!< "communication?" command*/
            {
                /*! Send the requested value to user */
                CONSOLE_WriteString(commandsList[command - 1]);
                if(ADXL345_COMMUNICATION == I2C_COMMUNICATION)
                {
                    CONSOLE_WriteString("I2C");
                }
                else
                {
                    CONSOLE_WriteString("SPI");
                }                    
                UART_Write(0xD);
            }
            if((command == 3) && (commandType != 0))   /*!< "acceleration?" command*/
            {
                /*! Read the requested value from the device */
                ADXL345_GetXyz(&x,&y,&z);
                /*! Send the requested value to user */
                CONSOLE_WriteString("x axis: ");
                itoa(tempString, x, 10);
                CONSOLE_WriteString(tempString);
                UART_Write(0xD);
                CONSOLE_WriteString("y axis: ");
                itoa(tempString, y, 10);
                CONSOLE_WriteString(tempString);
                UART_Write(0xD);
                CONSOLE_WriteString("z axis: ");
                itoa(tempString, z, 10);
                CONSOLE_WriteString(tempString);
                UART_Write(0xD);
            }
            if((command == 4) && (commandType != 0))   /*!< "interrupts?" command*/
            {
                /*! Read the requested value from the device */
                intSource = ADXL345_GetRegisterValue(ADXL345_INT_SOURCE);
                /*! Send the requested value to user */
                if((intSource & ADXL345_SINGLE_TAP) != 0)
                {
                    CONSOLE_WriteString("Single Tap\n");
                }
                if((intSource & ADXL345_DOUBLE_TAP) != 0)
                {
                    CONSOLE_WriteString("Double Tap\n");
                }
                if((intSource & ADXL345_FREE_FALL) != 0)
                {
                    CONSOLE_WriteString("Free Fall\n");
                }
            }
            if(invalidCommand == commandsNumber)
            {
                /*! Send feedback to user */
                CONSOLE_WriteString("Invalid command");
                UART_Write(0x0D);
            }
        }
    }
}