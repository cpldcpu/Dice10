/*
 * Tiny10Dice.c
 *
 *  Author: tim
 */ 

//#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "tinytouchlib.h"

// superplex tables 
uint8_t leds_accu[6];

uint8_t lednum=0;
uint8_t dicestate=1;

PROGMEM const uint8_t diceplay[]={
  0x00,  //00
  0x08,  //01
  0x01,  //02
  0x09,  //03
  0x05,  //04
  0x0d,  //05
  0x07,  //06
};

PROGMEM const uint8_t sp_port[]={
  0x00,
  0x00,
  0x04,
  0x04,
    
};

PROGMEM const uint8_t sp_ddr[]={
  0x04,
  0x01,
  0x04,
  0x05,
};

uint16_t mainctr=0;

void inittimer(void)
{
  TCCR0A = 0;
  TCCR0B = _BV(WGM02)|_BV(CS02); 
  OCR0A = (uint16_t)((F_CPU/256.0f)/1000.0f); // 1000 Hz
  TIMSK0 = _BV(OCIE0A);
  sei();
}

// 50Hz Interrupt
ISR (TIM0_COMPA_vect)
{
  lednum++;
  if (lednum==4) lednum=0;
  
  if ((1<<lednum)&pgm_read_byte(&diceplay[dicestate])) {
    PORTB=pgm_read_byte(&sp_port[lednum]);
    DDRB=pgm_read_byte(&sp_ddr[lednum]);
   }
   else
   {
     DDRB=0;
   }
}

int main(void)
{
  
    CCP=0xD8;		// configuration change protection, write signature
    CLKPSR=0;		// set cpu clock prescaler =1 (8Mhz) (attiny 4/5/9/10)
    
    inittimer();

	tinytouch_init();
	
  DDRB|=_BV(PB2);
  
	while(1) {
  	if (tinytouch_sense()==tt_on) {dicestate++; if (dicestate==7) dicestate=1;}  // increase shown value on touch event
  	_delay_ms(10);
        
    
}
  }
