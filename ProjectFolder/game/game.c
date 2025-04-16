#include "../GLCD/GLCD.h"
#include "game.h"
#include "../GLCD/red_pawn.c"
#include "../GLCD/white_pawn.c"
#include "../timer/timer.h"
#include <stdlib.h>
#include "../RIT/RIT.h"

cross wallGrid[6][6];
player player1;
player player2;
player *activePlayer;
bool nextPlayer;
bool turnMode;
bool facingPawns;
char string[4] = "";
int time;
bool win;
unsigned int move;
player fakePlayer;
bool flag;
cell possible[7][7];
bool trap1;
bool trap2;
player *savePlayer;


void ResetTime() {
	time = 20;
	TimeCount();	
}

void changePlayer() {
	nextPlayer = !(nextPlayer);
	
	if(nextPlayer == 0) {
		activePlayer = &player1;
	}
	else {
		activePlayer = &player2;
	}
	
	ResetTime();
}

void ChangeMode() {
	turnMode = !(turnMode); 
}


void WallCount() {
	char count[4] = "";
	
	sprintf(count, "%d", activePlayer->walls);
	if (activePlayer->id == 0) {
		GUI_Text(50, 255, (uint8_t *) count, White, Black);
	}
	else {
		GUI_Text(185, 255, (uint8_t *) count, White, Black);
	}
	
}

void TimeCount() {
	char count[4] = "";
	
	sprintf(count, "%d", time);

	if (time >= 10) {
		GUI_Text(110, 255, (uint8_t *) count, White, Black);
	}
	else {
		GUI_Text(118, 255, (uint8_t *) count, White, Black);
	}
	
	
}

void ClearMessage() {
	int i;
	for (i = 0; i<20; i++) {
		LCD_DrawLine(0, 285+i, 240, 285+i, Black);
	}
}

bool ValidMove(int move) {
	cross wallLeft; 
	cross wallRight;
	bool prova;
	
	switch(move) {
		case 0: 					// Up Move
			
			if((activePlayer->position[0]) > 0 && ((activePlayer->position[0] != 1 || !(facingPawns && activePlayer->jump == 0)))) {
				if(facingPawns && activePlayer->jump == 0) {
					wallLeft = wallGrid[activePlayer->position[0]-2][activePlayer->position[1]-1];
					wallRight = wallGrid[activePlayer->position[0]-2][activePlayer->position[1]];
				}
				else {
					wallLeft = wallGrid[activePlayer->position[0]-1][activePlayer->position[1]-1];
					wallRight = wallGrid[activePlayer->position[0]-1][activePlayer->position[1]];
				}

				if(!wallLeft.isWall && !wallRight.isWall) {
					return 1;
				}
				if((wallLeft.isWall && wallLeft.direction == 0) || (wallRight.isWall && wallRight.direction == 0)) {   // if none of the present walls is horizontal, then the move is valid
					return 0;
				}
				else {
					return 1;
				}
			}
			else
				return 0;
			
			break;
		
		case 1: 					// Down Move
			
			if((activePlayer->position[0]) < 6 && ((activePlayer->position[0] != 5 || !(facingPawns && activePlayer->jump == 1)))) {
				if(facingPawns && activePlayer->jump == 1) {
					wallLeft = wallGrid[activePlayer->position[0]+1][activePlayer->position[1]-1];
					wallRight = wallGrid[activePlayer->position[0]+1][activePlayer->position[1]];
				} 
				else {
					wallLeft = wallGrid[activePlayer->position[0]][activePlayer->position[1]-1];
					wallRight = wallGrid[activePlayer->position[0]][activePlayer->position[1]];
				}
				
			
				if(!wallLeft.isWall && !wallRight.isWall) {
					return 1;
				}
				if((wallLeft.isWall && wallLeft.direction == 0) || (wallRight.isWall && wallRight.direction == 0)) {	
					return 0;
				}
				else {
					return 1;
				}
			}
			else
				return 0;
			
			break;
			
		case 2: 					// Left Move
			
			if((activePlayer->position[1]) > 0 && ((activePlayer->position[1] != 1 || !(facingPawns && activePlayer->jump == 2)))) {
				if(facingPawns && activePlayer->jump == 2) {
					wallLeft = wallGrid[activePlayer->position[0]][activePlayer->position[1]-2];
					wallRight = wallGrid[activePlayer->position[0]-1][activePlayer->position[1]-2];
				} 
				else {
					wallLeft = wallGrid[activePlayer->position[0]][activePlayer->position[1]-1];
					wallRight = wallGrid[activePlayer->position[0]-1][activePlayer->position[1]-1];	
				}
				
			
				if(!wallLeft.isWall && !wallRight.isWall) {
					return 1;
				}
				if((wallLeft.isWall && wallLeft.direction == 1) || (wallRight.isWall && wallRight.direction == 1)) {	
					return 0;
				}
				else {
					return 1;
				}
			}
			else
				return 0;
			
			break;
			
		case 3: 					// Right Move
																						/* QUIIIIIIIIIIIII*/
			prova = !(facingPawns && activePlayer->jump == 3);
			if((activePlayer->position[1]) < 6 && ((activePlayer->position[1] != 5 || !(facingPawns && activePlayer->jump == 3)))) {
				if(facingPawns && activePlayer->jump == 3) {
					wallLeft = wallGrid[activePlayer->position[0]-1][activePlayer->position[1]+1];
					wallRight = wallGrid[activePlayer->position[0]][activePlayer->position[1]+1];
				} 
				else {
					wallLeft = wallGrid[activePlayer->position[0]-1][activePlayer->position[1]];
					wallRight = wallGrid[activePlayer->position[0]][activePlayer->position[1]];
				}
				
				if(!wallLeft.isWall && !wallRight.isWall) {
					return 1;
				}
				if((wallLeft.isWall && wallLeft.direction == 1) || (wallRight.isWall && wallRight.direction == 1)) {		
					return 0;
				}
				else {
					return 1;
				}
			}
			else
				return 0;
			
			break;
			
		
		default:
				break;
	}
}

