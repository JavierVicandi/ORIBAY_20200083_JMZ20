/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : EI2C1.h
**     Project     : TM3
**     Processor   : MC9S12ZVLS16MFM
**     Component   : SW_I2C
**     Version     : Component 01.121, Driver 03.31, CPU db: 3.00.000
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-07-27, 18:31, # CodeGen: 18
**     Abstract    :
**         This device "SW_I2C" implements an external I2C
**         communication interface. It uses two general-purpose
**         I/O pins.
**     Settings    :
**         Serial Clock (SCL) pin      :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       27            |  PP5_XIRQ_KWP5_PWM5
**             ----------------------------------------------------
**
**         Serial Data (SDA) pin       :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       28            |  PP7_KWP7_PWM7_IOC1_0
**             ----------------------------------------------------
**
**
**         Mode                        : MASTER
**         Communication Speed         : FAST MODE
**         Output Buffer Size          : 0
**         Auto Stop Condition         : yes
**         Acknowledge Polling Trials  : 2000
**
**         Speed modes
**
**             High Speed Mode         : Enabled
**             Low Speed Mode          : Disabled
**             Slow Speed Mode         : Disabled
**
**         Initialization
**
**             Slave address           : 68
**             Component function      : Enabled
**             Events                  : Enabled
**     Contents    :
**         SendChar    - byte EI2C1_SendChar(byte Chr);
**         RecvChar    - byte EI2C1_RecvChar(byte *Chr);
**         SendBlock   - byte EI2C1_SendBlock(void* Ptr, word Siz, word *Snt);
**         RecvBlock   - byte EI2C1_RecvBlock(void* Ptr, word Siz, word *Rcv);
**         SelectSlave - byte EI2C1_SelectSlave(byte Slv);
**         GetSelected - byte EI2C1_GetSelected(byte *Slv);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file EI2C1.h
** @version 03.31
** @brief
**         This device "SW_I2C" implements an external I2C
**         communication interface. It uses two general-purpose
**         I/O pins.
*/         
/*!
**  @addtogroup EI2C1_module EI2C1 module documentation
**  @{
*/         

#ifndef __EI2C1
#define __EI2C1

/* MODULE EI2C1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "Cpu.h"

/*lint -esym(522,Delay) Disable MISRA rule (14.2) checking for symbols (Delay). */


void EI2C1_Init(void);
/*
** ===================================================================
**     Method      :  EI2C1_Init (component SW_I2C)
**
**     Description :
**         Initializes the associated peripheral(s) and the components 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

byte EI2C1_SendChar(byte Chr);
/*
** ===================================================================
**     Method      :  EI2C1_SendChar (component SW_I2C)
**     Description :
**         When working as a MASTER, this method writes 7 bits of slave
**         address plus R/W bit = 0 to the I2C bus and then writes one
**         character (byte) to the bus. The slave address must be
**         specified before by the "SelectSlave" method or in the
**         component initialization section of <Slave address init>
**         property. When working as the SLAVE, this method writes one
**         character (byte) to the bus. If the ERR_NOTAVAIL error code
**         returned, the char is successfully sent to master but the
**         master device responds by an acknowledgement instead of no
**         acknowledgement at the end of transfer. <OnError> event is
**         called in this case.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledgement
**                           (MASTER mode only)
**                           ERR_NOTAVAIL - The master device sends
**                           acknowledgement instead of no
**                           acknowledgement after the last byte
**                           transfer (SLAVE mode only)
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSOFF - Clock timeout elapsed (SLAVE
**                           mode only)
** ===================================================================
*/

byte EI2C1_RecvChar(byte *Chr);
/*
** ===================================================================
**     Method      :  EI2C1_RecvChar (component SW_I2C)
**     Description :
**         When working as a MASTER, this method writes 7 bits of slave
**         address plus R/W bit = 1 to the I2C bus and then reads one
**         character (byte) from the bus. The slave address must be
**         specified before by the "SelectSlave" method or in the
**         component initialization section of <Slave address init>
**         property. When working as a SLAVE, this method reads one
**         character (byte) from the bus.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledgement
**                           (MASTER mode only)
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSOFF - Clock timeout elapsed (SLAVE
**                           mode only)
** ===================================================================
*/

