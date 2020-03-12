/*	Author: Ted Kim
 *  Email: tkim094@ucr.edu
 *  Partner(s) Name: None
 *	Lab Section: 023 (Tuesdays & Thursdays 2-3:20 PM)
 *	Assignment: Custom Laboratory Project
 *	Exercise Description: iBreadBoard
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "nokia5110.h"
#endif
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>

 typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR2B = 0x0B;
	OCR2A = 125;
	TIMSK2 = 0x02;
	TCNT2 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR2B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}
ISR(TIMER2_COMPA_vect) {
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

void set_PWM(double frequency) { //PB6
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) {OCR3A = 0x0000; }
		else {OCR3A = (short)(8000000 / (128 * frequency)) + 0; }

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
void set_PWMB(double frequency) { // PD5
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR1B &= 0x08; }
		else { TCCR1B |= 0x03; }

		if (frequency < 0.954) { OCR1A = 0xFFFF; }
		else if (frequency > 31250) {OCR1A = 0x0000; }
		else {OCR1A = (short)(8000000 / (128 * frequency)) + 0; }

		TCNT1 = 0;
		current_frequency = frequency;
	}
}
void PWMB_on() {
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
	set_PWMB(0);
}
void PWMB_off() {
	TCCR1A = 0x00;
	TCCR1B = 0x00;
}
void play(short freqSound) {
	//buzzerSound = 0; //dud
	set_PWM(freqSound);
}
void silence() {
	//buzzerSound = 0; //dud
	set_PWM(0);
}
void playH(short freqSound) {
	//buzzerSound = 0; //dud
	set_PWM0(freqSound);
}
void silenceH() {
	//buzzerSound = 0; //dud
	set_PWM0(0);
}
void playB(short freqSound) {
  //buzzerSound = 0;
  set_PWMB(freqSound);
}
void silenceB() {
  //buzzerSound = 0;
  set_PWMB(0);
}


//MUSIC NOTES

#define C 130.8 * 2
#define CS 138.6 * 2 //also known as D FLAT
#define D 146.8 * 2
#define DS 155.6 * 2 //ALSO KNOWN AS E FLAT
#define E 164.8 * 2
#define F 174.6 * 2
#define FS  185 * 2//ALSO KNOWN AS G FLAT
#define G 196 * 2
#define GS 207.7 * 2//ALSO KNOWN AS A FLAT
#define A 220 * 2
#define AS 233.1 * 2//ALSO KNOWN AS B FLAT
#define B 246.9 * 2



short testSong3[] = {C, B/2, A/2, B/2, C, C, C, B/2, B/2, B/2, C, E, E, C, B/2, A/2, B/2, C, C, C, C, B/2, B/2, C, A/2, G/2};
//short testSongH[] = {F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};
short testSongNoteLengths3[] = {300, 300, 300, 300, 300, 300, 600, 300, 300, 600, 300, 300, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 1000};
short testSongRestLengths3[] = {25, 25, 25, 25, 50, 50, 150, 50, 50, 150, 50, 50, 150, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 150};
//30 notes
//short renCircM[] = {CS*2,    B,   CS*2,   CS*2, B,   E*2,   E*2,      CS*2,   B,   CS*2,  CS*2,   CS*2, B,    E*2,   GS*2,      CS*2,    B,    CS*2,    CS*2,    CS*2,   B,    E*2,   E*2,   E*2,   FS*2,    E*2,    E*2,   E*2,   CS*2,   GS*2, E,  E,   FS,    G,    GS,    E,     B,    CS,     E,    E,    FS,      G,     GS,     E,    CS,    E,      CS,      E,     CS,     E,   CS,     E,   E,    A,    GS,   E,    E,    CS,     GS,  E,  E,   FS,    G,    GS,    E,     B,    CS,     E,    E,    FS,      G,     GS,     E,    CS,    E,      CS,      E,     CS,     E,   CS,     E,   E,    A,    GS,   E,    E,    FS,     E }; //30 notes
//short renCircMNL[] = {75,    150, 300,    300,  150,  300,    150,     75,   150,    300,    150,   75,  150, 300, 150,        75,     150,   300,      150,    75,     150,  300,    150,    75,     150,   300,    150,     75,   150,  150, 200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150, 200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150 };
//short renCircMRL[] = {10,     25,  100,   100,  100,  100,    250,     10,      25,   100,   100,   10,   100, 100,  250,      10,   25,       100,     100,       10,   100,   100,  100,    10,    100,    100,   100,     10,   100,   600, 100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   600, 100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   300          };
//30 notes
short renCircM2[] = {  E,  E,   FS,    G,    GS,    E,     B,    CS,     E,    E,    FS,      G,     GS,     E,    CS,    E,      CS,      E,     CS,     E,   CS,     E,   E,    A,    GS,   E,    E,    CS,     GS};
short renCircM2NL[] = {200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150};
short renCircM2RL[] = {100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   300     };



short renCircB2[] = {  GS,  GS,   A,    AS,    B,    GS,     E,    FS,     GS,    GS,    A,      AS,     B,     GS,    E,    GS,      E,      GS,     E,     GS,   E,     GS,   GS,    CS*2,    B,   GS,    GS,    E,     B};
short renCircB2NL[] = {200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150};
short renCircB2RL[] = {100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   300     };

short renCircB3[] = {  GS,  GS,   A,    AS,    B,    GS,     E,    FS,     GS,    GS,    A,      AS,     B,     GS,    E,    GS,      E,      GS,     E,     GS,   E,     GS,   GS,    CS*2,    B,   GS,    GS,    A,     GS};
short renCircB3NL[] = {200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150};
short renCircB3RL[] = {100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   300     };


/*short MaryHM[] = {E, D, C, D, E, E, E, D, D, D, E, G, G, E, D, C, D, E, E, E, E, D, D, E, D, C};
short MaryHMN[] = {300, 300, 300, 300, 300, 300, 600, 300, 300, 600, 300, 300, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 1000};
short MaryHMR[] = {25, 25, 25, 25, 50, 50, 150, 50, 50, 150, 50, 50, 150, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 150};

short MaryHB[] = {G, F, E, F, G, G, G, F, F, F, G, C*2, C*2, G, F, E, F, G, G, G, G, F, F, G, F, E};
//short testSongH[] = {F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};
short MaryHBN[] = {300, 300, 300, 300, 300, 300, 600, 300, 300, 600, 300, 300, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 1000};
short MaryHBR[] = {25, 25, 25, 25, 50, 50, 150, 50, 50, 150, 50, 50, 150, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 150};*/
	