void CheckFacing() {
	if((player1.position[1] == player2.position[1]) && (abs(player1.position[0] - player2.position[0])) == 1 ) {
		facingPawns = 1;
	
		if(player1.position[0] > player2.position[0]) {   // if white pawn vertical position (row) is greater than red pawn's 
			
			player1.jump = 0; // up			
			player2.jump = 1; // down
			
		}
		else {
			
			player1.jump = 1; // down
			player2.jump = 0; // up
			
		}
		
	}
	else {
		
		if((player1.position[0] == player2.position[0]) && (abs(player1.position[1] - player2.position[1])) == 1 ) {
			facingPawns = 1;
		
			if(player1.position[1] > player2.position[1]) {
				
					player1.jump = 2; // left
					player2.jump = 3; // right
				
			}
			else {
				
					player2.jump = 2; // left
					player1.jump = 3; // right
				
			}
			
			
		}
		else {
			facingPawns = 0;
		}
		
	}

}

bool CheckWin() {	
	if ((activePlayer->id == 0 && activePlayer->position[0] == 0) || (activePlayer->id == 1 && activePlayer->position[0] == 6)) {
		win = 1;
		disable_timer(0);
		NVIC_DisableIRQ(EINT1_IRQn);
		
		if (activePlayer->id == 0)
			GUI_Text(0, 140, (uint8_t *) "   			Player 1 Wins!           ", Red, Grey);
		else 
			GUI_Text(0, 140, (uint8_t *) "   			Player 2 Wins!           ", Red, Grey);
		
		GUI_Text(0, 155, (uint8_t *) "  		Press INT0 to play again! ", Red, Grey);
		
		return 1;
	}	

		return 0;
}

void SaveMove(int playerId, int mode, int vert_horiz, int y, int x) {
	move = 0;
	
	move = move + playerId;
	move <<= 4;
	move = move + mode;
	move <<= 4;
	
	if (turnMode == 0) 
		move = move + 0;
	else 
		move = move + vert_horiz;
	
	move <<= 8;
	move = move + y;
	move <<= 8;
	move = move + x;
	
}

