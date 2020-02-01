/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #7  Exercise #2
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
enum States{Start, Wait, Led0, Led1, Led2} state;


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

unsigned char out = 0x00;
// unsigned char press = 0x00;
unsigned char where;
unsigned char scoreCount = 0x00;

void TickLEDButton() {
  unsigned char A0 = ~PINA & 0x01;
  switch(state) {
    case Start:
      state = Led0;
      break;
    case Led0:
      if (A0) {
        state = Wait;
        break;
      }
      else {
        state = Led1;
      }
      break;
    case Led1:
      if (A0) {
        state = Wait;
      }
      else {
        state = Led2;
      }
      break;
    case Led2:
      if (A0) {
        state = Wait;
      }
      else {
        state = Led0;
      }
      break;
    case Wait:
      if (!A0) {
        switch(where) {
          case 1:
            state = Led0;
            score = scoreCount + 0x02;
            break;
          case 2:
            state = Led1;
            score = scoreCount + 0x02;
            break;
          case 3:
            state = Led2;
            score = scoreCount + 0x02;
            break;
        }
        where = 0x00;
      }
      else {
        state = Wait;
      }
      break;
    default:
      state = Start;
      scoreCount = scoreCount - 1;
      break;
  }
  switch(state) {
    case Start:
      if (scoreCount > 0) {
        scoreCount = scoreCount - 1;
      }
      break;
    case Led0:
      //where = 0x01;
      out = 0x01;
      PORTC = out;
      where = 0x01;
      //wait = 0;
      break;
    case Led1:
      out = 0x02;
      PORTC = out;
      where = 0x02;
      break;
    case Led2:
      out = 0x04;
      PORTC = out;
      where = 0x03;
      break;
    case Wait:
      out = out;
      PORTC = out;
      break;
    default:
      state = Start;
      break;
  }
}

int main(void) {
  DDRA = 0x00;
  DDRB = 0xFF;
  DDRC = 0xFF;
  DDRD = 0xFF;
  PORTA = 0xFF;
  PORTB = 0x00;
  PORTC = 0x00;
  PORTD = 0x00;



  //unsigned char tempValA = 0x00;
  //unsigned char tempValC = 0x00;
  TimerSet(50);
  TimerOn();
  LCD_init();
  LCD_Cursor(1);
  LCD_ClearScreen();
  scoreCount = 5;
  //unsigned char tempValB = PORTB;
  state = Start;

  while(1) {
    LCD_ClearScreen();
    TickLEDButton();
    if (scoreCount >= 9) {
      LCD_DisplayString(1, "Congraulations! You win!");
      break;
    }
    else {
      LCD_WriteData(scoreCount + '0');
    }

   // LCD_WriteData(countHold + '0');
    while (!TimerFlag) {}
    TimerFlag = 0;
    PORTB = out;
    //tempValB = out;
    //PORTC = countHold;
  }
  return 0;
}
