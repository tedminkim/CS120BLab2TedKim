/* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: None
*	 Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*	 Assignment: Lab #2  Exercise #2
*	 Exercise Description:  Write a program that outputs in binary on port C the number of available spaces.
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;
  PORTA = 0xFF;
  PORTC = 0x00;

  unsigned char tmpA = 0x00;
  unsigned char cntavail = 0xFF;

  while(1) {
    cntavail = 0x00;

    tmpA = PINA & 0x01; // this is BITWISE.
    if (tmpA == 0x00) {
      cntavail = cntavail + 1;
    }
    tmpA = PINA & 0x02; // this is BITWISE.
    if (tmpA == 0x00) {
      cntavail = cntavail + 1;
    }
    tmpA = PINA & 0x04; // this is BITWISE.
    if (tmpA == 0x00) {
      cntavail = cntavail + 1;
    }
    tmpA = PINA & 0x08; // this is BITWISE.
    if (tmpA == 0x00) {
      cntavail = cntavail + 1;
    }
    PORTC = cntavail;
  }


  return 0;
}
