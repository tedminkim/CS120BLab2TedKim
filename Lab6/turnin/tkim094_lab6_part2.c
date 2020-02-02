/*
* Author: Ted Kim
*  Email: tkim094@ucr.edu
*  Partner(s) Name: Kevin Chen
*  Partner's Email: kchen161@ucr.edu
*  Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
*  Assignment: Lab #6  Exercise #2
*  Exercise Description: Create a simple light game that requires pressing a button on PA0 while the middle of three LEDs on PB0, PB1, and PB2 is lit.

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
//unsigned char A0 = ~PINA & 0x01;
unsigned char where;

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

void TickLEDButton() {
  unsigned char A0 = ~PINA & 0x01;
  switch(state) {
    case Start:
      state = Led0;
      break;
    case Led0:
      if (A0) {
        state = Wait;
        break;
      }
      else {
        state = Led1;
      }
      break;
    case Led1:
      if (A0) {
        state = Wait;
      }
      else {
        state = Led2;
      }
      break;
    case Led2:
      if (A0) {
        state = Wait;
      }
      else {
        state = Led0;
      }
      break;
    case Wait:
      if (!A0) {
        switch(where) {
          case 1:
            state = Led0;
            break;
          case 2:
            state = Led1;
            break;
          case 3:
            state = Led2;
            break;
        }
        where = 0x00;
      }
      else {
        state = Wait;
      }
      break;
    default:
      state = Start;
      break;
  }
  switch(state) {
    case Start:
      break;
    case Led0:
      //where = 0x01;
      out = 0x01;
      PORTC = out;
      where = 0x01;
      //wait = 0;
      break;
    case Led1:
      out = 0x02;
      PORTC = out;
      where = 0x02;
      break;
    case Led2:
      out = 0x04;
      PORTC = out;
      where = 0x03;
      break;
    case Wait:
      out = out;
      PORTC = out;
      break;
    default:
      state = Start;
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
  TimerSet(50);
  TimerOn();
  //unsigned char tempValB = PORTB;
  state = Start;

  while(1) {
    TickLEDButton();
    while (!TimerFlag) {}
    TimerFlag = 0;
    PORTC = out;
  }
  return 0;
}
