/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: None
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #4  Exercise #2
*	 Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC once (stopping at 9).

*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"

enum States{Start, Init, Incr, Decr, Reset} state;
unsigned char countHold = 0x00;


void TickButtonCount() {
  unsigned char tempA = PINA;
  switch (state) {
    case Start:
      state = Init;
      break;
    case Init:
      if (tempA == 0) {
        state = Reset;
      }
      else if (tempA == 1) {
        state = Incr;
      }
      else if (tempA == 2) {
        state = Decr;
      }
      else {
        state = Init;
      }
      break;
    case Incr:
      state = Init;
      break;
    case Decr:
      state = Init;
      break;
    case Reset:
      state = Init;
      break;
  }
  switch (state) {
    case Start:
      break;
    case Init:
      break;
    case Incr:
      if (countHold < 9) {
        countHold = countHold + 1;
      }
      break;
    case Reset:
      countHold = 0;
      break;
    case Decr:
      if (countHold > 0) {
        countHold = countHold - 1;
      }
      break;
  }
}

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTC = 0x00;

  state = Start;
  countHold = 6;

  while(1) {
    TickButtonCount();
    PORTC = countHold;
  }
  return 0;
}
