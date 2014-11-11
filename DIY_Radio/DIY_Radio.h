#ifndef _DIY_Radio_
#define _DIY_Radio_

enum Mode{
    standby,
    radio,
    bluetooth,
    aux
};

// Pinout:
  // # A0: Rotory A
  // # A1: Rotory B
  // # A2: Button on swithcer
  // # A4: DTA - Red on pcb
  // # A5: CLK - back on pcb

#define radioFET     5
#define bluetoothFET 6
#define speakerFET   7
#define ledPin       13
#define LED_PIN_MASK   B0010000
#define LED_SWITCH PORTB ^= LED_PIN_MASK;
#define LED_ON  PORTB |= LED_PIN_MASK;
#define LED_OFF PORTB &= !LED_PIN_MASK

#endif