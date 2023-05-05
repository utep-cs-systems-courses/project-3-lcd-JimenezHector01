#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15


unsigned char step = 0;
const int base = screenHeight - 10;

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}


// axis zero for col, axis 1 for row

short drawPos[2] = {10,base-10}, controlPos[2] = {10, base-20};
short rowVelocity = 3, rowLimits[2] = {base-50, base-10};

void
draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 10, 10, color);
}

draw_Obs(int col, int row, unsigned short color)
{
  fillRectangle(col, row, 4, 7, color);
}

void
screen_update_ball()
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_WHITE); /* erase */
  for (char axis = 0; axis < 2; axis ++) 
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_BLACK); /* draw */
}

short startObs[2] = {screenWidth, base-7}, obsControl[2] = {screenWidth, base-7};
short obsVelocity = 3, obsLimit = -4;

void screen_update_obs()
{
  for(char axis = 0; axis<2; axis++)
    if(startObs[axis] != obsControl[axis])
       goto redraw;
    return;
   redraw:
       draw_Obs(startObs[0], startObs[1], COLOR_WHITE);
       for(char axis = 0; axis < 2; axis++)
	 startObs[axis] = obsControl[axis];
       draw_Obs(startObs[0], startObs[1], COLOR_GREEN);
}
short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;
  int count = 0;
  secCount ++;

  if (secCount%25 == 0) {		/* 10/sec */   
    {				/* move ball */
      short oldRow = controlPos[1];
      short newRow = oldRow + rowVelocity;
      if (newRow <= rowLimits[0] || newRow >= rowLimits[1])
	rowVelocity = -rowVelocity; 
      else
	controlPos[1] = newRow;
    }

    {
      short oldObs = startObs[0];
      short newObs = oldObs + obsVelocity;
      if(newObs <= obsLimit)
	newObs = startObs;
      else
	obsControl[0] = newObs;
    }

    
    {
      if(step <= 30)
	step++;
      else
	step = 0;
      //secCount = 0;
    }
    redrawScreen = 1;
  }
}
  
void update_shape();

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_WHITE);
  fillRectangle(0, base, screenWidth, screenHeight-base, COLOR_BLACK);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

void update_shape()
{
  screen_update_ball();
  screen_update_obs();
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
