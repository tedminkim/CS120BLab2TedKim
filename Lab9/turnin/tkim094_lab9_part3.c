/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #9  Exercise #3
*  Exercise Description:  Using the ATmega1284’s built in PWM functionality, design a system where a short, five-second melody, is played when a button is pressed



*  I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "simAVRHeader.h"
//#include "io.h"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}

void TimerOff() {
  TCCR1B = 0x00;
}

void TimerISR() {
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0) {
    TimerISR();
    _avr_timer_cntcurr = _avr_timer_M;
  }
}

void TimerSet(unsigned long M) {
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

unsigned char i = 0x00;
unsigned char signal = 0x00;
unsigned char buttonPress = PINA & 0x07;
const double notesArray[32] = {261.63, 0, 261.63, 0, 392.00, 0, 392.00, 0, 392.00, 0, 440.00, 0, 440.00,  0, 392.00, 392.00, 0, 261.63, 0, 349.23, 0, 329.63, 0, 329.63, 0, 293.66, 0, 293.66, 0, 261.63, 261.63};
enum States{Start, Init, PlayMelody} state;

void TickMelody() {
  switch(state) {
    case Start:
      state = Init;
      break;
    case Init:
      if (buttonPress == 0x01) {
        state = PlayMelody;
        break;
      }
      else {
        state = Init;
        break;
      }
    case PlayMelody:
      if (i < 32) {
        state = PlayMelody;
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
  switch(state) {
    case Start:
      break;
    case Init:
      set_PWM(0);
      i = 0;
      break;
    case Play:
      if (i < 32) {
        i = i + 1;
      }
      set_PWM(notesArray[i]);
      break;
  }
}


int main(void) {
  DDRA = 0x00;
  DDRB = 0xFF;
  //DDRC = 0xFF;
  //DDRD = 0xFF;
  PORTA = 0xFF;
  PORTB = 0x00
  //PORTC = 0x00;
  //PORTD = 0x00;
  // ADC_init();
  PWM_on();
  TimerSet(250);
  TimerOn();

  while(1) {
    //unsigned short sVal = ADC; //ADC value in variable
    //unsigned short cVal = (char)sVal; //cVal = 0xCD

    //store value in PORTC (cVal's value)
    //PORTC = cVal;
    //cVal = 0xAB
    //cVal = (char)(sVal >> 8);
    //PORTD = cVal;
    TickMelody();
    while (!TimerFlag) {}
    TimerFlag = 0;

  }
  return 0;
}
