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
#include "EI2C2.h"
#include "EI2C1.h"
#include "RST_SHT.h"

#include "sensores.h"

 // --------Definitions ---------------------
//IIO driver for the light sensor ISL76683.
//
// Copyright (c) 2017 Christoph Fritz <chf.fritz@googlemail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// Datasheet:
//  http://www.intersil.com/content/dam/Intersil/documents/isl7/isl76683.pdf

#define ISL76683_REG_CMD			0x00
#define ISL76683_REG_CTRL			0x01
#define ISL76683_REG_THR_HI		0x02
#define ISL76683_REG_THR_LO		0x03
#define ISL76683_REG_SENSOR_LSB	0x04
#define ISL76683_REG_SENSOR_MSB	0x05
#define ISL76683_REG_CLEAR_INT	0x40
#define ISL76683_REGMAP_MAX		0x40

#define ISL76683_CMD_ENABLE		0x80
#define ISL76683_CMD_PWRDWN		0x40
#define ISL76683_WIDTH_MASK		0x03
#define ISL76683_PHOTOD_SHFT		2
#define ISL76683_PHOTOD_MASK		(0x03 << ISL76683_PHOTOD_SHFT)
#define ISL76683_INTPERS_MASK		0x03

#define ISL76683_LUXRANGE_SHFT	2
#define ISL76683_LUXRANGE_MASK	(0x03 << ISL76683_LUXRANGE_SHFT)
#define ISL76683_LUXRANGE_STR		"1000 4000 16000 64000"

enum isl76683_dmode {
	ISL76683_DIODE_0 = 0,
	ISL76683_DIODE_IR,
	ISL76683_DIODE_DIFF,
};

enum isl76683_lux_range {
	ISL76683_LUX_1000 = 0,
	ISL76683_LUX_4000,
	ISL76683_LUX_16000,
	ISL76683_LUX_64000,
};
//Default values
#define ISL76683_LUX_RANGE_DEFAULT		ISL76683_LUX_16000	//31/07/2020 16:53:26 Como en Tesla S
#define ISL76683_DIODE_MAX					ISL76683_DIODE_DIFF
#define ISL76683_DIODE_DEFAULT			ISL76683_DIODE_0
#define ISL76683_WIDTH_DEFAULT			0x00						//16 bits. Número de clocks 2^16 = 65536
#define ISL76683_LUX_DEFAULT				ISL76683_LUX_16000	 
#define ISL76683_RESOLUTION_DEFAULT		16
#define ISL76683_EXT_RESISTOR_DEFAULT	100
#define ISL76683_KOHM_MIN					1
#define ISL76683_KOHM_MAX					1000
//Aunque no usamos interrupción hardware y no vamos a usar los umbrales fijamos el mismo valor. 
//Para el ISL76683 Int_Persist (B1 - B0) en Control (0x01). Interrupción después de 16 ciclos de integración. 
#define ISL76683_INTPERS_DEFAULT			0x03						//31/07/2020 16:56:37 Como en Tesla S, ISL29023
//INTERRUPT PERSIST BITS (B1 - B0)
//Umbrales como tras el reset. No se van a usar por lo que no los configuramos.31/07/2020 17:14:29
#define ISL76683_HI_THR_DEFAULT			0xFF
#define ISL76683_LO_THR_DEFAULT			0x00

#define SHT_31_I2C_ADDRESS_U8	    	(uint8_t)(0x45U)
//#define SHT_31_I2C_ADDRESS_U8	    	(uint8_t)(0x44U)	//Temporal 24/07/2020 18:47:34
#define ISL_76683_I2C_ADDRESS_U8		(uint8_t)(0x44U)
#define SHT_31_MEAS_TIME_MS			(uint16_t)(2U)
#define ISL_76683_MEAS_TIME_MS		(uint16_t)(100U)	
#define SHT_31_RST_TIME_MS				(uint16_t)(2U)	
//CRC
#define POLYNOMIAL 						0x131U      		//P(x)=x^8+x^5+x^4+1 = 100110001 
#define CHECKSUM_ERROR 					0x04U
#define CHECKSUM_OK 						0x00U