bool CheckTrap() {
	int i, j, k;

	activePlayer = &fakePlayer; 
	if (trap1 == 0) {
		if (flag == 1) {
			CheckGrid(&player1);
		} 
		else {
			
			for(i=0; i<7; i++) {
				
				if(possible[0][i].valid == 1) {
					trap1 = 1;
					flag = 1;
					for(j=0; j<7; j++) {
						for(k=0;k<7;k++) {
							possible[j][k].visited = 0;
							possible[j][k].valid = 0;
						}
					}
					break;
				}				
			}
			
		}
		
		if(trap1 == 0) {
			activePlayer = savePlayer;
			return 0;
		}
		
	}

	else {
		if (flag == 1) {
			CheckGrid(&player2);
		}
		else {
			
			for(i=0; i<7; i++) {
				
				if(possible[6][i].valid == 1) {
					trap2 = 1;
					flag = 1;
					for(j=0; j<7; j++) {
						for(k=0;k<7;k++) {
							possible[j][k].visited = 0;
							possible[j][k].valid = 0;
						}
					}
					break;
				}				
			}
			
		}
		
		if(trap2 == 0) {
			activePlayer = savePlayer;
			return 0;
		}
		
	}
	
	activePlayer = savePlayer;
	return 1;
}

void CheckGrid(player* player) {
	int i;
	int j;
	int x = player->position[0];
	int y = player->position[1];
	flag = 0;
	possible[x][y].visited = 1;
	
	for(i=0; i<7; i++) {
		for(j=0; j<7; j++) {
			
			if(possible[i][j].visited == 1) {
				fakePlayer.position[0] = i; // up
				fakePlayer.position[1] = j;
				fakePlayer.jump = savePlayer->jump;
				
				if(ValidMove(0) == 1 && possible[fakePlayer.position[0]-1][fakePlayer.position[1]].visited == 0) {
					flag = 1;
					possible[fakePlayer.position[0]-1][fakePlayer.position[1]].valid = 1;
					possible[fakePlayer.position[0]-1][fakePlayer.position[1]].visited = 1;
				}
				
				if(ValidMove(1) == 1 && possible[fakePlayer.position[0]+1][fakePlayer.position[1]].visited == 0) {
					flag = 1;
					possible[fakePlayer.position[0]+1][fakePlayer.position[1]].valid = 1;
					possible[fakePlayer.position[0]+1][fakePlayer.position[1]].visited = 1;
				}
				
				if(ValidMove(2) == 1 && possible[fakePlayer.position[0]][fakePlayer.position[1]-1].visited == 0) {
					flag = 1;
					possible[fakePlayer.position[0]][fakePlayer.position[1]-1].valid = 1;
					possible[fakePlayer.position[0]][fakePlayer.position[1]-1].visited = 1;
				}
				
				if(ValidMove(3) == 1 && possible[fakePlayer.position[0]][fakePlayer.position[1]+1].visited == 0) {
					flag = 1;
					possible[fakePlayer.position[0]][fakePlayer.position[1]+1].valid = 1;
					possible[fakePlayer.position[0]][fakePlayer.position[1]+1].visited = 1;
				}
				
			}
			
		}
	}
	
	CheckTrap();
	
}

void ClearPossible() {
	int i;
	int j;
	
	for(i=0;i<7;i++) {
		for(j=0;j<7;j++) {
			possible[i][j].valid = 0;
			possible[i][j].visited = 0;
		}
	}
}

void quoridor() {
	int i=0;
	int j=0;
	
	for (i=0; i<6; i++) {
		for (j=0; j<6; j++) {
			wallGrid[i][j].isWall = 0;
			wallGrid[i][j].direction = 0;
		}
	}
	
	ClearCell(player1.position[0], player1.position[1]);
	ClearCell(player2.position[0], player2.position[1]);
	
	player1.id = 0;
	player1.position[0] = 6;
	player1.position[1] = 3;
	player1.pawnImg = (uint16_t*)&(WHITE_PAWN_pixel_data);
	player1.walls = 8;
	
	player2.id = 1;
	player2.position[0] = 0;
	player2.position[1] = 3;
	player2.pawnImg = (uint16_t*)&(RED_PAWN_pixel_data);
	player2.walls = 8;
	
	nextPlayer = 0;
	activePlayer = &player1;
	
	turnMode = 0;
	facingPawns = 0;
	
	trap1 = 0;
	trap2 = 0;
	flag = 1;
	
	win = 0;
	
	
	NVIC_EnableIRQ(EINT1_IRQn);								// Enable interrupt on KEY1 so that we can use KEY1 while in game
	
	grid();
	enable_RIT();		
	enable_timer(0);
	ResetTime();
}