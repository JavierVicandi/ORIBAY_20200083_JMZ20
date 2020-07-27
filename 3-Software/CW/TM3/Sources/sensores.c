//------------------------------------------------------------------------                                                                             
//	sensores.c
//	Pertenece al proyecto Tesla Model 3 Versión x.xx 23/07/2020 19:54:45
// Copyright (C) 2.020 Microelectrónica Maser. All rights reserved.                
//                                                                                 
//	Developped: 	Borja Gómez			Date:
// Modified:		Javier Vicandi		Date:                                
//	Tested:			Borja Gómez	  		Date:			                                         
//	Approved:		Javier Vicandi	   Date:                                                                              
//                                                                                 

// ---------------Includes ---------------
//#include "CI2C1.h"
#include "EI2C1.h"
#include "RST_SHT.h"

#include "sensores.h"

 // --------Definitions ---------------------
//#define SHT_31_I2C_ADDRESS_U8	    	(uint8_t)(0x45U)
#define SHT_31_I2C_ADDRESS_U8	    	(uint8_t)(0x44U)	//Temporal 24/07/2020 18:47:34
#define ISL_76683_I2C_ADDRESS_U8		(uint8_t)(0x44U)
#define SHT_31_MEAS_TIME_MS			(uint16_t)(2U)
#define ISL_76683_MEAS_TIME_MS		(uint16_t)(100U)	
#define SHT_31_RST_TIME_MS				(uint16_t)(3U)	

//------------Global variables

stimage_process_t stimage_process;

//-----------------------FUNCTIONS--------------------------------------



/***********************************************************************************************
*
* @brief    Config_ISL - Configura los sensores de luminosidad ISL76683
* @param    none
* @return   none
*
************************************************************************************************/
void Config_ISLs(void)
{
	uint8_t u8cmd_ISL_commandII[2] = {0x01U, 0x02U};			//Command-II: ADC Resolution 16-bit ADC / Full Scale range3, 16,000
	uint8_t u8cmd_ISL_int_LT[3] = {0x04U, 0x00U, 0x10U};		//Lower Interrupt Threshold Registers:
	uint8_t u8cmd_ISL_int_HT[3] = {0x06U, 0xFFU, 0x11U};		//Upper Interrupt Threshold Registers:
	uint16_t u16sent;														//Número de bytes efectivamente enviados
	
	// Se configuran U3 y U1, sensores ISL76683 conectados a I2C hardware y software respectivamente.	
	// Set slave address 
	//CI2C1_SelectSlave(ISL_76683_I2C_ADDRESS_U8);					//Bus IIC hardware compartido con sensor de humedad SHT31
	// Enviar la configuración	a U3
	//CI2C1_SendBlock(u8cmd_ISL_commandII, 2, &u16sent);			
	//CI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41	
	//CI2C1_SendBlock(u8cmd_ISL_int_LT, 3, &u16sent);
	//CI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41	
	//CI2C1_SendBlock(u8cmd_ISL_int_HT, 3, &u16sent);
	//CI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41
	// Enviar la configuración	a U1
	/*
	EI2C1_SendBlock(u8cmd_ISL_commandII, 2, &u16sent);			//Bus IIC software exclusivo del ISL76683
	EI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41
	EI2C1_SendBlock(u8cmd_ISL_int_LT, 3, &u16sent);
	EI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41	
	EI2C1_SendBlock(u8cmd_ISL_int_HT, 3, &u16sent);
	EI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41
	*/
}//Fin de Config_ISLs 
volatile uint8_t u8return;

