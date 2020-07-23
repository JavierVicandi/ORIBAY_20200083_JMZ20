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

#include <sensores.h>

 // --------Definitions ---------------------
#define SHT_31_I2C_ADDRESS_U8	    	(uint8_t)(0x45U)
#define ISL_76683_I2C_ADDRESS_U8		(uint8_t)(0x44U)
#define SHT_31_MEAS_TIME_MS			(uint16_t)(20U)	

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
	uint8_t u8cmd_ISL_commandII[2] = {0x01U, 0x02U};		    //Command-II: ADC Resolution 16-bit ADC / Full Scale range3, 16,000
	uint8_t u8cmd_ISL_int_LT[3] = {0x04U, 0x00U, 0x10U};		//Lower Interrupt Threshold Registers:
	uint8_t u8cmd_ISL_int_HT[3] = {0x06U, 0xFFU, 0x11U};		//Upper Interrupt Threshold Registers:
	uint16_t u16sent;
	
	
	// Set slave address 
	CI2C1_SelectSlave(ISL_76683_I2C_ADDRESS_U8);					//Bus IIC hardware compartido con sensor de humedad
		
	CI2C1_SendBlock(u8cmd_ISL_commandII, 2, &u16sent);
	CI2C1_SendBlock(u8cmd_ISL_int_LT, 3, &u16sent);
	CI2C1_SendBlock(u8cmd_ISL_int_HT, 3,&u16sent);

	EI2C1_SendBlock(u8cmd_ISL_commandII, 2, &u16sent);			//Bus IIC software exclusivo del ISL76683
	EI2C1_SendBlock(u8cmd_ISL_int_LT, 3, &u16sent);
	EI2C1_SendBlock(u8cmd_ISL_int_HT, 3,&u16sent);

}//Fin de Config_ISLs 

/***********************************************************************************************
*
* @brief    Lectura_Sensores_i2c - Lee los valores de los sensores: luminosidad y humedad
* @param    none
* @return   none
*
************************************************************************************************/
void Lectura_Sensor_RH(void)
{
	uint8_t u8cmd_SHT31[2] = {0x24U, 0x00U};		        //Comando para lectura sensor RH
	uint8_t u8receivebuff[6];                            //, u8CRC;
	uint32_t u32i;
	uint16_t u16sent;
	uint16_t u16rcv;
	
    // Set slave address 
    CI2C1_SelectSlave(SHT_31_I2C_ADDRESS_U8);

  	//Lectura sensor de humedad
	CI2C1_SendBlock(u8cmd_SHT31, 2, &u16sent);
	for(stimage_process.u16temp = 0;
		stimage_process.u16temp < SHT_31_MEAS_TIME_MS;
		){;}//Espera unos 20ms para que se complete la medida

  	CI2C1_RecvBlock(u8receivebuff, 6, &u16rcv);
  	stimage_process.u8RH[1] = u8receivebuff[3];
  	stimage_process.u8RH[0] = u8receivebuff[4];

}//Fin de Lectura_Sensor_RH

/***********************************************************************************************
*
* @brief    Lectura_Sensor_ALS - Lee el valor del sensor de ALS
* @param    none
* @return   none
*
************************************************************************************************/
void Lectura_Sensor_ALS(void)
{
	uint8_t u8receivebuff[2];
	uint8_t u8cmd_ALS_commandI[2] = {0x00U, 0xA3U};		    //Command-I: IC measures ALS continuously operation mode, 16 persist integration cycles
	//uint8_t u8cmd_ALS_commandI[2] = {0x00U, 0x23U};		//Command-I: IC measures ALS once operation mode, 16 persist integration cycles
	uint8_t u8cmd_ISL_DataLSB[2] = {0x02U, 0x00U};		    //Comando para lectura sensor luminosidad
	uint16_t u16i;

    /* Set slave address */
    LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C1, ISL_76683_I2C_ADDRESS_U8, false);

  	//Lectura sensor de luminosidad ALS
	LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C1, u8cmd_ALS_commandI, 2, true, OSIF_WAIT_FOREVER);
	LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C1, u8cmd_ISL_DataLSB, 1, true, OSIF_WAIT_FOREVER);
    //LPI2C_DRV_MasterSendData(INST_LPI2C1, u8cmd_ALS_commandI, 2, true);
    //LPI2C_DRV_MasterSendData(INST_LPI2C1, u8cmd_ISL_DataLSB, 1, true);
	//PutDataArrayI2C(ISL_76683_I2C_ADDRESS_U8, 2, u8cmd_ALS_commandI);       //Escribe dato en el registro
  	//PutDataArrayI2C(ISL_76683_I2C_ADDRESS_U8, 1, u8cmd_ISL_DataLSB);
  	for(u16i = 0; u16i < 5000; u16i++)
  	{
  		//WAIT1_Wait100Cycles();                               //Espera unos 100ms, 3000 ciclos de 33us
  	}
  	LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C1, u8receivebuff, 2, true, OSIF_WAIT_FOREVER);
  	//LPI2C_DRV_MasterReceiveData(INST_LPI2C1, u8receivebuff, 2, true);
  	//GetDataArrayI2C(ISL_76683_I2C_ADDRESS_U8, 2, u8receiveBuff);
//  	u8ALS[0] = u8I2C_ReceiveBuff[1];
//  	u8ALS[1] = u8I2C_ReceiveBuff[0];

  	u8ALS[1] = u8receivebuff[1];
  	u8ALS[0] = u8receivebuff[0];

}



//************************ (C) COPYRIGHT MASERMIC *****END OF FILE****
