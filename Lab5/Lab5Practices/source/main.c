#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
  DDRB = 0xFF;
  PORTB = 0x00;
  while(1) {
    PORTB = 0x0F;
  }
}
