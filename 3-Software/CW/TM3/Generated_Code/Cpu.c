/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : Cpu.c
**     Project     : TM3
**     Processor   : MC9S12ZVLS16MFM
**     Component   : MC9S12ZVLS32_32
**     Version     : Component 01.004, Driver 02.08, CPU db: 3.00.000
**     Datasheet   : MC9S12ZVLRMV1 Rev. 0.09 December 10, 2012
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-06-10, 20:19, # CodeGen: 2
**     Abstract    :
**         This component "MC9S12ZVLS32_32" implements properties, methods,
**         and events of the CPU.
**     Settings    :
**
**     Contents    :
**         EnableInt   - void Cpu_EnableInt(void);
**         DisableInt  - void Cpu_DisableInt(void);
**         SetWaitMode - void Cpu_SetWaitMode(void);
**         SetStopMode - void Cpu_SetStopMode(void);
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
** @file Cpu.c
** @version 02.08
** @brief
**         This component "MC9S12ZVLS32_32" implements properties, methods,
**         and events of the CPU.
*/         
/*!
**  @addtogroup Cpu_module Cpu module documentation
**  @{
*/         

/* MODULE Cpu. */

#include "CI2C1.h"
#include "EI2C1.h"
#include "DataPin1.h"
#include "ClockPin1.h"
#include "Events.h"
#include "Cpu.h"

#define CGM_DELAY  0x02FFU

#pragma DATA_SEG DEFAULT               /* Select data segment "DEFAULT" */
#pragma CODE_SEG DEFAULT


/* Global variables */
volatile byte CCR_reg;                 /* Current CCR reegister */
#pragma CODE_SEG __NEAR_SEG NON_BANKED


/*
** ===================================================================
**     Method      :  Cpu_Cpu_Interrupt (component MC9S12ZVLS32_32)
**
**     Description :
**         The method services unhandled interrupt vectors.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(Cpu_Interrupt)
{
  /*lint -save -e950 Disable MISRA rule (1.1) checking. */
  asm(BGND);
  /*lint -restore Enable MISRA rule (1.1) checking. */
}


