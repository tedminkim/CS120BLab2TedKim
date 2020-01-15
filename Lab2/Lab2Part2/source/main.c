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
#include "simAVRHeader.h"

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;
  PORTA = 0xFF;
  PORTC = 0x00;

  unsigned char tmpValA = 0x00;
  unsigned char tmpValA1 = 0x00;
  unsigned char tmpValA2 = 0x00;
  unsigned char tmpValA3 = 0x00;
  unsigned char cntavail = 4;

  while(1) {
    //cntavail = 0x00;

    tmpValA = PINA & 0x01; // this is BITWISE.
    if (tmpValA == 0x01) {
      cntavail = cntavail - 1;
    }
    tmpValA1 = PINA & 0x02; // this is BITWISE.
    if (tmpValA1 == 0x02) {
      cntavail = cntavail - 1;
    }
    tmpValA2 = PINA & 0x04; // this is BITWISE.
    if (tmpValA2 == 0x04) {
      cntavail = cntavail - 1;
    }
    tmpValA3 = PINA & 0x08; // this is BITWISE.
    if (tmpValA3 == 0x08) {
      cntavail = cntavail - 1;
    }
    PORTC = cntavail;
    cntavail = 4;
  }


  return 0;
}
