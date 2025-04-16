/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "game/game.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	
  
	LCD_Clear(Black);
	GUI_Text(0, 140, (uint8_t *) "   Press INT0 to start game", White, Black);
	
	/* Usare libreria nuova dei timer per le inizializzazioni dei timer. */
	
	/* USARE QUESTA COSTANTE DI TEMPO PER ESEGUIRE IL PROGETTO SU SCHEDA FISICA, OPPURE SU EMULATORE ATTIVANDO LO SCALING DEL TIMER 0*/
	init_timer(0, 0, 0, 3, 0x17D7840); 			/* 1s * 25Mhz = 25*10^6 = 0x17D7840 */
	
	/* used to speed up time in simulation on my system. With normal time const 1 second of simulation = 65 sec real life so 1/65 * 25MHz = 0x5DE67*/
	//init_timer(0, 0, 0, 3, 0x5DE67);					
																							
	
	BUTTON_init();												/* BUTTON Initialization              */
	
	/* USARE QUESTA COSTANTE DI TEMPO PER ESEGUIRE IL PROGETTO SU SCHEDA FISICA, OPPURE SU EMULATORE ATTIVANDO LO SCALING DEL RIT*/
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */
	
	/* used to speed up time in simulation on my system. */
	//init_RIT(0x527AE);									
	enable_RIT();												/* enable RIT to count 50ms				 */
	
	NVIC_DisableIRQ(EINT1_IRQn);			// Disabilito KEY1 e KEY2 nel main, perche' altrimenti attiverebbe le funzioni anche se non sono ancora dentro il gioco.
	NVIC_DisableIRQ(EINT2_IRQn);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
