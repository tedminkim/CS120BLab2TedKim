/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #8  Exercise #3
*  Exercise Description: Design a system where an LED is illuminated only if enough light is detected from the photo resistor



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
  DDRD = 0xFF;
  PORTA = 0xFF;
  PORTC = 0x00;
  PORTD = 0x00;
  unsigned short maxVal = 0x0278;
  unsigned short minVal = 0x000C;
  ADC_init();

  while(1) {
    unsigned short sVal = ADC; //ADC value in variable
    if (sVal > (maxVal / 2)) {
      unsigned short cVal = (char)sVal; //cVal = 0xCD
      //store value in PORTC (cVal's value)
      PORTC = cVal;
      //cVal = 0xAB
      cVal = (char)(sVal >> 8);
      PORTD = cVal;
    }
    else {
      PORTC = 0x00;
      PORTD = 0x00;
    }


  }
  return 0;
}
