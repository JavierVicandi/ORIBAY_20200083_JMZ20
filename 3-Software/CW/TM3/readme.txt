//------------------------------------------------------------------------
//  Readme.txt
//------------------------------------------------------------------------
==========================================================================
                   Tesla Model 3 TM3 Project for MC9S12ZVLS16MFM
==========================================================================
                   Microelectr�nica Maser
                   Kurutz Gain, 20850 Mendaro
                    Tel.: +34 943 742669

==========================================================================
History
Date			  	Ver		   Author			CW11.1	   Compiler			Description
31/07/2020 		0.01			J.Vicandi		2018			S12Z 1.0.9		Versi�n inicial
31/07/2020 		0.02			J.Vicandi		2018			S12Z 1.0.9		
04/08/2020 		0.03			B.G�mez			2018			S12Z 1.0.9		
04/08/2020 		1.00			J.Vicandi		2018			S12Z 1.0.9		
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
- Documentation: Informaci�n generada por PE
- FLASH: C�digo reubicable y ejecutable generados por el compilador y linker
- settings: Opciones del proyecto
- Ficheros de proyecto Eclipse, PE e informaci�n


Ver. 0.01
Se configuran perif�ricos y se leen humedad y luminosidades y comunica por LIN.

Ver. 0.02
Se comprueba el CRC del dato de humedad leido del SHT31. Se a�ade el watchdog.

Ver. 0.03
Se a�aden mensajes de LIN para el segundo sensor de luminosidad (U1). En los ensayos 
que se han hecho en el Tesla no se ha podido ver peticiones LIN para este sensor pero 
puede ser debido a que se hagan s�lamente para configuraciones de volante a la derecha.

Ver. 1.00
Igual que la 0.03 para distribuci�n.

