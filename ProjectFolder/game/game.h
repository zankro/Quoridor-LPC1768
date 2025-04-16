#include <stdbool.h>
#include <stdint.h>

typedef struct player {
	bool id;														// 0 = Player1 1 = Player2
	int position[2];										// Defines Player position on the grid, first element of array is Y (row) coordinate, second element of array is X (column) coordinate
	uint16_t* pawnImg;									// Pointer to pawn image to display
	
	int walls;													// Counter of available walls
	int nextMove;												// Up=0 Down=1 Left=2 Right=3 (For RIT joystick)
	int jump;														// Defines direction of jump if facingPawns == 1
	
} player;

typedef	struct cross 
{
	bool isWall;												// 0 = Wall not placed 1 = Wall placed
	bool direction;											// 0 = Horizontal 1 = Vertical
	
} cross;

typedef	struct cell 									// cell struct is used to create possible[7][7] matrix. It represents the game grid
{
	bool valid;													// if a move towards possible[i][j] is possible, then this value is set to 1
	bool visited;												// if a move towards possible[i][j] has already been checked, this value is set to 1, so no need to check again
	
} cell;


extern player player1;								// Defines a player struct for Player 1
extern player player2;								// Defines a player struct for Player 2

extern player *activePlayer;					// Pointer to the active player for each turn
extern player *savePlayer;						// Used to save pointer to the active player when calling CheckTrap

extern bool nextPlayer;								// Boolean value to keep track of the next player who has to play in order to correctly change activePlayer pointer

extern cross wallGrid[6][6];					// Defines a 6x6 matrix that keeps track of the walls placed during the game
extern int tempWall[3];								// Defines temporary wall position. This is useful to keep track of last choosen position of the wall to place in wall mode. [0] is Y (row), [1] is X (column), [2] is direction

extern bool turnMode;									// Defines game mode. 0 = Pawn Mode, 1 = Wall Mode
extern bool facingPawns;							// Boolean value to check if players' pawn are facing, thus a jump might be available
extern int time;											// Value used my TimeCount() to count 20 seconds. Initialized to 20. Each time TimeCount() is called (by Timer0 interrupts, each second),
																			// the value is decreased and shown on display 

extern bool win;											// Boolean value to check if a player has won, thus the game should end.
extern unsigned int move;							// Variable used to save player's move
extern bool trap1;										// trap1 and trap2 are used to check if player1 or player2 or both are trapped, thus the wall can't be placed in choosen position
extern bool trap2;
extern bool flag;											// flag is used CheckTrap() and CheckGrid() functions to check if possible moves' matrix has changed
extern cell possible[7][7];						// 7x7 "cell" struct matrix used to keep track of possible moves available to players when trying to place a wall.
extern player fakePlayer;							// player struct used in CheckGrid() function to keep track of available moves for the player

/* Initializes game by setting initial values to variables (players positions', for instance) and draws the game elements (grid, players, walls counters and time counter */
void quoridor();			

/* Draws game grid and all initial graphical elements for the game (players' pawns, walls counters, time counter etc..) */
void grid();

/* Clears cell at choosen position on the grid*/
void ClearCell(int startX, int startY);

/* Draws player's pawn at player's position */
void DrawPawn2(player* player);

/* Draws available suggested moves for the player */
void DrawMoves(player* player);

/* Clears available suggested moves for the player when changing turn */
void ClearMoves(player* player);

/* Draws a wall in the choosen position, in the choosen direction */
void DrawWall(int xPosition, int yPosition, bool direction);

/* Clears wall, when changing wall position or when time is up and turn is to be changed */
void ClearWall(int xPosition, int yPosition, bool direction);

/* Clears messages displayed in the lower zone of the display */
void ClearMessage();

/* Changes the pointer to current active player */
void changePlayer();

/* Change game mode (move pawn or place wall) */
void ChangeMode();

/* Displays number of available walls for the player */
void WallCount();

/* Displays the time counter */
void TimeCount();

/* Reset time counter when changing turn */
void ResetTime();

/* Checks if choosen move is valid when trying to move the pawn */
bool ValidMove(int move);

/* Checks if players' pawns are "facing", thus a jump might be available */
void CheckFacing();

/* Checks if player has reached the goal and stops the game*/
bool CheckWin();

/* Saves the move*/
void SaveMove(int playerId, int mode, int vert_horiz, int y, int x);

/* Checking if players are trapped when trying to place a wall */
bool CheckTrap();
void CheckGrid(player* player);

/* Initializes matrix used by CheckGrid */
void ClearPossible();