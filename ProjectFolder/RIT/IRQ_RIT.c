/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down_0=0;
volatile int down_1=0;
volatile int down_2=0;

int tempWall[3];

void RIT_IRQHandler (void)
{					
	static int J_select=0;
	static int J_down = 0;
	static int J_up = 0;
	static int J_right = 0;
	static int J_left = 0;
	static int position=0;	
		
	if(!win) {
		if (turnMode == 0) {
			if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
				/* Joytick J_Select pressed p1.25*/
				/* Joytick J_Down pressed p1.26 --> using J_DOWN due to emulator issues*/
				
				J_down++;
				switch(J_down){
					case 1:
						
						activePlayer->nextMove=1;		
						break;
					
					default:
						break;
				}
			}
			else{
				
				J_down = 0;
			}
				
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
				/* Joytick J_Up pressed p1.29 */
				
				J_up++;
				switch(J_up){
					case 1:
					
						activePlayer->nextMove=0;			
						break;
					
					default:
						break;
				}
			}
			else{
				
					J_up=0;
			}
		
			if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
				/* Joytick J_Right pressed p1.28 */
				
				J_right++;
				switch(J_right){
					case 1:
					
						activePlayer->nextMove=3;		
						break;
					
					default:
						break;
				}
			}
			else{
				
					J_right=0;
			}
		
			if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
				/* Joytick J_Up pressed p1.27 */
				
				J_left++;
				switch(J_left){
					case 1:
					
						activePlayer->nextMove=2;		
						break;
					
					default:
						break;
				}
			}
			else{
				
					J_left=0;
			}
		
			if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
				/* Joytick J_Select pressed p1.25 */
				
				J_select++;
				switch(J_select){
					case 1:
						
						ClearMessage();
					
						switch(activePlayer->nextMove) {
							case 0: 
								/*Confirm Up Move */
								if(ValidMove(activePlayer->nextMove)) {
									ClearMoves(activePlayer);																						
									ClearCell(activePlayer->position[0],activePlayer->position[1]);		
									activePlayer->position[0]--;
									if(facingPawns && activePlayer->jump == activePlayer->nextMove) {
										activePlayer->position[0]--;
										facingPawns = 0;
									}
									DrawPawn2(activePlayer);
									SaveMove(activePlayer->id, 0, 0, activePlayer->position[0], activePlayer->position[1]);
									if(CheckWin()) break;
									changePlayer(); 
									CheckFacing();								
									DrawMoves(activePlayer);
									ResetTime();
								} 
								else {
									GUI_Text(15, 285, (uint8_t *) "    Can't move Up", White, Black);
								}
								break;
							
							case 1:
								/*Confirm Down Move */
								if(ValidMove(activePlayer->nextMove)) {
									ClearMoves(activePlayer);																						
									ClearCell(activePlayer->position[0],activePlayer->position[1]);	
									activePlayer->position[0]++;
									if(facingPawns && activePlayer->jump == activePlayer->nextMove) {
										activePlayer->position[0]++;
										facingPawns = 0;
									}
									DrawPawn2(activePlayer);
									SaveMove(activePlayer->id, 0, 0, activePlayer->position[0], activePlayer->position[1]);
									if(CheckWin()) break;
									changePlayer();
									CheckFacing();
									DrawMoves(activePlayer);
									ResetTime();
								}
								else {
									GUI_Text(15, 285, (uint8_t *) "   Can't move Down", White, Black);
								}
								break;
							
							case 2:
								/*Confirm Left Move */
								if(ValidMove(activePlayer->nextMove)) {
									ClearMoves(activePlayer);																						
									ClearCell(activePlayer->position[0],activePlayer->position[1]);	
									activePlayer->position[1]--;
									if(facingPawns && activePlayer->jump == activePlayer->nextMove) {
										activePlayer->position[1]--;
										facingPawns = 0;
									}
									DrawPawn2(activePlayer);	
									SaveMove(activePlayer->id, 0, 0, activePlayer->position[0], activePlayer->position[1]);
									if(CheckWin()) break;
									changePlayer();
									CheckFacing();
									DrawMoves(activePlayer);
									ResetTime();
								}
								else {
									GUI_Text(15, 285, (uint8_t *) "   Can't move Left", White, Black);
								}
								break;
							
							case 3:
								/*Confirm Right Move*/
								if(ValidMove(activePlayer->nextMove)) {
									ClearMoves(activePlayer);																						
									ClearCell(activePlayer->position[0],activePlayer->position[1]);	
									activePlayer->position[1]++;
									if(facingPawns && activePlayer->jump == activePlayer->nextMove) {
										activePlayer->position[1]++;
										facingPawns = 0;
									}
									DrawPawn2(activePlayer);
									SaveMove(activePlayer->id, 0, 0, activePlayer->position[0], activePlayer->position[1]);
									if(CheckWin()) break;
									changePlayer();
									CheckFacing();
									DrawMoves(activePlayer);
									ResetTime();
								}
								else {
									GUI_Text(15, 285, (uint8_t *) "   Can't move Right", White, Black);
								}
								break;
							
							default:
								break;
						}	

						CheckFacing();
						break;
						
					default:
						break;
				}
			}
			else{
			
			
				
					J_select=0;
			}
		
		}
		else {
			if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
				/* Joytick J_Select pressed p1.25*/
				/* Joytick J_Down pressed p1.26 --> using J_DOWN due to emulator issues*/
				
				J_down++;
				switch(J_down){
					case 1:
						
					//va evitato che muri vicini vengano cancellati..
					
						if (tempWall[0] < 5) {
							if (!wallGrid[tempWall[0]][tempWall[1]].isWall || (wallGrid[tempWall[0]][tempWall[1]].isWall && wallGrid[tempWall[0]][tempWall[1]].direction != tempWall[2])) {
								ClearWall(tempWall[0], tempWall[1], tempWall[2]);
							
								if (wallGrid[tempWall[0]][tempWall[1]].isWall) {
									DrawWall(tempWall[0], tempWall[1], wallGrid[tempWall[0]][tempWall[1]].direction); // Ridisegna il muro eventualmente cancellato in parte da tempWall 
								}
								
								if(wallGrid[tempWall[0]+1][tempWall[1]].isWall && tempWall[0]+1 <= 5) {
									DrawWall(tempWall[0]+1, tempWall[1], wallGrid[tempWall[0]+1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]-1][tempWall[1]].isWall && tempWall[0]-1 >= 0) {
									DrawWall(tempWall[0]-1, tempWall[1], wallGrid[tempWall[0]-1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]+1].isWall && tempWall[1]+1 <= 5) {
									DrawWall(tempWall[0], tempWall[1]+1, wallGrid[tempWall[0]][tempWall[1]+1].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]-1].isWall && tempWall[1]-1 >= 0) {
									DrawWall(tempWall[0], tempWall[1]-1, wallGrid[tempWall[0]][tempWall[1]-1].direction);
								}
								
							}
							tempWall[0]++;
							DrawWall(tempWall[0], tempWall[1], tempWall[2]);
							
						}
						
						break;
					
					default:
						break;
				}
			}
			else{
				
				J_down = 0;
			}
				
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
				/* Joytick J_Up pressed p1.29 */
				
				J_up++;
				switch(J_up){
					case 1:
						
						if(tempWall[0] > 0) {
							if (!wallGrid[tempWall[0]][tempWall[1]].isWall || (wallGrid[tempWall[0]][tempWall[1]].isWall && wallGrid[tempWall[0]][tempWall[1]].direction != tempWall[2])) {
								ClearWall(tempWall[0], tempWall[1], tempWall[2]);
							
								if (wallGrid[tempWall[0]][tempWall[1]].isWall) {
									DrawWall(tempWall[0], tempWall[1], wallGrid[tempWall[0]][tempWall[1]].direction); // Ridisegna il muro eventualmente cancellato in parte da tempWall 
								}
								
								if(wallGrid[tempWall[0]+1][tempWall[1]].isWall && tempWall[0]+1 <= 5) {
									DrawWall(tempWall[0]+1, tempWall[1], wallGrid[tempWall[0]+1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]-1][tempWall[1]].isWall && tempWall[0]-1 >= 0) {
									DrawWall(tempWall[0]-1, tempWall[1], wallGrid[tempWall[0]-1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]+1].isWall && tempWall[1]+1 <= 5) {
									DrawWall(tempWall[0], tempWall[1]+1, wallGrid[tempWall[0]][tempWall[1]+1].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]-1].isWall && tempWall[1]-1 >= 0) {
									DrawWall(tempWall[0], tempWall[1]-1, wallGrid[tempWall[0]][tempWall[1]-1].direction);
								}
							}
							tempWall[0]--;
							DrawWall(tempWall[0], tempWall[1], tempWall[2]);					
						}
					
						break;
					
					default:
						break;
				}
			}
			else{
				
					J_up=0;
			}
		
			if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
				/* Joytick J_Right pressed p1.28 */
				
				J_right++;
				switch(J_right){
					case 1:
						
						if (tempWall[1] < 5) {
							if (!wallGrid[tempWall[0]][tempWall[1]].isWall || (wallGrid[tempWall[0]][tempWall[1]].isWall && wallGrid[tempWall[0]][tempWall[1]].direction != tempWall[2])) {
								ClearWall(tempWall[0], tempWall[1], tempWall[2]);
							
								if (wallGrid[tempWall[0]][tempWall[1]].isWall) {
									DrawWall(tempWall[0], tempWall[1], wallGrid[tempWall[0]][tempWall[1]].direction); // Ridisegna il muro eventualmente cancellato in parte da tempWall 
								}
								
								if(wallGrid[tempWall[0]+1][tempWall[1]].isWall && tempWall[0]+1 <= 5) {
									DrawWall(tempWall[0]+1, tempWall[1], wallGrid[tempWall[0]+1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]-1][tempWall[1]].isWall && tempWall[0]-1 >= 0) {
									DrawWall(tempWall[0]-1, tempWall[1], wallGrid[tempWall[0]-1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]+1].isWall && tempWall[1]+1 <= 5) {
									DrawWall(tempWall[0], tempWall[1]+1, wallGrid[tempWall[0]][tempWall[1]+1].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]-1].isWall && tempWall[1]-1 >= 0) {
									DrawWall(tempWall[0], tempWall[1]-1, wallGrid[tempWall[0]][tempWall[1]-1].direction);
								}
							}
							tempWall[1]++;
							DrawWall(tempWall[0], tempWall[1], tempWall[2]);
						}
					
						break;
					
					default:
						break;
				}
			}
			else{
				
					J_right=0;
			}
		
			if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
				/* Joytick J_Up pressed p1.27 */
				
				J_left++;
				switch(J_left){
					case 1:
						
						if (tempWall[1] > 0) {
							if (!wallGrid[tempWall[0]][tempWall[1]].isWall || (wallGrid[tempWall[0]][tempWall[1]].isWall && wallGrid[tempWall[0]][tempWall[1]].direction != tempWall[2])) {
								ClearWall(tempWall[0], tempWall[1], tempWall[2]);
							
								if (wallGrid[tempWall[0]][tempWall[1]].isWall) {
									DrawWall(tempWall[0], tempWall[1], wallGrid[tempWall[0]][tempWall[1]].direction); // Ridisegna il muro eventualmente cancellato in parte da tempWall 
								}
								
								if(wallGrid[tempWall[0]+1][tempWall[1]].isWall && tempWall[0]+1 <= 5) {
									DrawWall(tempWall[0]+1, tempWall[1], wallGrid[tempWall[0]+1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]-1][tempWall[1]].isWall && tempWall[0]-1 >= 0) {
									DrawWall(tempWall[0]-1, tempWall[1], wallGrid[tempWall[0]-1][tempWall[1]].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]+1].isWall && tempWall[1]+1 <= 5) {
									DrawWall(tempWall[0], tempWall[1]+1, wallGrid[tempWall[0]][tempWall[1]+1].direction);
								}
								
								if(wallGrid[tempWall[0]][tempWall[1]-1].isWall && tempWall[1]-1 >= 0) {
									DrawWall(tempWall[0], tempWall[1]-1, wallGrid[tempWall[0]][tempWall[1]-1].direction);
								}
							}
							tempWall[1]--;
							DrawWall(tempWall[0], tempWall[1], tempWall[2]);
						}
					
						break;
					
					default:
						break;
				}
			}
			else{
				
					J_left=0;
			}
		
			if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
				/* Joytick J_Select pressed p1.25 */
				
				J_select++;
				switch(J_select){
					case 1:
						
						ClearMessage();
   					if (!wallGrid[tempWall[0]][tempWall[1]].isWall) {  // se la posizione non ha gia' un muro piazzato, allora consenti di piazzare il muro, altrimenti non faccio niente
							savePlayer = activePlayer;
							wallGrid[tempWall[0]][tempWall[1]].isWall = 1;
							wallGrid[tempWall[0]][tempWall[1]].direction = tempWall[2];
							trap1 = 0;
							trap2 = 0;
              if(!CheckTrap()) {
								wallGrid[tempWall[0]][tempWall[1]].isWall = 0;
								wallGrid[tempWall[0]][tempWall[1]].direction = 0;
								flag = 1;
								GUI_Text(15, 285, (uint8_t *) "    Can't block player", White, Black);
							}
							else {
								SaveMove(activePlayer->id, 1, tempWall[2], tempWall[0], tempWall[1]);
								activePlayer->walls--;
								WallCount();
								ChangeMode();
								changePlayer();
								DrawMoves(activePlayer);
							}
							ClearPossible();
						}
					
						break;
						
					default:
						break;
				}
			}
			else{
			
			
				
					J_select=0;
			}

		}
	}
	
	
	
	/* button management */
	
	if(down_0!=0) {
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			down_0++;
			switch(down_0){
				case 2:
					
					LCD_Clear(Black);
					quoridor();
					break;
				
				default:
					break;
			}
		}
		else {
			down_0=0;
			NVIC_EnableIRQ(EINT0_IRQn);						/* Riabilito INT0 a interrompere */
			LPC_PINCON->PINSEL4 |= (1 << 20);     /* Riattivo la modalita' di interrupt sul pin del bottone INT0*/
		}
	}
	
	if(down_1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down_1++;				
			switch(down_1){
				case 2:				/* pay attention here: please see slides to understand value 2 */		
					if (turnMode == 0) {
						if(activePlayer->walls > 0) {
							NVIC_EnableIRQ(EINT2_IRQn); 												// attivo KEY2 a interrompere cosi da poter girare i muri
							tempWall[0] = 3;
							tempWall[1] = 3;
							tempWall[2] = 0;
							ClearMoves(activePlayer);
							DrawWall(tempWall[0], tempWall[1], tempWall[2]);
						}
						else {
							GUI_Text(15, 285, (uint8_t *) " No walls left, move pawn", White, Black);
							ChangeMode();
						}
					}
					else {
						NVIC_DisableIRQ(EINT2_IRQn);												// disabilito KEY2 a interrompere nella Pawn Mode
						ClearWall(tempWall[0], tempWall[1], tempWall[2]);  
						DrawMoves(activePlayer);
					}
					ChangeMode();
				
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	
	if(down_2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down_2++;				
			switch(down_2){
				case 2:				/* pay attention here: please see slides to understand value 2 */		
				
					//anche qui va evitato che muri vicini vengano cancellati quando ruoto un muro..
					if (!wallGrid[tempWall[0]][tempWall[1]].isWall || (wallGrid[tempWall[0]][tempWall[1]].isWall && wallGrid[tempWall[0]][tempWall[1]].direction != tempWall[2])) {
						ClearWall(tempWall[0], tempWall[1], tempWall[2]);
					
						if (wallGrid[tempWall[0]][tempWall[1]].isWall) {
							DrawWall(tempWall[0], tempWall[1], wallGrid[tempWall[0]][tempWall[1]].direction); // Ridisegna il muro eventualmente cancellato in parte da tempWall 
						}
						
						if(wallGrid[tempWall[0]+1][tempWall[1]].isWall) {
							DrawWall(tempWall[0]+1, tempWall[1], wallGrid[tempWall[0]+1][tempWall[1]].direction);
						}
						
						if(wallGrid[tempWall[0]-1][tempWall[1]].isWall) {
							DrawWall(tempWall[0]-1, tempWall[1], wallGrid[tempWall[0]-1][tempWall[1]].direction);
						}
						
						if(wallGrid[tempWall[0]][tempWall[1]+1].isWall) {
							DrawWall(tempWall[0], tempWall[1]+1, wallGrid[tempWall[0]][tempWall[1]+1].direction);
						}
						
						if(wallGrid[tempWall[0]][tempWall[1]-1].isWall) {
							DrawWall(tempWall[0], tempWall[1]-1, wallGrid[tempWall[0]][tempWall[1]-1].direction);
						}
					}
					tempWall[2] = !(tempWall[2]);
					DrawWall(tempWall[0], tempWall[1], tempWall[2]);
				
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
	//reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