short AmGrM[] = { F,    AS,     D*2,     C*2,     AS,     D*2,     C*2,     AS,     G,     F,    F,     AS,       D*2,     C*2,    AS,      D*2,     C*2,      F*2,      F*2,        D*2,     F*2,     F*2,     D*2,     C*2,      AS,       D*2,     C*2,     AS,    G,     F,    F,      AS,    D*2,    C*2,    AS,   D*2,    C*2,    AS};
short AmGrMN[] = {500,  600,    300,      75,     75,     700,     400,     600,    350,   600,  500,   600,      300,      75,    75,      700,     300,     300,       800,        300,     300,      600,     300,      75,       75,      700,     500,     500,   300,   700,  500,    600,   300,    75,     75,   700,    550,    2000};
short AmGrMR[] = {200,  300,    75,      5,     10,     200,     200,     300,    200,   300,  200,   300,      75,      5,    10,      200,     50,     10,       300,        50,     10,      200,     75,      5,       10,      200,     200,     300,   100,   400,  200,    300,   75,    5,     10,   300,    200,    500};	
	
short AmGrB[] = { AS,    D*2,     F*2,     DS*2,     D*2,     F*2,     DS*2,     D*2,     C*2,     AS,    AS,     D*2,       F*2,     DS*2,    D*2,      F*2,     DS*2,      A*2,      A*2,        F*2,     AS*2,     AS*2,     F*2,     DS*2,      D*2,       F*2,     DS*2,     D*2,    C*2,     AS,    AS,      D*2,    F*2,    DS*2,    D*2,   F*2,    DS*2,    D*2};
short AmGrBN[] = {500,  600,    300,      75,     75,     700,     400,     600,    350,   600,  500,   600,      300,      75,    75,      700,     300,     300,       800,        300,     300,      600,     300,      75,       75,      700,     500,     500,   300,   700,  500,    600,   300,    75,     75,   700,    550,  2000};
short AmGrBR[] = {200,  300,    75,      5,     10,     200,     200,     300,    200,   300,  200,   300,      75,      5,    10,      200,     50,     10,       300,        50,     10,      200,     75,      5,       10,      200,     200,     300,   100,   400,  200,    300,   75,    5,     10,   300,    200,    500};
	