#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Method      :  Cpu_DisableInt (component MC9S12ZVLS32_32)
**     Description :
**         Disable maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_DisableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_EnableInt (component MC9S12ZVLS32_32)
**     Description :
**         Enable maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_EnableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_SetStopMode (component MC9S12ZVLS32_32)
**     Description :
**         Set low power mode - Stop mode.
**         For more information about the stop mode see
**         documentation of this CPU.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_SetStopMode(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_SetWaitMode (component MC9S12ZVLS32_32)
**     Description :
**         Set low power mode - Wait mode.
**         For more information about the wait mode see
**         documentation of this CPU.
**         Release from Wait mode: Reset or interrupt
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_SetWaitMode(void)

**      This method is implemented as macro in the header module. **
*/

#pragma CODE_SEG __NEAR_SEG NON_BANKED

/*
** ===================================================================
**     Method      :  _EntryPoint (component MC9S12ZVLS32_32)
**
**     Description :
**         Initializes the whole system like timing and so on. At the end 
**         of this function, the C startup is invoked to initialize stack,
**         memory areas and so on.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
extern void _Startup(void);            /* Forward declaration of external startup function declared in file Start12.c */

/*** !!! Here you can place your own code using property "User data declarations" on the build options tab of the CPU component. !!! ***/



#pragma NO_RETURN                      /* Suppress generation of return from a function */
void _EntryPoint(void)
{

  /*** !!! Here you can place your own code using property "User code before PE initialization" on the build options tab of the CPU compoennt. !!! ***/

  /* ### MC9S12ZVLS32_32 "Cpu" init code ... */
  /*  PE initialization code after reset */
  /* IVBR: IVB_ADDR=0x7FFF,??=0 */
  setReg16(IVBR, 0xFFFEU);              
  /* ECLKCTL: NECLK=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg8(ECLKCTL, 0x80U);              
  /*  System clock initialization */
  /* CPMUPROT: ??=0,??=0,??=1,??=0,??=0,??=1,??=1,PROT=0 */
  setReg8(CPMUPROT, 0x26U);            /* Disable protection of clock configuration registers */ 
  /* CPMUCLKS: PSTP=0 */
  clrReg8Bits(CPMUCLKS, 0x40U);         
  /* CPMUCLKS: PLLSEL=1 */
  setReg8Bits(CPMUCLKS, 0x80U);        /* Enable the PLL to allow write to divider registers */ 
  /* CPMUPOSTDIV: ??=0,??=0,??=0,POSTDIV4=0,POSTDIV3=0,POSTDIV2=0,POSTDIV1=1,POSTDIV0=1 */
  setReg8(CPMUPOSTDIV, 0x03U);         /* Set the post divider register */ 
  /* Whenever changing PLL reference clock (REFCLK) frequency to a higher value
   it is recommended to write CPMUSYNR = 0x00 in order to stay within specified
   maximum frequency of the MCU */
  /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=0,SYNDIV5=0,SYNDIV4=0,SYNDIV3=0,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */
  setReg8(CPMUSYNR, 0x00U);            /* Set the multiplier register */ 
  /* CPMUPLL: ??=0,??=0,FM1=0,FM0=0,??=0,??=0,??=0,??=0 */
  setReg8(CPMUPLL, 0x00U);             /* Set the PLL frequency modulation */ 
  /* CPMUSYNR: VCOFRQ1=0,VCOFRQ0=1,SYNDIV5=0,SYNDIV4=1,SYNDIV3=1,SYNDIV2=0,SYNDIV1=0,SYNDIV0=0 */
  setReg8(CPMUSYNR, 0x58U);            /* Set the multiplier register */ 
  while(CPMUIFLG_LOCK == 0U) {         /* Wait until the PLL is within the desired tolerance of the target frequency */
  }
  /* CPMUPROT: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,PROT=0 */
  setReg8(CPMUPROT, 0x00U);            /* Enable protection of clock configuration registers */ 
  /* CPMUCOP: RSBCK=0,WRTMASK=1 */
  clrSetReg8Bits(CPMUCOP, 0x40U, 0x20U); 
  /* CPMUHTCTL: ??=0,??=0,VSEL=0,??=0,HTE=0,HTDS=0,HTIE=0,HTIF=0 */
  setReg8(CPMUHTCTL, 0x00U);            
  /* CPMUVREGCTL: ??=0,??=0,??=0,??=0,??=0,??=0,EXTXON=0,INTXON=1 */
  setReg8(CPMUVREGCTL, 0x01U);          
  /*** End of PE initialization code after reset ***/

  /*** !!! Here you can place your own code using property "User code after PE initialization" on the build options tab of the CPU component. !!! ***/

  /*lint -save  -e950 Disable MISRA rule (1.1) checking. */
  __asm(jmp _Startup);                 /* Jump to C startup code */
  /*lint -restore Enable MISRA rule (1.1) checking. */
}

#pragma CODE_SEG DEFAULT
/*
** ===================================================================
**     Method      :  PE_low_level_init (component MC9S12ZVLS32_32)
**
**     Description :
**         Initializes components and provides common register 
**         initialization. The method is called automatically as a part 
**         of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_low_level_init(void)
{
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /* Common initialization of the CPU registers */
  /* DDRT: DDRT7=1,DDRT6=1,DDRT5=1,DDRT4=1,DDRT3=1,DDRT2=1,DDRT1=0,DDRT0=0 */
  setReg8(DDRT, 0xFCU);                 
  /* MODRR0: IIC0RR=1 */
  setReg8Bits(MODRR0, 0x10U);           
  /* PIEP: PIEP5=0,PIEP3=0 */
  clrReg8Bits(PIEP, 0x28U);             
  /* PTP: PTP5=1,PTP3=1 */
  setReg8Bits(PTP, 0x28U);              
  /* PPSP: PPSP5=0,PPSP3=0 */
  clrReg8Bits(PPSP, 0x28U);             
  /* PERP: PERP5=1,PERP3=1 */
  setReg8Bits(PERP, 0x28U);             
  /* DDRP: DDRP6=1,DDRP5=0,DDRP4=1,DDRP3=0,DDRP2=1,DDRP0=1 */
  clrSetReg8Bits(DDRP, 0x28U, 0x55U);   
  /* CPMUINT: LOCKIE=0,OSCIE=0 */
  clrReg8Bits(CPMUINT, 0x12U);          
  /* CPMULVCTL: LVIE=0 */
  clrReg8Bits(CPMULVCTL, 0x02U);        
  /* ECCIE: SBEEIE=0 */
  clrReg8Bits(ECCIE, 0x01U);            
  /* ECCDCMD: ECCDRR=0 */
  clrReg8Bits(ECCDCMD, 0x80U);          
  /* DDRADH: DDRADH1=1,DDRADH0=1 */
  setReg8Bits(DDRADH, 0x03U);           
  /* DDRADL: DDRADL7=1,DDRADL6=1 */
  setReg8Bits(DDRADL, 0xC0U);           
  /* DDRJ: DDRJ1=1,DDRJ0=1 */
  setReg8Bits(DDRJ, 0x03U);             
  /* RDRP: RDRP7=0,RDRP5=0,RDRP3=0,RDRP1=0 */
  clrReg8Bits(RDRP, 0xAAU);             
  /* IRQCR: IRQEN=0 */
  clrReg8Bits(IRQCR, 0x40U);            
  /* ### MC9S12ZVLS32_32 "Cpu" init code ... */
  /* ### InternalI2C "CI2C1" init code ... */
  CI2C1_Init();
  /* ### BitIO "DataPin1" init code ... */
  /* ### BitIO "ClockPin1" init code ... */
  /* ###  "EI2C1" init code ... */
  EI2C1_Init();
  __EI();                              /* Enable interrupts */
}

/*lint -save  -e950 Disable MISRA rule (1.1) checking. */
/* Initialization of the CPU registers in FLASH */
/*lint -restore Enable MISRA rule (1.1) checking. */

/* END Cpu. */

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
