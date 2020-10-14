//------------------------------------------------------------------------                                                                             
//	sensores.h
//	Pertenece al proyecto Tesla Model 3 Versión x.xx 23/07/2020 19:54:45
// Copyright (C) 2.020 Microelectrónica Maser. All rights reserved.                
//                                                                                 
//	Developped: 	Borja Gómez			Date:
// Modified:		Javier Vicandi		Date:                                
//	Tested:			Borja Gómez	  		Date:			                                         
//	Approved:		Javier Vicandi	   Date:


#ifndef SENSORES_H
#define SENSORES_H 

#include <PE_Types.h>			//Para uso de tipos enteros con notación indicando tamaño


//---------------------- End of User Change Area -----------------------------

//-------- ParameterType --------


//-----------DEFINITIONS



//Estos microcontroladores S12Z de NXP(antes Freescale, antes Motorola) utilizan un 
//orden de bytes BIG ENDIAN.
typedef struct{
	union{
		uint8_t u8RH[2];	
		uint16_t u16RH;
	}unRH;	
	uint8_t u8LE_RH[2];					//Versión Little Endian para LIN
	union{
		uint8_t u8ALS1[2];	
		uint16_t u16ALS1;
	}unALS1;
	uint8_t u8LE_ALS1[2];				//Versión Little Endian para LIN
	union{
		uint8_t u8ALS2[2];	
		uint16_t u16ALS2;
	}unALS2;
	uint8_t u8LE_ALS2[2];				//Versión Little Endian para LIN
	uint16_t u16temp;
	}stimage_process_t; 

//----------External variable declaration
extern stimage_process_t stimage_process;

//----------Prototipos
bool Config_ISLs(void);  
void Lectura_Sensor_RH(void);
void Lectura_Sensores_ALS(void);
void Reset_Sensor_RH(void); 

#endif // SENSORES_H   
// In order to avoid a repeated usage of the header file  

//************************ (C) COPYRIGHT MASERMIC *****END OF FILE****

