/* Includes ------------------------------------------------------------------*/
#include "GLCD.c" 
#include "AsciiLib.h"

void chest() {
	LCD_DrawLine(0, 0, 0, 20, White);
	LCD_DrawLine(0, 0, 20, 0, White);
}