short renCircM[] = {CS*2,    B,   CS*2,   CS*2, B,   E*2,   E*2,      CS*2,   B,   CS*2,  CS*2,   CS*2, B,    E*2,   GS*2,      CS*2,    B,    CS*2,    CS*2,    CS*2,   B,    E*2,   E*2,   E*2,   FS*2,    E*2,    E*2,   E*2,   CS*2,   GS*2, E,  E,   FS,    G,    GS,    E,     B,    CS,     E,    E,    FS,      G,     GS,     E,    CS,    E,      CS,      E,     CS,     E,   CS,     E,   E,    A,    GS,   E,    E,    CS,     GS,  E,  E,   FS,    G,    GS,    E,     B,    CS,     E,    E,    FS,      G,     GS,     E,    CS,    E,      CS,      E,     CS,     E,   CS,     E,   E,    A,    GS,   E,    E,    FS,     E }; //30 notes
short renCircMNL[] = {75,    150, 300,    300,  150,  300,    150,     75,   150,    300,    150,   75,  150, 300, 150,        75,     150,   300,      150,    75,     150,  300,    150,    75,     150,   300,    150,     75,   150,  150, 200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150, 200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150 };
short renCircMRL[] = {10,     25,  100,   100,  100,  100,    250,     10,      25,   100,   100,   10,   100, 100,  250,      10,   25,       100,     100,       10,   100,   100,  100,    10,    100,    100,   100,     10,   100,   600, 100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   600, 100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   300};
	
short renCircB[] = {  A,   B,    B,  GS, CS, CS,   FS,     B,  B,  E, CS,  GS,  GS,   A,    AS,    B,    GS,     E,    FS,     GS,    GS,    A,      AS,     B,     GS,    E,    GS,      E,      GS,     E,     GS,   E,     GS,   GS,    CS*2,    B,   GS,    GS,    E,     B,  GS,  GS,   A,    AS,    B,    GS,     E,    FS,     GS,    GS,    A,      AS,     B,     GS,    E,    GS,      E,      GS,     E,     GS,   E,     GS,   GS,    CS*2,    B,   GS,    GS,    A,     GS };
short renCircBNL[] = {975,300, 150, 975, 300, 150, 775, 250, 300, 875, 150, 200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150, 200,200, 75,    150,  300,   150,   100,   100,  300, 200,    75,     150,    300,   150,   250,   150,    75,      250,   250,     150,  75,   250, 150,   75,  150,  200,   75,  150,     150};
short renCircBRL[] = {335, 100, 475, 160, 25, 450, 285, 50, 350, 200, 545, 100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150, 100,   10,  100,  100 ,   10,    100,   600, 100, 100, 10,    100,  100,  200,   25,   25,   100,  100,   10,     100,    100,   300,   25,  100,      50,       150,  25,    100,     50,   150,  100,   10,  100,  100 ,   10,    100,   300      };

//60 notes
short mooHLM[] = {G,    A,      E*2,     G*2,    E*2,    E*2,    D*2,    C*2,    D*2,    E*2,    B*2,    G*2,    FS*2,    F*2,      C*4,       D*4,        C*4,      AS*2,      C*4,      AS*2,      GS*2, AS*2, GS*2, G*2, G,   D, E, F, G, C*2, A,    G,   G,    A,      E*2,     G*2,    E*2,    E*2,    D*2,    C*2,    D*2,    E*2,    B*2,    G*2,    FS*2,    F*2,      C*4,       D*4,        C*4,      AS*2,      C*4,      AS*2,      GS*2, AS*2, GS*2, G*2, G,   D, E, F, G, C*2, A,    G, E,   E,   C};
short mooHLMN[] = { 200,  200,    600,   600,    600,  200,  75,   200,  75,   600,  600,   600,    75,                   200,      100,       75,         300,      100,        75,       300,     100,  75,   300,  400, 500, 75,75,75,75,300, 75,    400, 200,  200,    600,   600,    600,  200,  75,   200,  75,   600,  600,   600,    75,                   200,      100,       75,         300,      100,        75,       300,     100,  75,   300,  400, 500, 75,75,75,75,300, 75,    400, 400, 400, 1000 };     
short mooHLMR[] = { 200,  200,    200,   200,    200,  100,  10,   100,  10,   300,  200,   200,    10,                   200,       100,       10,         200,      100,        10,       200,     100,  10,   200,  200, 300, 10,10,10,100,100, 10,  500, 200,  200,    200,   200,    200,  100,  10,   100,  10,   300,  200,   200,    10,                   200,       100,       10,         200,      100,        10,       200,     100,  10,   200,  200, 300, 10,10,10,100,100, 10,  800, 300, 300, 500};

