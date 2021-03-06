/* ###################################################################
**     Filename    : main.c
**     Project     : TM3
**     Processor   : MC9S12ZVLS16MFM
**     Version     : Driver 01.14
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2020-06-10, 20:01, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.14
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "EI2C1.h"
#include "DataPin1.h"
#include "ClockPin1.h"
#include "EI2C2.h"
#include "DataPin2.h"
#include "ClockPin2.h"
#include "Vsci0.h"
#include "Vtim0ch2.h"
#include "TEST_IN_4.h"
#include "TEST_IN_0_3.h"
#include "TEST_OUT.h"
#include "LINPHY0.h"
#include "TI1.h"
#include "RST_SHT.h"
#include "WDog1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "sensores.h"
#include "lin_common_api.h"

void main(void)
{
  /* Write your local variable definition here */
	uint8_t u8wd = 0x55U;
	
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
	(void)Config_ISLs();	
	Reset_Sensor_RH();
	l_sys_init();
	l_ifc_init(LI0);

	for(;;){
		if (u8wd == 0x55U){
			WDog1_Clear();
			u8wd = 0xAAU;
		}
		
		Lectura_Sensor_RH();	
		Lectura_Sensores_ALS();

		if (l_flg_tst_LI0_Humidity_f_flag())
		{
			l_flg_clr_LI0_Humidity_f_flag();               // Clear flag
			l_bytes_wr_LI0_Humidity(0, 2, stimage_process.u8LE_RH);           // escribimos dato: se�al humedad
		} // (l_flg_tst_LI0_Humidity_f_flag())

		if (l_flg_tst_LI0_LuminosityA_f_flag())
		{
			l_flg_clr_LI0_LuminosityA_f_flag();          // Clear flag
			l_bytes_wr_LI0_LuminosityA(0, 2, stimage_process.u8LE_ALS1);     // escribimos dato: se�al luminosidadA
		} // (l_flg_tst_LI0_LuminosityA_f_flag())
		
		
		
		if (l_flg_tst_LI0_LuminosityB_f_flag())
		{
			l_flg_clr_LI0_LuminosityB_f_flag();          // Clear flag
			l_bytes_wr_LI0_LuminosityB(0, 2, stimage_process.u8LE_ALS1);     // escribimos dato: se�al luminosidadB
		} // (l_flg_tst_LI0_LuminosityB_f_flag())
		
		if (l_flg_tst_LI0_LuminosityA2_f_flag())
		{
			l_flg_clr_LI0_LuminosityA2_f_flag();          // Clear flag
			l_bytes_wr_LI0_LuminosityA2(0, 2, stimage_process.u8LE_ALS2);     // escribimos dato: se�al luminosidadA
		} // (l_flg_tst_LI0_LuminosityA_f_flag())
		
		
		
		if (l_flg_tst_LI0_LuminosityB2_f_flag())
		{
			l_flg_clr_LI0_LuminosityB2_f_flag();          // Clear flag
			l_bytes_wr_LI0_LuminosityB2(0, 2, stimage_process.u8LE_ALS2);     // escribimos dato: se�al luminosidadB
		} // (l_flg_tst_LI0_LuminosityB_f_flag())

		if (u8wd == 0xAAU){
			WDog1_Clear();
			u8wd = 0x55U;
		}		

	}//(;;)


  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
