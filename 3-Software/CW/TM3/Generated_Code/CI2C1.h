/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : CI2C1.h
**     Project     : TM3
**     Processor   : MC9S12ZVLS16MFM
**     Component   : InternalI2C
**     Version     : Component 01.287, Driver 01.22, CPU db: 3.00.000
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-07-15, 18:51, # CodeGen: 4
**     Abstract    :
**          This component encapsulates the internal I2C communication 
**          interface. The implementation of the interface is based 
**          on the Philips I2C-bus specification version 2.0. 
**          Interface features:
**          MASTER mode
**            - Multi master communication
**            - The combined format of communication possible 
**              (see "Automatic stop condition" property)
**            - 7-bit slave addressing (10-bit addressing can be made as well)
**            - Acknowledge polling provided
**            - No wait state initiated when a slave device holds the SCL line low
**            - Holding of the SCL line low by slave device recognized as 'not available bus'
**            - Invalid start/stop condition detection provided
**          SLAVE mode
**            - 7-bit slave addressing
**            - General call address detection provided
**     Settings    :
**         Serial channel              : IIC0
**
**         Protocol
**             Mode                    : MASTER
**             Auto stop condition     : yes
**             SCL frequency           : 86.806 kHz
**
**         Initialization
**
**             Target slave address    : 69
**             Component init          : Enabled
**             Events                  : Enabled
**
**         Registers
**             Input buffer            : IIC0IBDR  [0x07C4]
**             Output buffer           : IIC0IBDR  [0x07C4]
**             Control register        : IIC0IBCR  [0x07C2]
**             Status register         : IIC0IBSR  [0x07C3]
**             Baud setting reg.       : IIC0IBFD  [0x07C1]
**             Address register        : IIC0IBAD  [0x07C0]
**
**             Priority                : 
**
**         Used pins                   :
**       ----------------------------------------------------------
**            Function    | On package |    Name
**       ----------------------------------------------------------
**              SDA       |     14     |  PT0_IOC0_0_SDA0_RXD1_PWM2_LPTXD0
**              SCL       |     19     |  PT1_IOC0_1_SCL0_TXD1_PWM0_LPRXD0
**       ----------------------------------------------------------
**     Contents    :
**         SendChar    - byte CI2C1_SendChar(byte Chr);
**         RecvChar    - byte CI2C1_RecvChar(byte *Chr);
**         SendBlock   - byte CI2C1_SendBlock(void* Ptr, word Siz, word *Snt);
**         RecvBlock   - byte CI2C1_RecvBlock(void* Ptr, word Siz, word *Rcv);
**         SelectSlave - byte CI2C1_SelectSlave(byte Slv);
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
** @file CI2C1.h
** @version 01.22
** @brief
**          This component encapsulates the internal I2C communication 
**          interface. The implementation of the interface is based 
**          on the Philips I2C-bus specification version 2.0. 
**          Interface features:
**          MASTER mode
**            - Multi master communication
**            - The combined format of communication possible 
**              (see "Automatic stop condition" property)
**            - 7-bit slave addressing (10-bit addressing can be made as well)
**            - Acknowledge polling provided
**            - No wait state initiated when a slave device holds the SCL line low
**            - Holding of the SCL line low by slave device recognized as 'not available bus'
**            - Invalid start/stop condition detection provided
**          SLAVE mode
**            - 7-bit slave addressing
**            - General call address detection provided
*/         
/*!
**  @addtogroup CI2C1_module CI2C1 module documentation
**  @{
*/         

#ifndef __CI2C1
#define __CI2C1

/* MODULE CI2C1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited components */
#include "Cpu.h"

#pragma DATA_SEG CI2C1_DATA            /* Select data segment "CI2C1_DATA" */
#pragma CODE_SEG CI2C1_CODE


/* MODULE CI2C1. */

extern word CI2C1_SndRcvTemp;

byte CI2C1_SendChar(byte Chr);
/*
** ===================================================================
**     Method      :  CI2C1_SendChar (component InternalI2C)
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 0 to the I2C bus and then writes one
**         character (byte) to the bus. The slave address must be
**         specified before, by the "SelectSlave" or "SelectSlave10"
**         method or in the component initialization section, "Target
**         slave address init" property. If interrupt service is
**         enabled and the method returns ERR_OK, it doesn't mean that
**         transmission was successful. The state of transmission is
**         obtainable from (OnTransmitData, OnError or OnArbitLost)
**         events. 
**         When working as a SLAVE, this method writes a character to
**         the internal output slave buffer and, after the master
**         starts the communication, to the I2C bus. If no character is
**         ready for a transmission (internal output slave buffer is
**         empty), the Empty character will be sent (see "Empty
**         character" property).
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by an acknowledge (only in
**                           master mode and when interrupt service is
**                           disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot transmit data
**                           ERR_TXFULL - Transmitter is full (slave
**                           mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
** ===================================================================
*/