//------------Global variables

stimage_process_t stimage_process;


//-----------------PROTOTYPES
static uint8_t SF04_CheckCrc (uint8_t *u8data, uint8_t u8nbr_of_bytes, uint8_t u8checksum);

//-----------------------FUNCTIONS--------------------------------------

/***********************************************************************************************
*
* @brief    Config_ISL - Configura los sensores de luminosidad ISL76683
* @param    none
* @return   bool
*
************************************************************************************************/
bool Config_ISLs(void)
{
//Command Register 00(hex)
//ENABLE		ADCPD		TM		X		PSM		WIDTH
//1			0			0		0		00			00
//ISL76683_REG_CMD Deshabilita las conversiones, modo normal, timer interno, diodo1, luz visible, 2^16 ciclos.
	uint8_t u8cmd_ISL_command[2] = {ISL76683_REG_CMD, ((ISL76683_DIODE_DEFAULT << ISL76683_PHOTOD_SHFT) |
				ISL76683_WIDTH_DEFAULT)};			
//Command Register 01(hex)
//X		X		INT		X		GAIN		PERSIST
//0		0		0			0		11			00
//Máximo rango (15568 lux con Rext=100k), persistencia al máximo aunque es irrelevante al no usar interrupciones.
	uint8_t u8cmd_ISL_control[2] = {ISL76683_REG_CTRL,((ISL76683_LUX_RANGE_DEFAULT << ISL76683_LUXRANGE_SHFT) |
				ISL76683_INTPERS_DEFAULT)};			
	uint16_t u16sent;														//Número de bytes efectivamente enviados
	uint16_t u16rcv;														//Número de bytes efectivamente recibidos
	uint8_t u8receivebuff[2];                          		//Para comprobación
	bool b8return = TRUE;
	
	// Se configuran U3 y U1, sensores ISL76683 conectados a I2C hardware y software respectivamente.	
	// Set slave address 
	EI2C2_SelectSlave(ISL_76683_I2C_ADDRESS_U8);					//Bus IIC hardware compartido con sensor de humedad SHT31
	// Enviar la configuración	a U3 y U1. Primero al control
	EI2C2_SendBlock(u8cmd_ISL_control, 2, &u16sent);			
	EI2C1_SendBlock(u8cmd_ISL_control, 2, &u16sent);			//Bus IIC software exclusivo del ISL76683
	// A continuación el comando
	EI2C2_SendBlock(u8cmd_ISL_command, 2, &u16sent);			
	EI2C1_SendBlock(u8cmd_ISL_command, 2, &u16sent);			
	//Leemos para comprobación y a continuación habilitamos el ADC
	//U3
	EI2C2_SendBlock(u8cmd_ISL_command, 1, &u16sent);
  	EI2C2_RecvBlock(&u8receivebuff[0], 1, &u16rcv);
  	if (u8receivebuff[0] != u8cmd_ISL_command[1]){
  		b8return = FALSE;	
  	}
	EI2C2_SendBlock(u8cmd_ISL_control, 1, &u16sent);
  	EI2C2_RecvBlock(&u8receivebuff[1], 1, &u16rcv);
  	if (u8receivebuff[1] != u8cmd_ISL_control[1]){
  		b8return = FALSE;	
  	}
 
  	//U1
	EI2C1_SendBlock(u8cmd_ISL_command, 1, &u16sent);
  	EI2C1_RecvBlock(&u8receivebuff[0], 1, &u16rcv);
  	if (u8receivebuff[0] != u8cmd_ISL_command[1]){
  		b8return = FALSE;	
  	}
	EI2C1_SendBlock(u8cmd_ISL_control, 1, &u16sent);
  	EI2C1_RecvBlock(&u8receivebuff[1], 1, &u16rcv);
  	if (u8receivebuff[1] != u8cmd_ISL_control[1]){
  		b8return = FALSE;	
  	}

	u8cmd_ISL_command[1] |= ISL76683_CMD_ENABLE;
	// Habilitamos ADC a U3 y U1. 
	EI2C2_SendBlock(u8cmd_ISL_command, 2, &u16sent);			
	EI2C1_SendBlock(u8cmd_ISL_command, 2, &u16sent);			
	//No volvemos a comprobar la escritura
	return(b8return);
	
}//Fin de Config_ISLs 


