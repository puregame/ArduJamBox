#ifndef _DIY_Radio_
#define _DIY_Radio_

enum Mode{
    Standby,
    Radio,
    Bluetooth,
    Aux
};

// Pinout:
  // # A0: Rotory A
  // # A1: Rotory B
  // # A2: Button on swithcer
  // # A4: DTA - Red on pcb
  // # A5: CLK - back on pcb

#define RADIO_FET     5
#define BLUETOOTH_FET 7
#define SPEAKER_FET   6
#define AMP_FET       5
#define ledPin        13

#define LED_HEX_ID    0x01
#define TEA5767_ID    0x60

#define off LOW
#define OFF LOW
#define on HIGH
#define ON HIGH

#define BLUETOOTH_PIN_MASK B10000000 // on port d7
#define BLUETOOTH_ON PORTD |= BLUETOOTH_PIN_MASK;
#define BLUETOOTH_OFF  PORTD &= ~BLUETOOTH_PIN_MASK;
#define BLUETOOTH_SWITCH PORTD ^= BLUETOOTH_PIN_MASK;

#define LED_PIN_MASK   B00000001
#define LED_SWITCH PORTB ^= LED_PIN_MASK;
#define LED_ON  PORTB |= LED_PIN_MASK;
#define LED_OFF PORTB &= ~LED_PIN_MASK;

#endif
