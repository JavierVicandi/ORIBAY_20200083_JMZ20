/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : LINPHY0.c
**     Project     : TM3
**     Processor   : MC9S12ZVLS16MFM
**     Component   : Init_LINPHY
**     Version     : Component 01.000, Driver 01.00, CPU db: 3.00.000
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-07-23, 19:13, # CodeGen: 10
**     Abstract    :
**          This file implements the LINPHY (LINPHY0) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : LINPHY0
**          Device                                         : LINPHY0
**          Settings                                       : 
**            Slew rate                                    : Normal
**            Rx only mode                                 : Disabled
**            Wake up                                      : Disabled
**            Pull up                                      : Strong (34k)
**            TxD-dominant timeout                         : Enabled
**          Pins/Signals                                   : 
**            LIN Bus                                      : 
**              Pin                                        : LIN
**              Pin signal                                 : 
**            LIN GND                                      : 
**              Pin                                        : LGND
**              Pin signal                                 : 
**            Tx Input                                     : Enabled
**              Pin                                        : LIN_TXD_to_SCI0TXD
**              Pin signal                                 : 
**              Tx Probe Output                            : Disabled
**            Rx Output                                    : 
**              Output 0                                   : Enabled
**                Pin 0                                    : LIN_RXD_to_SCI0RXD
**                Pin 0 signal                             : 
**          Interrupts                                     : 
**            Interrupt                                    : Vlinphy0
**            Interrupt priority                           : 1
**            Over-current interrupt                       : Disabled
**            ISR Name                                     : LinPhy_ISR
**          Initialization                                 : 
**            Enable LINPHY                                : yes
**            Init state of Tx SW control                  : 1
**            Call Init method                             : yes
**     Contents    :
**         Init - void LINPHY0_Init(void);
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
** @file LINPHY0.c
** @version 01.00
** @brief
**          This file implements the LINPHY (LINPHY0) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup LINPHY0_module LINPHY0 module documentation
**  @{
*/         

/* MODULE LINPHY0. */

#include "LINPHY0.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma DATA_SEG LINPHY0_DATA          /* Select data segment "LINPHY0_DATA" */
#pragma CODE_SEG LINPHY0_CODE
#pragma CONST_SEG LINPHY0_CONST        /* Constant section for this module */
/*
** ===================================================================
**     Method      :  LINPHY0_Init (component Init_LINPHY)
**     Description :
**         This method initializes registers of the LINPHY module
**         according to the Peripheral Initialization settings.
**         Call this method in user code to initialize the module. By
**         default, the method is called by PE automatically; see "Call
**         Init method" property of the component for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LINPHY0_Init(void)
{
  /* LP0CR: ??=0,??=0,??=0,??=0,LPE=0,RXONLY=0,LPWUE=0,LPPUE=0 */
  setReg8(LP0CR, 0x00U);                
  if (LP0IF & 0x80U) {                 /* Is LPDTIF flag set? */
    /* LP0IF: LPDTIF=1,LPOCIF=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    setReg8(LP0IF, 0x80U);            
  }
  if (LP0IF & 0x40U) {                 /* Is LPOCIF flag set? */
    /* LP0IF: LPDTIF=0,LPOCIF=1,??=0,??=0,??=0,??=0,??=0,??=0 */
    setReg8(LP0IF, 0x40U);            
  }
  /* LP0DR: ??=0,??=0,??=0,??=0,??=0,??=0,LPDR1=1,LPDR0=1 */
  setReg8(LP0DR, 0x03U);                
  /* LP0SLRM: LPDTDIS=0,LPSLR=0 */
  clrReg8Bits(LP0SLRM, 0x83U);          
  /* LP0IE: LPDTIE=0,LPOCIE=0 */
  clrReg8Bits(LP0IE, 0xC0U);            
  /* LP0SLRM: LPDTDIS=0 */
  clrReg8Bits(LP0SLRM, 0x80U);          
  /* LP0CR: ??=0,??=0,??=0,??=0,LPE=1,RXONLY=0,LPWUE=0,LPPUE=1 */
  setReg8(LP0CR, 0x09U);                
}

/*
** ###################################################################
**
**  The interrupt service routine(s) must be implemented
**  by user in one of the following user modules.
**
**  If the "Generate ISR" option is enabled, Processor Expert generates
**  ISR templates in the CPU event module.
**
**  User modules:
**      main.c
**      Events.c
**
** ###################################################################
ISR(LinPhy_ISR)
{
// NOTE: The routine should include actions to clear the appropriate
//       interrupt flags.
//
}
*/


/* END LINPHY0. */
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