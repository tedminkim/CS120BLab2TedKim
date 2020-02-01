/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #7  Exercise #1
*  Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTB is initially 7. Pressing PA0 increments PORTB once (stopping at 9). Pressing PA1 decrements PORTB once (stopping at 0).

*  I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "simAVRHeader.h"
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.


volatile unsigned char TimerFlag = 0;
//unsigned char countHold = 0x00;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}

void TimerOn() {
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}

void TimerOff() {
  TCCR1B = 0x00;
}

void TimerISR() {
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0) {
    TimerISR();
    _avr_timer_cntcurr = _avr_timer_M;
  }
}

void TimerSet(unsigned long M) {
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

enum States{Start, Init, Incr, Wait1, Decr, Wait2, Reset, Wait3} state;
unsigned char countHold = 0x00;


void TickButtonCount() {
  unsigned char tempA0 = PINA & 0x01;
  unsigned char tempA1 = PINA & 0x02;
  switch (state) {
    case Start:
      state = Init;
      break;
    case Init:
      if (tempA0 && tempA1) {
        state = Reset;
      }
      else if (tempA1) {
        state = Decr;
      }
      else if (tempA0) {
        state = Incr;
      }
      else {
        state = Init;
      }
      break;
    case Incr:
      state = Wait1;
      break;
    case Wait1:
      if (tempA0) {
        state = Incr;
      }
      else if (tempA0 && tempA1 ) {
        state = Reset;
      }
      else {
        state = Init;
      }
      break;
    case Decr:
      state = Wait2;
      break;
    case Wait2:
      if (tempA1) {
        state = Decr;
      }
      else if (tempA0 && tempA1) {
        state = Reset;
      }
      else {
        state = Init;
      }
      break;
    case Reset:
      state = Wait3;
      break;
    case Wait3:
      if ((tempA0) || (tempA1)) {
        state = Reset;
      }
      else {
        state = Init;
      }
      break;
    default:
      break;
  }
  switch (state) {
    case Start:
      //countHold = 7;
      break;
    case Init:
      break;
    case Reset:
      countHold = 0;
      break;
    case Incr:
      if (countHold < 9) {
        countHold = countHold + 1;
      }
      break;
    case Wait1:
      break;
    case Wait2:
      break;
    case Decr:
      if (countHold > 0) {
        countHold = countHold - 1;
      }
      break;
    case Wait3:
      break;
    default:
      break;
  }
}

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;
  DDRD = 0xFF;
  PORTA = 0xFF;
  PORTC = 0x00;
  PORTD = 0x00;



  //unsigned char tempValA = 0x00;
  //unsigned char tempValC = 0x00;
  TimerSet(1000);
  TimerOn();
  LCD_init();
  LCD_ClearScreen();
  countHold = 0;
  //unsigned char tempValB = PORTB;
  state = Start;

  while(1) {
    LCD_Cursor(1);
    TickButtonCount();
    PORTC = countHold;
    LCD_WriteData(countHold + '0');
    while (!TimerFlag) {}
    TimerFlag = 0;
    //tempValB = out;
    //PORTC = countHold;
  }
  return 0;
}
