/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #4  Exercise #1
*	 Exercise Description: Button press - one light off, other light on.

*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"

enum States{Start, Init, Wait, Light, WaitP2} state;
unsigned char stateCurr = 0x00;
unsigned char tempA = 0x00;


void TickMachine() {
  tempA = PINA;
  switch(state) {
    case Start:
      state = Init;
      break;
    case Init:
      if ((tempA & 0x01) == 1) {
        state = Wait;
      }
      else {
        state = Init;
      }
      break;
    case Wait:
      if (tempA == 0) {
        state = Light;
      }
      else {
        state = Wait;
      }
      break;
    case Light:
      if ((tempA & 0x01) == 1) {
        state = WaitP2;
      }
      else {
        state = Light;
      }
      break;
    case WaitP2:
      if (tempA == 0) {
        state = Init;
      }
      else {
        state = WaitP2;
      }
      break;
  }
  switch(state) {
    case Start:
      break;
    case Init:
      stateCurr = 0x01;
      PORTB = stateCurr;
      break;
    case Wait:
      break;
    case Light:
      stateCurr = 0x02;
      PORTB = stateCurr;
      break;
    case WaitP2:
      break;
  }
}
int main(void) {

  DDRA = 0x00;
  DDRB = 0xFF;
  PORTA = 0xFF;
  PORTB = 0x00;

  state = Start;

  while(1) {
    Tickmachine();
  }
  return 0;
}