short mooHLB[] = {E,    E,      G,     B,    G,    G,    F,    E,    F,    G,    D*4,    B*2,    AS*2,    A*2,      C*4,       D*4,        C*4,      AS*2,      C*4,      AS*2,      GS*2, AS*2, GS*2, G*2, G,   F, G, A, C*2, E*2, A,    G,   E,    F,      G,     B,    G,    G,    F,    E,    F,    G,    D*4,    B*2,    AS*2,    A*2,      C*4,       D*4,        C*4,      AS*2,      C*4,      AS*2,      GS*2, AS*2, GS*2, G*2, G,   F, G, A, C*2, E*2, A,    G, GS,   GS,   FS};
short mooHLBN[] = { 200,  200,    600,   600,    600,  200,  75,   200,  75,   600,  600,   600,    75,                   200,      100,       75,         300,      100,        75,       300,     100,  75,   300,  400, 500, 75,75,75,75,300, 75,    400, 200,  200,    600,   600,    600,  200,  75,   200,  75,   600,  600,   600,    75,                   200,      100,       75,         300,      100,        75,       300,     100,  75,   300,  400, 500, 75,75,75,75,300, 75,    400, 400, 400, 1000 };
short mooHLBR[] = { 200,  200,    200,   200,    200,  100,  10,   100,  10,   300,  200,   200,    10,                   200,       100,       10,         200,      100,        10,       200,     100,  10,   200,  200, 300, 10,10,10,100,100, 10,  500, 200,  200,    200,   200,    200,  100,  10,   100,  10,   300,  200,   200,    10,                   200,       100,       10,         200,      100,        10,       200,     100,  10,   200,  200, 300, 10,10,10,100,100, 10,  800, 300, 300, 500};

short* renCircMel[] = {renCircM};
short* renCircMelNL[] = {renCircMNL};
short* renCircMelRL[] = {renCircMRL};
short* AmGrMel[] = {AmGrM};
short* AmGrMelN[] = {AmGrMN};
short* AmGrMelR[] = {AmGrMR};
short* AmGrBas[] = {AmGrB};
short* AmGrBasN[] = {AmGrBN};
short* AmGrBasR[] = {AmGrBR};
short* mooHLMel[] = {mooHLM};
short* mooHLMelN[] = {mooHLMN};
short* mooHLMelR[] = {mooHLMR};
short* mooHLBas[] = {mooHLB};
short* mooHLBasN[] = {mooHLBN};
short* mooHLBasR[] = {mooHLBR};
short* renCircHar[] = {renCircB};
short* renCircHarNL[] = {renCircBNL};
short* renCircHarRL[] = {renCircBRL};
/*short* MaryHMel[] = {MaryHM};
short* MaryHMelN[] = {MaryHMN};
short* MaryHMelR[] = {MaryHMR};
short* MaryHBas[] = {MaryHB};
short* MaryHBasN[] = {MaryHBN};
short* MaryHBasR[] = {MaryHBR};*/
short** playlistMel[] = {renCircMel, AmGrMel, mooHLMel};
short** playlistMelNL[] = {renCircMelNL, AmGrMelN, mooHLMelN};
short** playlistMelRL[] = {renCircMelRL, AmGrMelR, mooHLMelR};
short** playlistBas[] = {renCircHar, AmGrBas, mooHLBas};
short** playlistBasNL[] = {renCircHarNL, AmGrBasN, mooHLBasN};
short** playlistBasRL[] = {renCircHarRL, AmGrBasR, mooHLBasR};
char numRows[] = {1, 1, 1};