#define CI2C1_RecvChar(Chr) CI2C1_RecvBlock((Chr), 1U, &CI2C1_SndRcvTemp)
/*
** ===================================================================
**     Method      :  CI2C1_RecvChar (component InternalI2C)
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 1 to the I2C bus, then reads one
**         character (byte) from the bus and then sends the stop
**         condition. The slave address must be specified before, by
**         the "SelectSlave" or "SelectSlave10" method or in component
**         initialization section, property "Target slave address init".
**         If interrupt service is enabled and the method returns
**         ERR_OK, it doesn't mean that transmission was finished
**         successfully. The state of transmission must be tested by
**         means of events (OnReceiveData, OnError or OnArbitLost). In
**         case of successful transmission, received data is ready
**         after OnReceiveData event is called. 
**         When working as a SLAVE, this method reads a character from
**         the input slave buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Received character.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledge (only
**                           in master mode and when interrupt service
**                           is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot receive data
**                           ERR_RXEMPTY - No data in receiver (slave
**                           mode only)
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last character or block received
**                           (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
**                           ERR_NOTAVAIL - Method is not available in
**                           current mode - see the comment in the
**                           generated code
** ===================================================================
*/

byte CI2C1_SendBlock(void* Ptr,word Siz,word *Snt);
/*
** ===================================================================
**     Method      :  CI2C1_SendBlock (component InternalI2C)
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 0 to the I2C bus and then writes the
**         block of characters to the bus. The slave address must be
**         specified before, by the "SelectSlave" or "SlaveSelect10"
**         method or in component initialization section, "Target slave
**         address init" property. If interrupt service is enabled and
**         the method returns ERR_OK, it doesn't mean that transmission
**         was successful. The state of transmission is detectable by
**         means of events (OnTransmitData, OnError or OnArbitLost).
**         Data to be send is not copied to an internal buffer and
**         remains in the original location. Therefore the content of
**         the buffer should not be changed until the transmission is
**         complete. Event OnTransmitData can be used to detect the end
**         of the transmission.
**         When working as a SLAVE, this method writes a block of
**         characters to the internal output slave buffer and then,
**         after the master starts the communication, to the I2C bus.
**         If no character is ready for a transmission (internal output
**         slave buffer is empty), the "Empty character" will be sent
**         (see "Empty character" property). In SLAVE mode the data are
**         copied to an internal buffer, if specified by "Output buffer
**         size" property.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send.
**         Siz             - Size of the block.
**       * Snt             - Amount of data sent (moved to a buffer).
**                           In master mode, if interrupt support is
**                           enabled, the parameter always returns the
**                           same value as the parameter 'Siz' of this
**                           method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledge (only
**                           in master mode and when interrupt service
**                           is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot transmit data
**                           ERR_TXFULL - Transmitter is full. Some data
**                           has not been sent. (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
** ===================================================================
*/

byte CI2C1_RecvBlock(void* Ptr,word Siz,word *Rcv);
/*
** ===================================================================
**     Method      :  CI2C1_RecvBlock (component InternalI2C)
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 1 to the I2C bus, then reads the
**         block of characters from the bus and then sends the stop
**         condition. The slave address must be specified before, by
**         the "SelectSlave" or "SelectSlave10" method or in component
**         initialization section, "Target slave address init" property.
**         If interrupt service is enabled and the method returns
**         ERR_OK, it doesn't mean that transmission was finished
**         successfully. The state of transmission must be tested by
**         means of events (OnReceiveData, OnError or OnArbitLost). In
**         case of successful transmission, received data is ready
**         after OnReceiveData event is called. 
**         When working as a SLAVE, this method reads a block of
**         characters from the input slave buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - A pointer to the block space for received
**                           data.
**         Siz             - The size of the block.
**       * Rcv             - Amount of received data. In master mode,
**                           if interrupt support is enabled, the
**                           parameter always returns the same value as
**                           the parameter 'Siz' of this method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by an acknowledge (only in
**                           master mode and when interrupt service is
**                           disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot receive data
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data. Only
**                           available data (or no data) has been
**                           returned  (slave mode only).
**                           ERR_OVERRUN - Overrun error was detected
**                           from last character or block receiving
**                           (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
**                           ERR_NOTAVAIL - Method is not available in
**                           current mode - see the comment in the
**                           generated code.
** ===================================================================
*/

byte CI2C1_SelectSlave(byte Slv);
/*
** ===================================================================
**     Method      :  CI2C1_SelectSlave (component InternalI2C)
**     Description :
**         This method selects a new slave for communication by its
**         7-bit slave address value. Any send or receive method
**         directs to or from selected device, until a new slave device
**         is selected by this method. This method is not available for
**         the SLAVE mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Slv             - 7-bit slave address value.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_BUSY - The device is busy, wait until
**                           the current operation is finished.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  The device is disabled
** ===================================================================
*/

void CI2C1_Init(void);
/*
** ===================================================================
**     Method      :  CI2C1_Init (component InternalI2C)
**
**     Description :
**         Initializes the associated peripheral(s) and the components 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#pragma DATA_SEG DEFAULT               /* Select data segment "DEFAULT" */
#pragma CODE_SEG DEFAULT

/* END CI2C1. */

#endif /* ifndef __CI2C1 */
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
