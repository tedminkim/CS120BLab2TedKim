/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #9  Exercise #2
*  Exercise Description: Using the ATmega1284’s PWM functionality, design a system where the notes: C4, D, E, F, G, A, B, and C5,  from the table at the top of the lab, can be generated on the speaker by scaling up or down the eight note scale.


*  I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include "simAVRHeader.h"
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

enum States{Start, Init, Press, On, scaleUp, scaleDown, upWait, downWait} state;
unsigned char buttonPress = PINA & 0x07;
unsigned char i = 0x00;
unsigned char signal = 0x00;
const double notesArray[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

void ScaleTick() {
  switch(state) {
    case Start:
      state = Init;
      break;
    case Init:
      signal = 0x00;
      if (buttonPress == 0x01) {
        state = Pressed;
        break;
      }
      else {
        state = Init;
        break;
      }
    case Pressed:
      if (buttonPress == 0x00) {
        if (signal == 0x00) {
          state = On;
          break;
        }
        else {
          state = Init;
          break;
        }
        break;
      }
      else {
        state = Pressed;
        break;
      }
    case On:
      signal = 0x01;
      if (buttonPress == 0x01) {
        state = Pressed;
        break;
      }
      else if (buttonPress == 0x02) {
        state = scaleUp;
        break;
      }
      else if (buttonPress == 0x04) {
        state = scaleDown;
        break;
      }
      else {
        state = On;
        break;
      }
      case scaleUp:
        if (buttonPress == 0x02) {
          state = scaleUp;
          break;
        }
        else if (buttonPress == 0x00) {
          state = upWait;
          break;
        }
        else {
          state = On;
          break;
        }
      case scaleDown:
        if (buttonPress == 0x04) {
          state = scaleDown;
          break;
        }
        else if (buttonPress == 0x00) {
          state = downWait;
          break;
        }
        else {
          state = On;
          break;
        }
      case upWait:
        state = On;
        break;
      case downWait:
        state = On;
        break;
      default:
        state = Start;
        break;
  }
  switch(state) {
    case Start:
      break;
    case Init:
      set_PWM(0);
      break;
    case Pressed:
      break;
    case On:
      set_PWM(notesArray[i]);
      break;
    case scaleUp:
      set_PWM(notesArray[i]);
      break;
    case scaleDown:
      set_PWM(notesArray[i]);
      break;
    case upWait:
      if (i < 7) {
        i = i + 1;
      }
      set_PWM(notesArray[i]);
      break;
    case downWait:
      if (i > 0) {
        i = i - 1;
      }
      set_PWM(notesArray[i]);
      break;
    default:
      break;
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
  //ADC_init();

  while(1) {
    ///unsigned short sVal = ADC; //ADC value in variable
    //unsigned short cVal = (char)sVal; //cVal = 0xCD

    //store value in PORTC (cVal's value)
    //PORTC = cVal;
    //cVal = 0xAB
    //cVal = (char)(sVal >> 8);
    //PORTD = cVal;
    ScaleTick();
  }
  return 0;
}