void DisplaySong1() {
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	nokia_lcd_write_string("Song Name:    Renai         Circulation", 1);
	nokia_lcd_render();
}
/*void DisplaySong2() {
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	nokia_lcd_write_string("Song Name:    Mary Had A    Little Lamb", 1);
	nokia_lcd_render();
}*/
void DisplaySong2() {
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	nokia_lcd_write_string("Song Name:    Amazing Grace", 1);
	nokia_lcd_render();
}
void DisplaySong3() {
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	nokia_lcd_write_string("Song Name:    Mamamoo :     Hello", 1);
	nokia_lcd_render();
}
void DisplayPaused() {
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 0);
	nokia_lcd_write_string("Song Paused!  Press Play to Resume.", 1);
	nokia_lcd_render();
}
void PressPlay() {
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0,0);
	nokia_lcd_write_string("Welcome to the iBreadBoard! Press Play to Begin Your     Music!", 1);
	nokia_lcd_render();
}

unsigned char paused = 1;
//unsigned char paused = 1;
enum Pauses{Pinit, Pp, Pr};
int pauseTick(int state) {
	unsigned char playPause = PINA & 0x01;
	switch(state) {
		case Pinit:
		paused = 1;
		state = Pp;
		break;
		case Pp:
		if (playPause) {
			state = Pr;
			paused = !paused;
		}
		else {
			state = Pp;
		}
		break;
		case Pr:
		if (playPause) {
			//DisplayPaused();
			state = Pr;
		}
		else {
			state = Pp;
		}
		break;
		default:
		state = Pinit;
		break;
	}
	return state;
}

unsigned char prevSong;
unsigned char nextSong;

enum nextPrevStates{Pw, backSong, forSong};
int NextPrevTick(int state) {
	//unsigned char NextPrevButton = 0;
	unsigned char nextButton = PINA & 0x02;
	unsigned char prevButton = PINA & 0x04;
	switch(state) {
		case Pw:
		if (prevButton) {
			prevSong = 1;
			nextSong = 0;
			state = backSong;
		}
		else if (nextButton) {
			nextSong = 1;
			prevSong = 0;
			state = forSong;
		}
		else {
			prevSong = 0;
			nextSong = 0;
			state = Pw;
		}
		break;
		case backSong:
		if (prevButton) {
			state = backSong;
		}
		else {
			state = Pw;
		}
		break;

		case forSong:
		if (nextButton) {
			state = forSong;
		}
		else {
			state = Pw;
		}
		break;
		default:
		state = Pw;
		break;
	}
	return state;
}

unsigned char songDone = 1;
unsigned long x = sizeof(playlistMel);
unsigned long y = sizeof(playlistMel[0]);
unsigned long playList = sizeof(playlistMel) / sizeof(playlistMel[0]);

