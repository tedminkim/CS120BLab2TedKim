/*	Author: Ted Kim
 *  Email: tkim094@ucr.edu
 *  Partner(s) Name: None
 *	Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: Garage Open at Night
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 #include <avr/io.h>
 #ifdef _SIMULATE_
 #include “simAVRHeader.h”
 #endif	



int main(void) {
    /* Insert DDR and PORT initializations */
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  unsigned char tmpB = 0x00;
  unsigned char tmpA = 0x00;

    /* Insert your solution below */
    while (1) {


      tmpA = PINA;
      if (tmpA != 0x01) {
	       tmpB = 0x00;
      }
      else {
	       tmpB = 0x01;
      }
      PORTB = tmpB;
    }
    return 0;
}
