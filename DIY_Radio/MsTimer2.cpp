/*
  MsTimer2.h - Using timer2 with 1ms resolution
  Javier Valencia <javiervalencia80@gmail.com>
  
  History:
  	29/May/09 - V0.5 added support for Atmega1280 (thanks to Manuel Negri)
  	19/Mar/09 - V0.4 added support for ATmega328P (thanks to Jerome Despatis)
  	11/Jun/08 - V0.3 
  		changes to allow working with different CPU frequencies
  		added support for ATMega128 (using timer2)
  		compatible with ATMega48/88/168/8
	10/May/08 - V0.2 added some security tests and volatile keywords
	9/May/08 - V0.1 released working on ATMEGA168 only
	

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <MsTimer2.h>

unsigned long MsTimer2::msecs;
void (*MsTimer2::func)();
volatile unsigned long MsTimer2::count;
volatile char MsTimer2::overflowing;
volatile unsigned int MsTimer2::tcnt2;

void MsTimer2::set(unsigned long ms, void (*f)()) {
	float prescaler = 0.0;
	TIMSK2 &= ~(1<<TOIE2); // enable overflow interrupt
	TCCR2A &= ~((1<<WGM21) | (1<<WGM20)); // set mode to fast PWM, OCRA is TOP, update OCRx at BOTTOm, TOV flag sets on TOP
	TCCR2B &= ~(1<<WGM22);
	ASSR &= ~(1<<AS2);
	TIMSK2 &= ~(1<<OCIE2A);
	
	if ((F_CPU >= 1000000UL) && (F_CPU <= 16000000UL)) {	// prescaler set to 64
		TCCR2B |= (1<<CS22);
		TCCR2B &= ~((1<<CS21) | (1<<CS20));
		prescaler = 64.0;
	} else if (F_CPU < 1000000UL) {	// prescaler set to 8
		TCCR2B |= (1<<CS21);
		TCCR2B &= ~((1<<CS22) | (1<<CS20));
		prescaler = 8.0;
	} else { // F_CPU > 16Mhz, prescaler set to 128
		TCCR2B |= ((1<<CS22) | (1<<CS20));
		TCCR2B &= ~(1<<CS21);
		prescaler = 128.0;
	}
	
	tcnt2 = 256 - (int)((float)F_CPU * 0.001 / prescaler);
	
	if (ms == 0)
		msecs = 1;
	else
		msecs = ms;
		
	func = f;
}

void MsTimer2::start() {
	count = 0;
	overflowing = 0;
	TCNT2 = tcnt2;
	TIMSK2 |= (1<<TOIE2);
}

void MsTimer2::stop() {
	TIMSK2 &= ~(1<<TOIE2);
}

void MsTimer2::_overflow() {
	count += 1;
	
	if (count >= msecs && !overflowing) {
		overflowing = 1;
		count = 0;
		(*func)();
		overflowing = 0;
	}
}

ISR(TIMER2_OVF_vect) {
	TCNT2 = MsTimer2::tcnt2;
	MsTimer2::_overflow();
}