unsigned char it4;
enum MelodyStates{Minit, Mwait, Mplay, Ml, Mr, Mp};
int melodyTick(int state) {
	static unsigned short it1;
	static unsigned short it2;
	static unsigned short it3;
	//static unsigned char it4;
	static unsigned char placeHolder;
	switch(state) {
		case Minit:
			state= Mwait;
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it4 = 1;
			prevSong = 0;
			nextSong = 0;
			silence();
			break;

		case Mwait:
			if (!paused) {
				state= Mplay;
				//DisplayName(it4);
			}
			else {
				state= Mwait;
			}
			break;

		case Mplay:
			songDone = 0;
			if (paused) {
				silence();
				DisplayPaused();
				placeHolder = state;
				state = Mp;
				break;
			}
			//else {
				//DisplayName(it4);
			//}
			if (prevSong && (it1 == 0)) {
				it1 = 0;
				it2 = 0;
				it3 = 0;
				if (it4 == 0) {
					it4 = playList - 1;
					
					//if (it4 == 2) {
						//DisplaySong3();
					//}
					//else if (it4 == 1) {
						//DisplaySong2();
					//}
					//else if (it4 == 0) {
						//DisplaySong1();
					//}
				}
				else {
					it4 = it4 - 1;
				}
				state = Mplay;
				break;
			}
			else if (prevSong){
				it1 = 0;
				it2 = 0;
				it3 = 0;
				state = Mplay;
				break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it4 = it4 + 1;
			if (it4 == playList) {
				it4 = 0;
			}
			state = Mplay;
			break;
		}
		if (playlistMel[it4][it1][it2] == -1) {
			it1 = it1 + 1;
			it2 = 0;
		}
		if (it1 == numRows[it4]) {
			songDone = 1;
			state = Mwait;
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it4 = it4 + 1;
			if (it4 == playList) {
				it4 = 0;
			}
			break;
		}
		/*if (it4 == 0) {
			DisplaySong1();
		}
		else if (it4 == 1) {
			DisplaySong2();
		}
		else if (it4 == 2) {
			DisplaySong3();
		}*/
		//play(playlistMel[it4][it1][it2]);
		//play(playlistMel[it4][it1][it2]);
		//DisplayName(it4);
		if (it4 == 0) {
			play(playlistMel[it4][it1][it2]);
			DisplaySong1();
			
			state = Ml;
			//state = Ms0;
		}
		else if (it4 == 1) {
			play(playlistMel[it4][it1][it2]);
			DisplaySong2();
			
			state = Ml;
			//state = Ms1;
		}
		else if (it4 == 2) {
			play(playlistMel[it4][it1][it2]);
			DisplaySong3();
			
			state = Ml;
			//state = Ms2;
		}
		//play(playlistMel[it4][it1][it2]);
		//state = Ml;
		
		break;

		//case Ms0:
					

		case Ml:
		if (paused) {
			silence();
			DisplayPaused();
			placeHolder = state;
			state = Mp;
			break;
		}
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it4 == 0) {
				it4 = playList - 1;
				}
			else {
				it4 = it4 - 1;
			}
			state = Mplay;
			break;
    }
    else if (prevSong){
			it1 = 0;
			it2 = 0;
			it3 = 0;
			state = Mplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it4 = it4 + 1;
			if (it4 == playList) {
				it4 = 0;
			}
			state = Mplay;
			break;
		}
		if (it3 < playlistMelNL[it4][it1][it2]) {
			it3 = it3 + 1;
			state = Ml;
			}
			else {
				it3 = 0;
				silence();
				state = Mr;
		}
		/*if (it4 == 0) {
			DisplaySong1();
		}
		else if (it4 == 1) {
			DisplaySong2();
		}
		else if (it4 == 2) {
			DisplaySong3();
		}*/
		break;

		case Mr:
		if (paused) {
			DisplayPaused();
			silence();
			placeHolder = state;
			state = Mp;
			break;
		}
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it4 == 0) {
				it4 = playList - 1;
				}
      else {
				it4 = it4 - 1;
			}
			state = Mplay;
			break;
    }
    else if (prevSong){
			it1 = 0;
			 it2 = 0;
			  it3 = 0;
			state = Mplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			 it2 = 0;
			 it3 = 0;
			it4 = it4 + 1;
			if (it4 == playList) {
				it4 = 0;
			}
			state = Mplay;
			break;
		}
		if (it3 < playlistMelRL[it4][it1][it2]) {
			it3 = it3 + 1;
			state = Mr;
			}
    else {
			it3 = 0;
			it2 = it2 + 1;
			state = Mplay;
		}
		break;

		case Mp:
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it4 == 0) {
				it4 = playList - 1;
				}
      else {
				it4 = it4 - 1;
			}
			paused = 0;
			state = Mplay;
			break;
    } else if (prevSong){
			it1 = 0;
			it2 = 0;
			it3 = 0;
			paused = 0;
			state = Mplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it4 = it4 + 1;
			if (it4 == playList) {
				it4 = 0;
			}
			paused = 0;
			state = Mplay;
			break;
		}
		if (paused) {
			DisplayPaused();
			state = Mp;
			}
    else {
			if (placeHolder == Ml) {
				/*if (it4 == 0) {
					DisplaySong1();
				}
				else if (it4 == 1) {
					DisplaySong2();
				}
				else if (it4 == 2) {
					DisplaySong3();
				}*/
				
				if (it4 == 0) {
					play(playlistMel[it4][it1][it2]);
					//DisplaySong1();
					
				}
				else if (it4 == 1) {
					play(playlistMel[it4][it1][it2]);
					//DisplaySong2();
					
				}
				else if (it4 == 2) {
					play(playlistMel[it4][it1][it2]);
					//DisplaySong3();
				}
				//play(playlistMel[it4][it1][it2]);
					
				}
			//DisplayName(it4);
			state = placeHolder;
		}
		break;
	}
	//DisplayName(it4);
	return state;
}
unsigned char it42;
enum BassStates{Binit, Bwait, Bplay, Bl, Br, Bp};
int bassTick(int state) {
	static unsigned short it1;
	static unsigned short it2;
	static unsigned short it3;
	//static unsigned char it42 = 0;
	static unsigned char placeHolder;
	switch(state) {
		case Binit:
		state= Bwait;
		it1 = 0;
		it2 = 0;
		it3 = 0;
		it42 = 1;
		prevSong = 0;
		nextSong = 0;
		silenceB();
		break;

		case Bwait:
		if (!paused) {
			state= Bplay;
			//DisplayName(it42);
		}
		else {
			state= Bwait;
		}
		break;

		case Bplay:
		songDone = 0;
		if (paused) {
			silenceB();
			DisplayPaused();
			placeHolder = state;
			state = Bp;
			break;
		}
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it42 == 0) {
				it42 = playList - 1;
			}
			else {
				it42 = it42 - 1;
			}
			state = Bplay;
			break;
		}
		else if (prevSong){
			it1 = 0;
			it2 = 0;
			it3 = 0;
			state = Bplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it42 = it42 + 1;;
			if (it42 == playList) {
				it42 = 0;
			}
			state = Bplay;
			break;
		}
		if (playlistBas[it42][it1][it2] == -1) {
			it1 = it1 + 1;
			it2 = 0;
		}
		if (it1 == numRows[it42]) {
			songDone = 1;
			state = Bwait;
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it42 = it42 + 1;
			if (it42 == playList) {
				it42 = 0;
			}
			break;
		}
		//playB(playlistBas[it42][it1][it2]);
		//playB(playlistBas[it4][it1][it2]);
		//DisplayName(it42);
		if (it42 == 0) {
			playB(playlistBas[it42][it1][it2]);
			DisplaySong1();
			state = Bl;
		}
		else if (it42 == 1) {
			playB(playlistBas[it42][it1][it2]);
			DisplaySong2();
			state = Bl;
		}
		else if (it42 == 2) {
			playB(playlistBas[it42][it1][it2]);
			DisplaySong3();
			state = Bl;
		}
		//
		//playB(playlistBas[it42][it1][it2]);
		//state = Bl;
		break;

		case Bl:
		if (paused) {
			silenceB();
			DisplayPaused();
			placeHolder = state;
			state = Bp;
			break;
		}
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it42 == 0) {
				it42 = playList - 1;
			}
			else {
				it42 = it42 - 1;
			}
			state = Bplay;
			break;
			}
    else if (prevSong){
			it1 = 0;
			it2 = 0;
			it3 = 0;
			state = Bplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it42 = it42 + 1;
			if (it42 == playList) {
				it42 = 0;
			}
			state = Bplay;
			break;
		}
		if (it3 < playlistBasNL[it42][it1][it2]) {
			it3 = it3 + 1;
			state = Bl;
		}
		else {
			it3 = 0;
			silenceB();
			state = Br;
		}
		/*if (it42 == 0) {
			DisplaySong1();
		}
		else if (it42 == 1) {
			DisplaySong2();
		}
		else if (it42 == 2) {
			DisplaySong3();
		}*/
		//DisplayName(it42);
		break;

		case Br:
		if (paused) {
			silenceB();
			DisplayPaused();
			placeHolder = state;
			state = Bp;
			break;
		}
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it42 == 0) {
				it42 = playList - 1;
				}
      else {
				it42 = it42 - 1;
			}
			state = Bplay;
			break;
			}
      else if (prevSong){
			it1 = 0;
			it2 = 0;
			it3 = 0;
			state = Bplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it42 = it42 + 1;
			if (it42 == playList) {
				it42 = 0;
			}
			state = Bplay;
			break;
		}
		if (it3 < playlistBasRL[it42][it1][it2]) {
			it3 = it3 + 1;
			state = Br;
			}
    else {
			it3 = 0;
			it2 = it2 + 1;
			state = Bplay;
		}
		break;

		case Bp:
		if (prevSong && (it1 == 0)) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			if (it42 == 0) {
				it42 = playList - 1;
				}
      else {
				it42 = it42 - 1;
			}
			paused = 0;
			state = Bplay;
			break;
			} else if (prevSong){
			it1 = 0;
			it2 = 0;
			it3 = 0;
			paused = 0;
			state = Bplay;
			break;
		}
		if (nextSong) {
			it1 = 0;
			it2 = 0;
			it3 = 0;
			it42 = it42 + 1;
			if (it42 == playList) {
				it42 = 0;
			}
			paused = 0;
			state = Bplay;
			break;
		}
		if (paused) {
			
			state = Bp;
			DisplayPaused();
			}
    else {
			if (placeHolder == Bl) {
				/*if (it4 == 0) {
					DisplaySong1();
				}
				else if (it4 == 1) {
					DisplaySong2();
				}
				else if (it4 == 2) {
					DisplaySong3();
				}*/
				//playB(playlistBas[it4][it1][it2]);
				if (it42 == 0) {
					playB(playlistBas[it42][it1][it2]);
					//DisplaySong1();
					
				}
				else if (it42 == 1) {
					playB(playlistBas[it42][it1][it2]);
					//DisplaySong2();
					
				}
				else if (it42 == 2) {
					playB(playlistBas[it42][it1][it2]);
					//DisplaySong3();
					//playB(playlistBas[it4][it1][it2]);
				}
					
			}
			//DisplayName(it42);
			state = placeHolder;
		}
		break;
	}
	//DisplayName(it42);
	return state;
}
/*enum NameStates{Ninit, Ren, Mar, Mam};
int DisplayName(int state) {
	unsigned char boop = it4;
	switch(state) {
		case Ninit:
		//case 0:
		if (boop == 0) {
			state = Ren;
			//break;
		}
		//case 1:
		else if (boop == 1) {
			state = Mar;
			//break;
		}
		//case 2:
		else if (boop == 2) {
			state = Mam;
			//break;
		}
		break;
		case Ren:
		if (boop != 0) {
			state  = Ninit;
		}
		else {
			state = Ren;
		}
		break;
		case Mar:
		if (boop != 1) {
			state = Ninit;
		}
		else {
			state = Mar;
		}
		break;
		case Mam:
		if (boop != 2) {
			state = Ninit;
		}
		else {
			state = Mam;
		}
		break;
	}
		switch(state) {
			case Ninit:
			break;
		case Ren:
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);
			nokia_lcd_write_string("Song Name:    Renai         Circulation", 1);
			nokia_lcd_render();
			break;
		case Mar:
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);
			nokia_lcd_write_string("Song Name:    Mary Had A    Little Lamb", 1);
			nokia_lcd_render();
			break;
		case Mam:
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);
			nokia_lcd_write_string("Song Name:    Mamamoo :     Hello", 1);
			nokia_lcd_render();
			break;
		}
			
	return state;
};*/








