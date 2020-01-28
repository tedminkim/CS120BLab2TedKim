/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner Email: kchen161@ucr.edu
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #5  Exercise #2
*	 Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC once (stopping at 9).

*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"

enum States{Start, Init, Incr, Wait1, Decr, Wait2, Reset} state;
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
      break;
    case Incr:
      state = Wait1;
      break;
    case Wait1:
      if (tempA0 && tempA1) {
        state = Reset;
      }
      if (!tempA0) {
        state = Init;
      }
      break;
    case Decr:
      state = Wait2;
      break;
    case Wait2:
      if (tempA0 && tempA1) {
        state = Reset;
      }
      if (!tempA1) {
        state = Init;
      }
      break;
    case Reset:
      state = Init;
      break;
    default:
      break;
  }
  switch (state) {
    case Start:
      countHold = 7;
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
    default:
      break;
  }
}

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTC = 0x00;

  state = Start;
  countHold = 7;

  while(1) {
    TickButtonCount();
    PORTC = countHold;
  }
  return 0;
}
