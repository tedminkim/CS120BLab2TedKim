/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #9  Exercise #1
*  Exercise Description: Using the ATmega1284’s PWM functionality, design a system that uses three buttons to select one of three tones to be generated on the speaker.



*  I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
//#include "simAVRHeader.h"
//#include "io.h"
void set_PWM(double frequency) {
  static double current_frequency;
  if (frequency != current_frequency) {
    if (!frequency) { TCCR3B &= 0x08; }
    else { TCCR3B |= 0x03; }
    if (frequency < 0.954) { OCR3A = 0xFFFF; }
    else if (frequency > 31250) { OCR3A = 0x0000; }
    else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }
    TCNT3 = 0;
    current_frequency = frequency;
  }
}

void PWM_on() {
  TCCR3A = (1 << COM3A0);
  TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
  set_PWM(0);
}

void PWM_off() {
  TCCR3A = 0x00;
  TCCR3B = 0x00;
}

enum States{Start, Init, CSound, DSound, ESound} state;
unsigned char buttonPress = PINA & 0x07;

void MusicTick() {
  switch(state) {
    case Start:
      state = Init;
      break;
    case Init:
      if (buttonPress == 0x01) {
        state = CSound;
        break;
      }
      else if (buttonPress == 0x02) {
        state = DSound;
        break;
      }
      else if (buttonPress == 0x04) {
        state = ESound;
        break;
      }
      else {
        state = Init;
        break;
      }
    case CSound:
      if (buttonPress == 0x01) {
        state = CSound;
        break;
      }
      else if (buttonPress == 0x02) {
        state = DSound;
        break;
      }
      else if (buttonPress == 0x04) {
        state = ESound;
        break;
      }
      else {
        state = Init;
        break;
    }
    case DSound:
      if (buttonPress == 0x01) {
        state = CSound;
        break;
      }
      else if (buttonPress == 0x02) {
        state = DSound;
        break;
      }
      else if (buttonPress == 0x04) {
        state = ESound;
        break;
      }
      else {
        state = Init;
        break;
    }
    case ESound:
      if (buttonPress == 0x01) {
        state = CSound;
        break;
      }
      else if (buttonPress == 0x02) {
        state = DSound;
        break;
      }
      else if (buttonPress == 0x04) {
        state = ESound;
        break;
      }
      else {
        state = Init;
        break;
    }
    default:
      state = Start;
      break;
  }
  switch (state) {
    case Start:
      break;
    case Iniit:
      set_PWM(0);
      break;
    case CSound:
      set_PWM(261.63);
      break;
    case DSound:
      set_PWM(293.66);
      break;
    case ESound:
      set_PWM(329.63);
  }
}

int main(void) {
  DDRA = 0x00;
  DDRB = 0xFF;
  //DDRC = 0xFF;
  //DDRD = 0xFF;
  PORTA = 0xFF;
  PORTB = 0x00;
  //PORTC = 0x00;
  //PORTD = 0x00;
  //unsigned short maxVal = 0x0278;
  //unsigned short minVal = 0x000C;
  //ADC_init
  PWM_on();

  while(1) {
    MusicTick();


  }
  return 0;
}
