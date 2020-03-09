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

 typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;
enum MelodyStates{Init, WaitPlay, Melody, MNL, MR, MP};
int melodyTick(int MState) {
	unsigned short i = 0;
	unsigned short j = 0;
	unsigned short k = 0;
	unsigned placeHolder = 0;
	unsigned char keys = sizeof(melodyTone) / sizeof(melodyTone[0]);
	switch(MState) {
		case Init:
			MState = WaitPlay;
			i = 0;
			j = 0;
			k = 0;
			silence();
			break;
		case WaitPlay:
			if (!songPaused) {
				MState = Melody;
				songFinished = 0;
			}
			else {
				MState = WaitPlay;
			}
			break;
		case Melody:
			if (songPaused) {
				silence();
				placeHolder = MState;
				MState = MP;
			}
			if (melodyTone[i][j] == -1) {
				i = i + 1;
				j = 0;
			}
			if (i == keys) {
				songFinished = 1;
				songPaused = 1;
				MState = Init;
				i = 0;
				break;
			}
			play(melodyTone[i][j]);
			MState = MNL;
			break;
		case MR:
			if (songPaused) {
				silence();
				placeHolder = MState;
				MState = MP;
				break;
			}
			if (k < melodyToneR[i][j]) {
				k = k + 1;
				MState = MR;
			}
			else {
				k = 0;
				j = j + 1;
				MState = Melody;
			}
			break;
		case MP:
			if (songPaused) {
				MState = MP;
			}
			else {
				if (placeHolder == MNL) {
					play(melodyTone[i][j]);
				}
				MState = placeHolder;
			}
			break;
	}
	return MState;
}
enum HarmonyStates{Init2, WaitPlay2, Harmony, HNL, HR, HP};
int harmonyTick(int HState) {
	unsigned short index1 = 0;
	unsigned short index2 = 0;
	unsigned short index3 = 0;
	//unsigned short index4;
	unsigned placeHolder = 0;
	unsigned char keys = sizeof(harmonyTone) / sizeof(harmonyTone[0]);
	switch(HState) {
		case Init2:
			HState = WaitPlay2;
			//index4 = 1;
			index3 = 0;
			index2 = 0;
			index1 = 0;
			silenceH();
			break;
		case WaitPlay2:
			if (!songPaused) {
				HState = Harmony;
				songFinished = 0;
			}
			else {
				HState = WaitPlay2;
		}
		break;
		case Harmony:
			if (songPaused) {
				silenceH();
				placeHolder = HState;
				HState = HP;
			}
			if (harmonyTone[index1][index2] == -1) {
				index1 = index1 + 1;
				index2 = 0;
			}
			if (index1 == keys) {
				songFinished = 1;
				songPaused = 1;
				HState = Init2;
				index1 = 0;
				break;
			}
			playH(harmonyTone[index1][index2]);
			HState = HNL;
			break;
		case HR:
			if (songPaused) {
				silenceH();
				placeHolder = HState;
				HState = HP;
				break;
			}
			if (index3 < harmonyToneR[index1][index2]) {
				index3 = index3 + 1;
				HState = HR;
			}
			else {
				index3 = 0;
				index2 = index2 + 1;
				HState = Harmony;
			}
			break;
		case HP:
			if (songPaused) {
				HState = HP;
			}
			else {
				if (placeHolder == HNL) {
					playH(harmonyTone[index1][index2]);
				}
				HState = placeHolder;
			}
			break;
	}
	return HState;
}*/
 //NOKIA LCD 5110
 #define F_CPU 8000000UL
 #define LCD_RST_SET PORTD |= (1 << 0)
 #define LCD_RST_clr  PORTD &=~ (1<<0)

 #define LCD_DC_set   PORTD |=  (1<<1)
 #define LCD_DC_clr   PORTD &=~ (1<<1)

 #define SDIN_set     PORTD |=  (1<<2)
 #define SDIN_clr     PORTD &=~ (1<<2)

 #define SCLK_set     PORTD |=  (1<<3)
 #define SCLK_clr     PORTD &= ~(1<<3)
 const unsigned char font6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

void LCD_write_byte(unsigned char dat, unsigned char command)
{
	unsigned char i;

	if (command == 1) {
		LCD_DC_clr;
	}

	else {
		LCD_DC_set;
	}

	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		SDIN_set;
		else
		SDIN_clr;
		SCLK_clr;
		dat = dat << 1;
		SCLK_set;
	}
}

