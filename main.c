/**********************************************************************************************************************
 // FileName:      main.c
 // Dependencies:    msp.h, stdint.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V10 TI
 // Company:         Texas Instruments
 // Description:    UTILIZACION DE APUNTADORES A REGIRSTROS DE PERIFERICO GPIO.
 // Authors:         ALFREDO CHACON
 // Updated:         03/2021
 //Nota: no se están usando las estructuras definidas en los sorucefiles dados por el BSP de TI, los punteros declarados
 //      se incializan con las direcciones de memoria de los registros de los perifericos en cuestión
 ************************************************************************************************************************/

/************************************************
  HEADER FILES
 ************************************************/
#include<stdint.h>
#include "msp.h"
/************************************************
  DEFINICIONES DE CONSTANTES Y/O MASCARAS
 ************************************************/
#define RETARDO (100000)
#define RETARDO2 (10000)
#define PIN0    (0x01)  //definimos el numero Hexadecimal segun el Pin del PUERTO
#define PIN1    (0x02)
#define PIN2    (0x04)
#define PIN4    (0x10)
#define FALSE   0
#define TRUE    1
#define GIT     21052021
#define GIT2     1234
/************************************************
DECLARACION DE APUNTADORES PARA ACCESO A MEMORIA
 ************************************************/
    uint8_t *p1_in  = (uint8_t*)0x040004c00;
    uint8_t *p1_out = (uint8_t*)0x040004c02;
    uint8_t *p1_dir = (uint8_t*)0x040004c04;
    uint8_t *p1_ren = (uint8_t*)0x040004c06;

    uint8_t *p2_in  = (uint8_t*)0x040004c01;
    uint8_t *p2_out = (uint8_t*)0x040004c03;
    uint8_t *p2_dir = (uint8_t*)0x040004c05;

/************************************************
    PROTOTIPO DE FUNCION
************************************************/
extern uint8_t Gpio_Pin_in( uint_fast16_t pin_);

/*****************************************************************************
 * Function: MAIN
 * Preconditions: NINGUNA.
 * Overview:
 * Input: NINGUNA.
 * Output: NINGUNA
 *
 *****************************************************************************/

void main(void)
{

    /************************************************
       DECLARACION DE VARIABLES
    ************************************************/
    uint32_t i;
    uint8_t bandera = 0;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// DETIENE EL TIMER DEL WATCHDOG
	/************************************************
	    DEREFERENCIA DE APUNTADORES
	************************************************/
	*p1_dir &= ~0x10;                               //PIN P1.4 COMO ENTRADA
	*p1_ren |= 0x10;                                //HABILITAMOS RESISTENCIA
	*p1_out |= 0x10;                                //PULL UP

	*p2_dir |= 0x07;                                //PIN P2.0, P2.1, P2.2  COMO SALIDA, LEDS RGB
	*p2_out &= ~0x07;                               // APAGADOS
	while(1){
	    if ( Gpio_Pin_in(0x10) != 1)                // ENTRA AL PRESIONAR EL SWITCH DEL PUERTO P1.4 (CUANDO ESTA EN BAJO)
	    {
	        if(bandera == FALSE)
	            {
	                bandera = TRUE;                                 //BANDERA EN ALTO
	                while (bandera==TRUE){
                        *p2_out &= ~0x07;                           //APAGA BLUE
                        *p2_out |= 0x01;                            //ENCIENDE RED
                        for(i=RETARDO; i>0 ;i--);
                        *p2_out &= ~0x01;                           //APAGA RED
                        *p2_out |= 0x02;                            //ENCIENDE GREEN
                        for(i=RETARDO; i>0 ;i--);
                        *p2_out &= ~0x02;                           //APAGA GREEN
                        *p2_out |= 0x04;                            //ENCIENDE BLUE
                        for(i=RETARDO; i>0 ;i--);
                     if ( Gpio_Pin_in(0x10) != 1) bandera=FALSE;    // ENTRA CUANDO PRESIONAMOS BOTON DE P1.4
                     while( Gpio_Pin_in(0x10) != 1);                //MIENTRAS ESTE PRESIONADO EL BOTON
	                }

	            }
	        *p2_out  &= ~0x07;                         //APAGA TODOS
           // while( Gpio_Pin_in(0x10) != 1);
	    }
	    for(i=RETARDO2; i>0 ;i--);
	}
}

/*****************************************************************************
 * Function: Gpio_Pin_in
 * Preconditions: PUERTO1 SELECCIONADO.
 * Overview: LEE EL PIN DESEADO
 * Input: pin_.
 * Output: 0/1
 *
 *****************************************************************************/
uint8_t Gpio_Pin_in( uint_fast16_t pin_)
{
    uint_fast16_t inputPinValue;
    inputPinValue = *p1_in & (pin_);        // LEE EL REGISTRO QUE INDICA EL VALOR DE ENTRADA DE UN PIN
    if (inputPinValue > 0)                  // RETORNA EL VALOR 1 o 0
        return (0x01);
    return (0x00);
}
