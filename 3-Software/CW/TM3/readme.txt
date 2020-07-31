//------------------------------------------------------------------------
//  Readme.txt
//------------------------------------------------------------------------
==========================================================================
                   Tesla Model 3 TM3 Project for MC9S12ZVLS16MFM
==========================================================================
                   Microelectrónica Maser
                   Kurutz Gain, 20850 Mendaro
                    Tel.: +34 943 742669

==========================================================================
History
Date			  	Ver		   Author			CW11.1	   Compiler			Description
31/07/2020 		0.01			J.Vicandi		2018			S12Z 1.0.9		Versión inicial
31/07/2020 		0.02			J.Vicandi		2018			S12Z 1.0.9		

//------------------------------------------------------------------------
// Project structure
//------------------------------------------------------------------------
The project generated contains various files/folders:

- readme.txt: this file
- Sources: folder with the application source code
- Sources/Generated_Code: *.c y *.h generados para hablar por I2C, GPIOs, etc.
- lin_cfg: ficheros de base de datos de LIN
- LIN_Stack: ficheros para comunicaciones LIN por UART
- Project_Settings: Incluye startup_code, comandos para el linker y
	para el debugger *.launch 
- Project_Headers: Ficheros de cabecera
- Documentation: Información generada por PE
- FLASH: Código reubicable y ejecutable generados por el compilador y linker
- settings: Opciones del proyecto
- Ficheros de proyecto Eclipse, PE e información


Ver. 0.01
Se configuran periféricos y se leen humedad y luminosidades y comunica por LIN.

Ver. 0.02
Se comprueba el CRC del dato de humedad leido del SHT31. Se añade el watchdog.