//============================================================ 
//calculates checksum for n bytes of data  
//and compares it with expected checksum 
//input:    uint8_t *        checksum is built based on this data 
//          uint8_t    			checksum is built for n bytes of data 
//          uint8_t         	expected checksum          
//return:   uint8_t:        	CHECKSUM_ERROR = checksum does not match 
//                        		CHECKSUM_OK = checksum matches 
//============================================================ 
static uint8_t SF04_CheckCrc (uint8_t *u8data, uint8_t u8nbr_of_bytes, uint8_t u8checksum)
{ 
	uint8_t u8crc = 0xFF;
	uint8_t u8byte_ctr;
	uint8_t u8bit;
	uint8_t u8return = CHECKSUM_OK;
	
	//calculates 8-Bit checksum with given polynomial 
	for(u8byte_ctr = 0;
		u8byte_ctr < u8nbr_of_bytes;
		++u8byte_ctr){
		u8crc ^= (u8data[u8byte_ctr]);   
		for (u8bit = 8; u8bit > 0; --u8bit){
			if (u8crc & 0x80U){
				u8crc = (u8crc << 1) ^ POLYNOMIAL;
				}//(u8crc & 0x80)   
			else{
				u8crc = (u8crc << 1);
				}//NO (u8crc & 0x80)
			}//(u8bit = 8; u8bit > 0; --u8bit)
		}//(u8byte_ctr = 0;.. 
	if (u8crc != u8checksum){
		u8return = CHECKSUM_ERROR;
		}//(u8crc != u8checksum) 

	return(u8return);
		 
}//Fin de SF04_CheckCrc 


