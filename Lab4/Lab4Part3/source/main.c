/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: None
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #4  Exercise #3
*	 Exercise Description:  Household Keypad: X, Y, #.

*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"

enum States{Start, Init, KeyX, KeyY, KeyHash, Unlocked} state;

void TickKeypadSM() {
  unsigned char tempA = PINA;
  switch(state) {
    case Start:
      state = Init;
      break;
    case Init:
      if (tempA == 1) {
        state = KeyX;
      }
      else {
        state = Init;
      }
      break;
    case KeyX:
      if (tempA == 1) {
        state = KeyX;
      }
      else if (tempA == 0) {
        state = KeyY;
      }
      else {
        state = Init;
      }
      break;
    case KeyY:
      if (tempA == 2) {
        state = KeyHash;
      }
      else if (tempA == 0) {
        state = Unlocked;
      }
      else {
        state = Init;
      }
      break;
    case Unlocked:
      if (tempA == 0x80) {
        state = Init;
      }
      else {
        state = Unlocked;
      }
      break;
  }
  switch (state) {
    case Start:
      break;
    case Init:
      PORTC = 0x00;
      break;
    case KeyX:
      break;
    case KeyY:
      break;
    case KeyHash:
      break;
    case Unlocked:
      PORTC = 0x01;
      break;
  }
}


int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTC = 0x00;

  while(1) {
    state = Start;
    TickKeypadSM();
  }
  return 0;
}
