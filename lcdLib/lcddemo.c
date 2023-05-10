/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  //  clearScreen(COLOR_BLUE);

  //  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  //  fillRectangle(30,30, 60, 60, COLOR_ORANGE);

  clearScreen(COLOR_WHITE);
  int base = screenHeight - 10;

  for(int i = 0; i < 15; i++)
    fillRectangle(0, base - (3*i), 60 - (4*i), 3, COLOR_BLACK);

  for(int i = 0; i < 18; i++)
    fillRectangle(30 + (2*i), base - (3*i), 70 - (4*i), 3, COLOR_BLACK);

  for(int i = 0; i < 16; i++)
    fillRectangle(36 + (3*i), base - (3*i), 64 - (4*i), 3, COLOR_BLACK);
  
  for(int i = 0; i < 15; i++)
    fillRectangle(0 + (2*i), base - (3*i), 60 - (4*i), 3, COLOR_GRAY);

  for(int i = 0; i < 9; i++)
    fillRectangle(50 + (3*i), base - (3*i), 50 - (6*i), 3, COLOR_GRAY);

  for(int i = 0; i < 20; i++)
    fillRectangle(80 + (2*i), base - (3*i), 80 - (4*i), 3, COLOR_GRAY);

  drawString5x7(20, 20, "5x7 Hello", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(20, 50, "8x12 Hello", COLOR_WHITE, COLOR_BLACK);
  
  fillRectangle(0, base, screenWidth, 10, COLOR_BLACK);
  fillRectangle(10, base - 10, 10, 10, COLOR_BLACK);

}