void LCD_wait()
{
	LCD_RST_clr;
	_delay_us(1);
	LCD_RST_set;

	_delay_us(1);

	LCD_write_byte(0x21, 1); // set LCD mode
	LCD_write_byte(0xc8, 1); // set bias voltage
	LCD_write_byte(0x06, 1); // temperature correction
	LCD_write_byte(0x13, 1); // 1:48
	LCD_write_byte(0x20, 1); // use bias command, vertical
	LCD_write_byte(0x0c, 1); // set LCD mode,display normally
	LCD_clear();             // clear the LCD
}

void LCD_clear()
{
	unsigned int i;

	LCD_write_byte(0x0c, 1);
	LCD_write_byte(0x80, 1);

	for (i=0; i<504; i++)
	{
		LCD_write_byte(0, 0);
	}
}

void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 1); // column
	LCD_write_byte(0x80 | X, 1);    // row
}

void LCD_write_char(unsigned char c)
{
	unsigned char line;

	c -= 32;

	for (line=0; line<6; line++) {
		LCD_write_byte(font6x8[c][line], 0);
	}
}

void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
{
	LCD_set_XY(X,Y);
	while (*s)
	{
		LCD_write_char(*s);
		s++;
	}
}
//END OF NOKI LCD 5110

#include <avr/io.h>
#include "simAVRHeader.h"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void silence(unsigned char input) {
  input = 0;
  set_PWM(0);
}
void play(unsigned short freqSound, unsigned char input) {
  input = 0;
  set_PWM(freqSound);
}
void rest(unsigned short piece) {
  unsigned short i = 0x00;
  for (i = 0; i < piece; i++) {
    while (!TimerFlag) {}
    TimerFlag = 0;
  }
}



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

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) {OCR3A = 0x0000; }
		else {OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

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

void set_PWMBass(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) {OCR3A = 0x0000; }
		else {OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWMBass_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}
void PWMBass_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
void set_PWMHarm(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) {OCR3A = 0x0000; }
		else {OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWMHarm_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}
void PWMHarm_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
//MUSIC NOTES
#define C 130.8 * 2
#define Cs 138.6 * 2 //also known as D FLAT
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


//END OF MUSIC NOTES

//REST LENGTHS

#define QUARTER 75/2
#define HALF 75
#define THREEFOURTHS 150
#define WHOLE 400

//END OF REST LENGTHS

//NOTE LENGTHS
//test song mary had a little lamb
unsigned short testSong[] = {E, D, C, D, E, E, E, D, D, D, E, G, G, E, D, C, D, E, E, E, E, D, D, E, D, C};
unsigned short testSongNoteLengths[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 200, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 600};
unsigned short testSongRestLengths[] = {25, 25, 25, 25, 50, 50, 150, 50, 50, 150, 50, 50, 150, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 150};
//unsigned short testSongHarmony[] = {G, F, E, F, G, G, G, E, E, E, G, B*2, B*2, G, F, E, F, G, G, G, G, F, F, G, F, E};



int main(void) {
    /* Insert DDR and PORT initializations */
  DDRA = 0x00;
  PORTA = 0x00;
  DDRB = 0xFF;
  PORTB = 0x00;

  unsigned char numVals = 0;
  unsigned char playButton = 0;
  unsigned short i = 0;
  unsigned short j = 0;
  unsigned char soundBuzzer = 0;

  //unsigned char tmpB = 0x00;
  //unsigned char tmpA = 0x00;
  TimetSet(1);
  TimerOn();
  PWM_on();
    /* Insert your solution below */
    while (1) {
      playButton = PINA & 0x01;
      if (playButton) {
        numVals = sizeof(testSong) / sizeof(int);
        for (i = 0; i < numVals; i++) {
          play(soundBuzzer, testSong[i]);
          rest(testSongNoteLengths[i]);
          silence(soundBuzzer);
          rest(testSongRestLengths);
        }
        playButton = PINA & 0x01;
        while (playButton) {
          playButton = PINA & 0x01;
        }
      }
    //
    //   tmpA = PINA;
    //   if (tmpA != 0x01) {
	  //      tmpB = 0x00;
    //   }
    //   else {
	  //      tmpB = 0x01;
    //   }
    //   PORTB = tmpB;
    // }
    return 0;
}