/***********************************************************************************************
*
* @brief    Lectura_Sensor_RH - Lee los valores del sensor de humedad SHT-31
* @param    none
* @return   none
*
************************************************************************************************/
void Lectura_Sensor_RH(void)
{
	uint8_t u8cmd_SHT31[2] = {0x2C, 0x06U};		      //Comando para lectura sensor RH
	uint8_t u8receivebuff[6];                          //Temperatura[2] +CRC + Humedad[2] + CRC;
	uint16_t u16sent;												//Número de bytes efectivamente enviados
	uint16_t u16rcv;												//Número de bytes efectivamente recibidos
	
    // Set slave address 
    //CI2C1_SelectSlave(SHT_31_I2C_ADDRESS_U8);			//Bus IIC hardware compartido con sensor de luminosidad ISL76683
    EI2C1_SelectSlave(SHT_31_I2C_ADDRESS_U8);			//Bus IIC hardware compartido con sensor de luminosidad ISL76683
  	//Lectura sensor de humedad
	//CI2C1_SendBlock(u8cmd_SHT31, 2, &u16sent);
	EI2C1_SendBlock(u8cmd_SHT31, 2, &u16sent);
	
	//EI2C1_SendStop();
	
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < SHT_31_MEAS_TIME_MS;
		){;}//Espera unos SHT_31_MEAS_TIME_MS para que se complete la medida
	
	//u8return = CI2C1_RecvBlock(u8receivebuff, 6, &u16rcv);
  	u8return = EI2C1_RecvBlock(u8receivebuff, 6, &u16rcv);
  	//EI2C1_SendStop();												//No se envía de forma automática 27/07/2020 14:16:41
  	stimage_process.u8RH[1] = u8receivebuff[3];
  	stimage_process.u8RH[0] = u8receivebuff[4];
  	
	
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < SHT_31_MEAS_TIME_MS;
		){;}//Espera unos SHT_31_MEAS_TIME_MS para que se complete la medida
	

}//Fin de Lectura_Sensor_RH

/***********************************************************************************************
*
* @brief    Lectura_Sensores_ALS - Lee el valor del sensor de ALS
* @param    none
* @return   none
*
************************************************************************************************/
void Lectura_Sensores_ALS(void)
{
	uint8_t u8receivebuff[2];
	uint8_t u8cmd_ALS_commandI[2] = {0x00U, 0xA3U};		    //Command-I: IC measures ALS continuously operation mode, 16 persist integration cycles
	uint8_t u8cmd_ISL_DataLSB[1] = {0x02U};		    	//Comando para lectura sensor luminosidad
	uint16_t u16sent;												//Número de bytes efectivamente enviados
	uint16_t u16rcv;												//Número de bytes efectivamente recibidos


	// Se leen U3 y U1, sensores ISL76683 conectados a I2C hardware y software respectivamente.	
	// Set slave address 
	//CI2C1_SelectSlave(ISL_76683_I2C_ADDRESS_U8);					//Bus IIC hardware compartido con sensor de humedad SHT31
	// Enviar comando para conversión a U3
	//CI2C1_SendBlock(u8cmd_ALS_commandI, 2, &u16sent);	
  	//CI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41			
	//CI2C1_SendBlock(u8cmd_ISL_DataLSB, 1, &u16sent);			//Se van a leer los registros de datos
	//CI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41
	// Enviar comando para conversión a U1
	//EI2C1_SendBlock(u8cmd_ALS_commandI, 2, &u16sent);	
	//EI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41			
	//EI2C1_SendBlock(u8cmd_ISL_DataLSB, 1, &u16sent);			//Se van a leer los registros de datos
	//EI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < ISL_76683_MEAS_TIME_MS;
		){;}//Espera unos ISL_76683_MEAS_TIME_MS para que se complete la medida
	// Leer de U3
  	//CI2C1_RecvBlock(u8receivebuff, 2, &u16rcv);
  	//CI2C1_SendStop();												//No se envía de forma automática 27/07/2020 14:16:41
  	stimage_process.u8ALS1[1] = u8receivebuff[1];
  	stimage_process.u8ALS1[0] = u8receivebuff[0];
	// Leer de U1
  	//EI2C1_RecvBlock(u8receivebuff, 2, &u16rcv);
 	//EI2C1_SendStop();														//No se envía de forma automática 27/07/2020 14:16:41
  	stimage_process.u8ALS2[1] = u8receivebuff[1];
  	stimage_process.u8ALS2[0] = u8receivebuff[0];

}//Fin de Lectura_Sensores_ALS

/***********************************************************************************************
*
* @brief    Reset_Sensor_RH - Pulso a LOW para resetear el sensor de humedad SHT-31
* @param    none
* @return   none
*
************************************************************************************************/
void Reset_Sensor_RH(void)
{
	
	RST_SHT_ClrVal();
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < SHT_31_RST_TIME_MS;
		){;}//Espera unos SHT_31_RST_TIME_MS para garantizar el reset
	RST_SHT_SetVal();	

}//Fin de Reset_Sensor_RH

//************************ (C) COPYRIGHT MASERMIC *****END OF FILE****
