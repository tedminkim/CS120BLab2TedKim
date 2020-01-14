/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: None
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #3  Exercise #3
*	 Exercise Description:  PC7 should light a "Fasten seatbelt" icon if a key is in the ignition, the driver is seated, but the belt is not fastened.

*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
  return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
  return ((x & (0x01 << k)) != 0);
}

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTC = 0x00;

  unsigned char tempValA = 0x00;
  unsigned char tempValC = 0x00;


  while(1) {
    tempValA = PINA & 0x0F;
    if (tempValA == 0x01 || tempValA == 0x02) {
      tempValC = 0x60;
    }
    else if (tempValA == 0x03 || tempValA == 0x04) {
      tempValC = 0x70;
    }
    else if (tempValA == 0x05 || tempValA == 0x06) {
      tempValC = 0x28;
    }
    else if (tempValA == 0x07 || tempValA == 0x08 || tempValA == 0x09) {
      tempValC = 0x2C;
    }
    else if (tempValA == 0x0A || tempValA == 0x0B || tempValA == 0x0C) {
      tempValC = 0x2E;
    }
    else if (tempValA == 0x0D || tempValA == 0x0E || tempValA == 0x0F) {
      tempValC = 0x2F;
    }

    PORTC = tempValC;
    if ((GetBit(tempValA & 0xF0, 4) && (GetBit(tempValA & 0xF0, 5) && (!GetBit(tempValA & 0xF0, 6))))) {
      tempValC = SetBit(tempValC, 7, 1);
    }
    PORTC = tempValC;
  }
  return 0;
}
