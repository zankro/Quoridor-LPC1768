/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../game/game.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	
	/* Match register 0 interrupt service routine */
	if (LPC_TIM0->IR & 01) {
		
		if (time > 0) {
			time--;
			if(time == 9) GUI_Text(110, 255, (uint8_t *) "   ", White, Black);
			TimeCount();
		}
		else {
			SaveMove(activePlayer->id, 0, 1, activePlayer->position[0], activePlayer->position[1]);
			if (turnMode == 0) {
				ClearMoves(activePlayer);
			}
			if (turnMode == 1) {
				if(!wallGrid[tempWall[0]][tempWall[1]].isWall || (wallGrid[tempWall[0]][tempWall[1]].isWall && wallGrid[tempWall[0]][tempWall[1]].direction != tempWall[2])) {
					ClearWall(tempWall[0], tempWall[1], tempWall[2]);
				}
					
			}
			if (turnMode == 1) ChangeMode();
			changePlayer();
			DrawMoves(activePlayer);
		}

		
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
	}
	
	
		/* Match register 1 interrupt service routine */
	  /* it should be possible to access to both interrupt requests in the same procedure*/
	if(LPC_TIM0->IR & 02)
  {

		LPC_TIM0->IR =  2 ;			/* clear interrupt flag */	
	}
	
	
	/* Match register 2 interrupt service routine */
  /* it should be possible to access to both interrupt requests in the same procedure*/
	if(LPC_TIM0->IR & 4)
  {

		/*tmp_leds ^= 1;
		

		LPC_TIM0->MR2 += 0x05f5e10;
	  if(LPC_TIM0->MR2 > 0x017D7840)
		{  
			 LPC_TIM0->MR2 = 0x05f5e10;
		}	*/
		
		LPC_TIM0->IR =  4 ;			/* clear interrupt flag */	
	}
	
	
		/* Match register 3 interrupt service routine */
  	/* it should be possible to access to both interrupt requests in the same procedure*/
	if(LPC_TIM0->IR & 8)
  {
	 
		LPC_TIM0->IR =  8 ;			/* clear interrupt flag */	
	}
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}





/******************************************************************************
**                            End Of File
******************************************************************************/