byte EI2C1_SendBlock(const void* Ptr,word Siz,word *Snt);
/*
** ===================================================================
**     Method      :  EI2C1_SendBlock (component SW_I2C)
**     Description :
**         When working as a MASTER, this method writes 7 bits of slave
**         address plus R/W bit = 0 to the I2C bus and then writes the
**         block of characters to the bus. If the component is disabled
**         (by the "Disable" method or initialization), the block is
**         moved to the output buffer if the <Output buffer size>
**         property is different from zero. The content of the output
**         buffer is sent immediately with the stop condition at the
**         end when the component is enabled by the "Enable" method.
**         After the output buffer transmission, the whole output
**         buffer is cleared. The slave address must be specified
**         before by the "SelectSlave" method or in component
**         initialization section of <Slave address init> property.
**         When working as a SLAVE, this method writes block of
**         characters to the bus. If the ERR_NOTAVAIL error code is
**         returned, the whole block is successfully sent to a master
**         but the master device responds by an acknowledgement instead
**         of no acknowledgment sent at the end of the last block byte
**         transfer. <OnError> event is called in this case.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - A pointer to the block of data to send
**         Siz             - The size of the block
**       * Snt             - A pointer to the number of data that are
**                           sent (copied to buffer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledgement
**                           (MASTER mode only)
**                           ERR_TXFULL - Output buffer is full (MASTER
**                           mode only)
**                           ERR_NOTAVAIL - The master device sends
**                           acknowledgement instead of no
**                           acknowledgement after the last byte
**                           transfer (SLAVE mode only)
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSOFF - Clock timeout elapsed (SLAVE
**                           mode only)
** ===================================================================
*/

byte EI2C1_RecvBlock(void* Ptr,word Siz,word *Rcv);
/*
** ===================================================================
**     Method      :  EI2C1_RecvBlock (component SW_I2C)
**     Description :
**         When working as a MASTER, this method writes 7 bits of slave
**         address plus R/W bit = 1 to the I2C bus and then reads the
**         block of characters from the bus. The slave address must be
**         specified before by the "SelectSlave" method or in component
**         initialization section of <Slave address init> property.
**         When working as a SLAVE, this method reads the block of
**         characters from the bus.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - A pointer to the block space for received
**                           data
**         Siz             - The size of the block
**       * Rcv             - A pointer to the number of actually
**                           received data
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledgement
**                           (MASTER mode only)
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSOFF - Clock timeout elapsed (SLAVE
**                           mode only)
** ===================================================================
*/

byte EI2C1_SelectSlave(byte Slv);
/*
** ===================================================================
**     Method      :  EI2C1_SelectSlave (component SW_I2C)
**     Description :
**         When working as a MASTER, this method selects a new slave
**         for communication by its slave address value. Any send or
**         receive method are directed to or from the selected device,
**         until a new slave device is selected by this method. If the
**         selected slave uses 10-bit slave addressing, as the
**         parameter 7 bits must be passed, which involves 10-bit
**         addressing (11110XX), including two MSBs of slave address
**         (XX). The second byte of the 10-bit slave address must be
**         sent to the slave as a general character of send methods.
**         This method is available only if the component is in MASTER
**         mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Slv             - Slave address value
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
** ===================================================================
*/

byte EI2C1_GetSelected(byte *Slv);
/*
** ===================================================================
**     Method      :  EI2C1_GetSelected (component SW_I2C)
**     Description :
**         When working as a MASTER, this method returns the
**         identification address value of the slave, which is
**         currently selected for communication with the master. If the
**         current slave uses 10-bit slave addressing, the method
**         returns the first 7 bits only, which involves 10-bit
**         addressing (11110XX), including two MSBs of the slave
**         address (XX). This method is not able to return the rest
**         value of 10-bit slave address. This method is available only
**         if the component is in MASTER mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Slv             - A pointer to the current selected slave
**                           address value
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/


/* END EI2C1. */

#endif /* ifndef __EI2C1 */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS12Z series of microcontrollers.
**
** ###################################################################
*/
