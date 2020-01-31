/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #6  Exercise #3
*  Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTB is initially 7. Pressing PA0 increments PORTB once (stopping at 9). Pressing PA1 decrements PORTB once (stopping at 0).

*  I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "simAVRHeader.h"

volatile unsigned char TimerFlag = 0;
enum States{Start, Wait, Led0, Led1, Led2} state;
unsigned char out = 0x00;

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

vnum States{Start, Init, Incr, Wait1, Decr, Wait2, Reset, Wait3} state;
unsigned char countHold = 0x00;


void TickButtonCount() {
  unsigned char tempA0 = PINA & 0x01;
  unsigned char tempA1 = PINA & 0x02;
  switch (state) {
    case Start:
      state = Init;
      break;
    case Init:
      if (tempA0 && tempA1) {
        state = Wait3;
      }
      else if (tempA1) {
        state = Wait2;
      }
      else if (tempA0) {
        state = Wait1;
      }
      else {
        state = Init;
      }
      break;
    case Incr:
      state = Init;
      break;
    case Wait1:
      if (tempA0) {
        state = Incr;
      }
      else if (!tempA0) {
        state = Wait1;
      }
      break;
    case Decr:
      state = Init;
      break;
    case Wait2:
      if (tempA1) {
        state = Decr;
      }
      else if (!tempA1) {
        state = Wait2;
      }
      break;
    case Reset:
      state = Init;
      break;
    case Wait3:
      if ((tempA0) || (tempA1)) {
        state = Reset;
      }
      else if ((!tempA0) || (tempA1)) {
        state = Wait3;
      }
      break;
    default:
      break;
  }
  switch (state) {
    case Start:
      //countHold = 7;
      break;
    case Init:
      break;
    case Reset:
      countHold = 0;
      break;
    case Incr:
      if (countHold < 9) {
        countHold = countHold + 1;
      }
      break;
    case Wait1:
      break;
    case Wait2:
      break;
    case Decr:
      if (countHold > 0) {
        countHold = countHold - 1;
      }
      break;
    case Wait3:
      break;
    default:
      break;
  }
}

int main(void) {
  DDRA = 0x00;
  DDRC = 0xFF;

  PORTA = 0xFF;
  PORTC = 0x00;

  //unsigned char tempValA = 0x00;
  //unsigned char tempValC = 0x00;
  TimerSet(300);
  TimerOn();
  countHold = 7;
  //unsigned char tempValB = PORTB;
  state = Start;

  while(1) {
    TickLED();
    while (!TimerFlag) {}
    TimerFlag = 0;
    //tempValB = out;
    PORTC = countHold;
  }
  return 0;
}