/***********************************************************************************************
*
* @brief    Lectura_Sensor_RH - Lee los valores del sensor de humedad SHT-31
* @param    none
* @return   none
*
************************************************************************************************/
void Lectura_Sensor_RH(void)
{
	uint8_t u8cmd_SHT31[2] = {0x2CU, 0x06U};		      //Comando para lectura sensor RH, clock stretching y max repet.
	uint8_t u8receivebuff[6];                          //Temperatura[2] +CRC + Humedad[2] + CRC;
	uint16_t u16sent;												//Número de bytes efectivamente enviados
	uint16_t u16rcv;												//Número de bytes efectivamente recibidos
	
	// Set slave address 
   EI2C2_SelectSlave(SHT_31_I2C_ADDRESS_U8);			//Bus IIC hardware compartido con sensor de luminosidad ISL76683
  	//Lectura sensor de humedad
	EI2C2_SendBlock(u8cmd_SHT31, 2, &u16sent);
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < SHT_31_MEAS_TIME_MS;){
		;
		}//Espera unos SHT_31_MEAS_TIME_MS antes de la lectura (>1ms)

  	EI2C2_RecvBlock(u8receivebuff, 6, &u16rcv);
  	
  	if (SF04_CheckCrc (&u8receivebuff[3], 2, u8receivebuff[5]) == CHECKSUM_OK){
	  	stimage_process.unRH.u8RH[0] = u8receivebuff[3];		//Mantenemos en big endian
  		stimage_process.unRH.u8RH[1] = u8receivebuff[4];
  		stimage_process.u8LE_RH[0] = u8receivebuff[4];			//Pasamos a little endian
  		stimage_process.u8LE_RH[1] = u8receivebuff[3];
  		}
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < SHT_31_MEAS_TIME_MS;){
		;
		}//Espera unos SHT_31_MEAS_TIME_MS antes de otro comando (>1ms)

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
	uint8_t u8start_measurement[1] = {ISL76683_REG_CLEAR_INT};
	uint8_t u8cmd_ISL_DataLSB[1] = {ISL76683_REG_SENSOR_LSB};	//Comando para lectura sensor luminosidad byte bajo
	uint8_t u8cmd_ISL_DataMSB[1] = {ISL76683_REG_SENSOR_MSB};	//Comando para lectura sensor luminosidad byte alto
	uint16_t u16sent;												//Número de bytes efectivamente enviados
	uint16_t u16rcv;												//Número de bytes efectivamente recibidos

	// Se leen U3 y U1, sensores ISL76683 conectados a I2C hardware y software respectivamente.	
	// Set slave address 
	EI2C2_SelectSlave(ISL_76683_I2C_ADDRESS_U8);					//Bus IIC hardware compartido con sensor de humedad SHT31
	// U3
	EI2C2_SendBlock(u8start_measurement, 1, &u16sent);			//dummy read is clearing irq and triggers new measurement
	//  U1
	EI2C1_SendBlock(u8start_measurement, 1, &u16sent);			
	// Leer de U3
  	EI2C2_RecvBlock(&u8receivebuff[0], 1, &u16rcv);
	// Leer de U1
  	EI2C1_RecvBlock(&u8receivebuff[1], 1, &u16rcv);
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < ISL_76683_MEAS_TIME_MS;){
		;
		}//Espera unos ISL_76683_MEAS_TIME_MS para que se complete la medida

	// Se leen el dato LSB de U3 y U1, sensores ISL76683 conectados a I2C hardware y software respectivamente.	
	// U3
	EI2C2_SendBlock(u8cmd_ISL_DataLSB, 1, &u16sent);			//Se van a leer los registros de datos
	// U1
	EI2C1_SendBlock(u8cmd_ISL_DataLSB, 1, &u16sent);			//Se van a leer los registros de datos
	// Leer de U3
  	EI2C2_RecvBlock(u8receivebuff, 1, &u16rcv);
  	stimage_process.u8LE_ALS1[0] = u8receivebuff[0];		//Mantenemos en little endian
  	stimage_process.unALS1.u8ALS1[1] = u8receivebuff[0];	//Pasamos de litle endian a big endia
	// Leer de U1
  	EI2C1_RecvBlock(u8receivebuff, 1, &u16rcv);
  	stimage_process.u8LE_ALS2[0] = u8receivebuff[0];		//Mantenemos en little endian
	stimage_process.unALS2.u8ALS2[1] = u8receivebuff[0];	//Pasamos de litle endian a big endian
 	// Se leen el dato MSB de U3 y U1, sensores ISL76683 conectados a I2C hardware y software respectivamente.	
	// U3
	EI2C2_SendBlock(u8cmd_ISL_DataMSB, 1, &u16sent);			//Se van a leer los registros de datos
	// U1
	EI2C1_SendBlock(u8cmd_ISL_DataMSB, 1, &u16sent);			//Se van a leer los registros de datos
	// Leer de U3
  	EI2C2_RecvBlock(u8receivebuff, 1, &u16rcv);
  	stimage_process.u8LE_ALS1[1] = u8receivebuff[0];		//Mantenemos en little endian
  	stimage_process.unALS1.u8ALS1[0] = u8receivebuff[0];	//Pasamos de litle endian a big endia
	// Leer de U1
  	EI2C1_RecvBlock(u8receivebuff, 1, &u16rcv);
  	stimage_process.u8LE_ALS2[1] = u8receivebuff[0];		//Mantenemos en little endian
	stimage_process.unALS2.u8ALS2[0] = u8receivebuff[0];	//Pasamos de litle endian a big endian

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
		stimage_process.u16temp < SHT_31_RST_TIME_MS;){
		;
		}//Espera unos SHT_31_RST_TIME_MS para garantizar el reset
	RST_SHT_SetVal();	

}//Fin de Reset_Sensor_RH

//************************ (C) COPYRIGHT MASERMIC *****END OF FILE****
