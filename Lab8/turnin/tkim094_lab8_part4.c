/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #8  Exercise #4
*  Exercise Description: (Challenge) Design a system, using a bank of eight LEDs, where the number of LEDs illuminated is a representation of how much light is detected



*  I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"
//#include "io.h"

void ADC_init() {
  ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;
  //DDRD = 0xFF;
  PORTA = 0xFF;
  PORTC = 0x00;
  //PORTD = 0x00;
  unsigned short maxVal = 0x0278;
  unsigned short minVal = 0x000C;
  ADC_init();

  while(1) {
    unsigned short sVal = ADC; //ADC value in variable
    if (sVal <= 0) {
      PORTC = 0x00;
    }
    else if (sVal < 0x004F && sVal >= 0x000) {
      PORTC = 0x01;
    }
    else if (sVal < 0x009E && sVal >= 0x004F) {
      PORTC = 0x03;
    }
    else if (sVal < 0x00ED && sVal >= 0x09E) {
      PORTC = 0x07;
    }
    else if (sVal < 0x0013C && sVal >= 0x00ED) {
      PORTC = 0x0F;
    }
    else if (sVal < 0x018B && sVal >= 0x013C) {
      PORTC = 0x01F;
    }
    else if (sVal < 0x01DA && sVal >= 0x018B) {
      PORTC = 0x03F;
    }
    else if (sVal < 0x0229 && sVal >= 0x01DA) {
      PORTC = 0x07F;
    }
    else if (sVal < 0x0278 && sVal >= 0x0229) {
      PORTC = 0xFF;
    }


  }
  return 0;
}
