/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #5  Exercise #1
*  Exercise Description: A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full).

*  I acknowledge all content contained herein, excluding template or example
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
    tempValA = PINA;
    if (tempValA == 0x01 || tempValA == 0x02) {
      tempValC = 0x60;
    }
    else if (tempValA == 0x03 || tempValA == 0x04) {
      tempValC = 0x70;
    }
    else if (tempValA == 0x05 || tempValA == 0x06) {
      tempValC = 0x28;
    }
    else if (tempValA >= 0x07 &&  tempValA <= 0x09) {
      tempValC = 0x2C;
    }
    else if (tempValA >= 0x0A && tempValA <= 0x0C) {
      tempValC = 0x2E;
    }
    else if (tempValA >= 0x0D && tempValA <= 0x0F) {
      tempValC = 0x2F;
    }

    PORTC = tempValC;
  }
  return 0;
}
