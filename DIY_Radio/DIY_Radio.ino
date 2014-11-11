#include "ClickEncoder.h"
#include "TEA5767.h"
#include <Wire.h>
#include "TimerOne.h"


// Pinout:
  // # A0: 
  // # A1: 
  // # A2: Button on swithcer
  // # A4: DTA - Red on pcb
  // # A5: CLK - back on pcb

#define radioFET     5
#define bluetoothFET 6
#define speakerFET   7
#define ledPin 13

TEA5767 radio;

ClickEncoder *encoder;
int16_t last,frequency = 9910;

byte old_mute=1;
byte mute=0;
long loopnum = 0;

byte old_signal_level=1;
byte signal_level=0;

void timerIsr() {
  encoder->service();
}

isr(TIMER2_COMPA_vect){
  rotationPos = rotationPos>10 ? 0 : rotationPos + 1; // increment rotation position from 0 to 11
  solid = solid ^ B00000010;

}

void setup() {
  Serial.begin(9600);
  encoder = new ClickEncoder(A1, A0, A2);
  Wire.begin();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  pinMode(ledPin, OUTPUT);
  
  last = -1;

  // setup timer2 for interrupt to blink or rotate
  cli(); // disable global interrupts 
  TCCR2B = 0; // reset control regesters
  TCCR2A = 0;

  OCR2A = 0xFF;
  TCCR2B |= (1 << WGM22)
    TCCR2B |= (1 << CS22); // timer2 1024 prescalar
    TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS20);
  TIMSK2 |= (1 << OCIE2A);
  sei(); // enable global interrupts
}

void loop() {  
  // digitalWrite(ledPin, LOW);
  // loopnum +=1;
  // if (loopnum % 10000 && radio.ReadData()){
  //   Serial.println(radio.getStereo());
  //   Serial.println(radio.getSignal());
  //   delay(100);

  // }

  frequency += encoder->getValue();
  
  if (frequency != last) {
    if (frequency < 8750){
      frequency = 10800;
    }
    else if (frequency > 10800){
      frequency = 8750;
    }
    last = frequency;
    Serial.print("New frequency: ");
    Serial.println(frequency);
    radio.SetFrequency(frequency);
  }  
  
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      case ClickEncoder::Clicked:
        Serial.println("pressed!");
        radio.ReadData();
        Serial.print("Signal Level is: ");
        Serial.println(radio.getSignal());
        break;
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      case ClickEncoder::DoubleClicked:
          Serial.println("ClickEncoder::DoubleClicked");
          encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
          Serial.print("  Acceleration is ");
          Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
    }
  }    
}
