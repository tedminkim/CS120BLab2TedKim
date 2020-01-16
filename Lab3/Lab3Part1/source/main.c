/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: None
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #3  Exercise #1
*	 Exercise Description: Count the number of 1s on ports A and B and output that number on port C.

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
  DDRB = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTB = 0xFF;
  PORTC = 0x00;

  unsigned char total = 0x00;

  while(1) {
   unsigned char tmpA = PINA;
   unsigned char tmpB = PINB;
   unsigned char i = 0;
    for (i = 0; i < 8; i++) {
      total = total + GetBit(tmpA, i) + GetBit(tmpB, i);
    }
    PORTC = total;
    total = 0;
  }
  return 0;
}
