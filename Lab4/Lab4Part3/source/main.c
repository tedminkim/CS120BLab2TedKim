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

void TickKeypadSM() {

}


int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTC = 0x00;

  unsigned char tempValA = 0x00;
  unsigned char tempValC = 0x00;


  while(1) {
    state = Start;
    TickKeypadSM();
  }
  return 0;
}