int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xF0;
	PORTD = 0x00;


  unsigned char timerPeriod = 1;
  static task  task2, task3, task4, task5;
  task *tasks[] = {  &task2, &task3, &task4, &task5};
  const unsigned short taskNum = sizeof(tasks) / sizeof(*tasks);

  //task1.state = 0;
  //task1.period = 50;
  //task1.elapsedTime = task1.period;
  //task1.TickFct = &DisplayName;
  task2.state = 0;
  task2.period = 1;
  task2.elapsedTime = task2.period;
  task2.TickFct = &pauseTick;
  task3.state = 0;
  task3.period = 1;
  task3.elapsedTime = task3.period;
  task3.TickFct = &NextPrevTick;
  task4.state = 0;
  task4.period = 1;
  task4.elapsedTime = task4.period;
  task4.TickFct = &melodyTick;
  task5.state = 0;
  task5.period = 1;
  task5.elapsedTime = task5.period;
  task5.TickFct = &bassTick;
	TimerSet(timerPeriod);
	TimerOn();

	nokia_lcd_init();
	PressPlay();
	//if (paused) {
		//DisplayPaused();
	//}
	//else {
	/*	if (it4 == 0 && it42 == 0) {
			DisplaySong1();
		}
		else if (it4 == 1 && it42 == 1) {
			DisplaySong2();
		}
		else if (it4 == 2 && it42 == 2) {
			DisplaySong3();
		}*/
	//}
	
	//nokia_lcd_clear();
	//nokia_lcd_set_cursor(0, 0);
	//nokia_lcd_write_string("JESUS LOVES     YOU         JOHN 3:16", 1);
	//nokia_lcd_render();
	PWM_on();
	//PWM0_on();
	PWMB_on();

    while (1) {
			for (unsigned char i = 0; i < taskNum; i++) {
				if ( tasks[i]->elapsedTime == tasks[i]->period) {
					tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
					tasks[i]->elapsedTime = 0;
				}
				tasks[i]->elapsedTime += timerPeriod;
			}

		while(!TimerFlag){};
			TimerFlag = 0;
		}
    return 1;
}
